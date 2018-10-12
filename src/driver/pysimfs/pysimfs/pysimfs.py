import subprocess

import asyncio
import threading

import os
import json
import time
import uuid
import numpy as np

from . import IO, ComponentLog

###############################################################################
class Simulation:

    ########################################################################### 
    def __init__(self, name=None, tmpdir='./pysimfs_tmp'):
        self.tmpdir = tmpdir
        self.matched = set()
        self.components = []
        self.unmatched_in = set()
        self.unmatched_out = set()
        self.open_pipes = set()
        try:
            os.mkdir(self.tmpdir)
        except FileExistsError:
            print(f'Temporary folder {self.tmpdir} exists.')

    ########################################################################### 
    def __enter__(self):
        return self

    ########################################################################### 
    def __exit__(self, exc_type, exc_value, traceback):
        self.clear()

    ########################################################################### 
    def add(self, comp):

        self.components.append(comp)
        new_in = comp.inputs
        new_out = comp.outputs
                                            
        self.unmatched_in.update(new_in)
        self.unmatched_out.update(new_out)
                                                                    
        updates = {}
        new_matches = set()
        
        for elem in self.unmatched_in:
            if elem in self.unmatched_out:
                # match        
                new_matches.add(elem)
                pipe = self.new_pipe(elem.name)
                updates[elem.name] = pipe
                self.matched.add(IO(pipe, elem.dtype))

        self.unmatched_out.difference_update(new_matches)
        self.unmatched_in.difference_update(new_matches)
        
        for c in self.components:
            c.remap_inputs(updates)
            c.remap_outputs(updates)

    ########################################################################### 
    def make_pipes(self):
        for elem in self.matched:
            try:
                os.mkfifo(elem.name)
                self.open_pipes.add(elem.name)
            except:
                print(f'Failed to create pipe {elem.name}')

    ########################################################################### 
    def run(self):

        for f in self.unmatched_in:
            assert os.path.exists(f.name), f'File "{f.name}" does not exist.'
     
        self.make_pipes()

        # new event loop in a thread (to run 2 loops in case the environment is already runnign asyncio -> jupyterhub)
        loop = asyncio.new_event_loop()
        asyncio.get_child_watcher().attach_loop(loop) # register loop (?)
        
        commands = asyncio.gather(
                *(Simulation.call_simfs_async(c.call, *c.opts, **c.params) for c in self.components), loop=loop
        )                  
        
        #Run the commands
        start = time.time()
        t = threading.Thread(target=self.run_loop, args=(loop, commands))
        t.start()
        t.join()
        end = time.time()
        print(f'Simulation completed after {round(end-start, 2)} seconds.')
        return [ComponentLog(p, e) for (p, e) in self.results]
    
    ########################################################################### 
    def run_loop(self, loop, commands):
        self.results = loop.run_until_complete(commands)

    ########################################################################### 
    def clear(self):
        self.components = []
        for p in self.open_pipes:
            try:
                os.remove(p)
            except Exception as e:
                print('error', e)
        try:
            os.rmdir(self.tmpdir)
        except Exception as e:
            print('error', e)
          
    ########################################################################### 
    def new_pipe(self, name):
        uid = '_'.join((name, str(uuid.uuid1())[:8]))
        return os.path.abspath(os.path.join(self.tmpdir, uid))

    ########################################################################### 
    def get_results(self):
        res = {}
        for o in self.unmatched_out:
            if o.name == os.devnull: 
                continue
            res[o.name] = np.fromfile(o.name, o.dtype)
        return res
           
    ########################################################################### 
    @property
    def inputs(self):
        inputs = set()
        for c in self.components:
            inputs.update(c.inputs)
        return inputs
              
    ########################################################################### 
    @property
    def outputs(self):
        outputs = set()
        for c in self.components:
            outputs.update(c.outputs)
        return outputs
                 
    ########################################################################### 
    @staticmethod
    def call_simfs(cmd, *opts, **params):
        proc = subprocess.Popen(
            [cmd]+list(opts), 
            stdin=subprocess.PIPE, 
            stdout=subprocess.PIPE, 
            stderr=subprocess.PIPE
        )
        out, err = proc.communicate(input=json.dumps(params).encode('utf8'))
        return json.loads(out.decode().strip()), err.decode().strip()
                   
    ########################################################################### 
    @staticmethod
    async def call_simfs_async(cmd, *opts, **params):
        proc = await asyncio.create_subprocess_exec(
            cmd, *opts, 
            stdin=asyncio.subprocess.PIPE, 
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE
        )
        print(f"started {cmd}")
        out, err = await proc.communicate(input=json.dumps(params).encode('utf-8'))
        return json.loads(out.decode().strip()), err.decode().strip()
