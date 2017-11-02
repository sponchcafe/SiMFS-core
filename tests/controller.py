import subprocess as sp
import os
import argparse
import sys
import json
import uuid

class Component():

    env = os.environ.copy()

    def __init__(self, executable, *args, stdin=sp.PIPE, stdout=sp.PIPE, **kwargs):
        self.executable = executable
        self.cmd_args = []
        self.operands = []
        self.stdin = stdin
        self.stdout = stdout
        self.set_args(*args, **kwargs)
        self.env = self.__class__.env
        self.get_process()

    @classmethod
    def add_path(cls, execpath): 
        cls.env['PATH'] = cls.env['PATH'] + ':' + os.path.abspath(execpath)

    @classmethod
    def add_paramfile(cls, paramfile):
        cls.env['PARAMETERS'] = paramfile

    def set_args(self, *args, **kwargs):
        for k in kwargs:
            arg_dashed = k.replace('_', '-')
            if len(k) > 1:
                pre = '--'
            else:
                pre = '-'
            if type(kwargs[k]) == bool and kwargs[k]:
                self.cmd_args.append(pre + arg_dashed)
            if type(kwargs[k]) != bool:
                self.cmd_args.append(pre + arg_dashed)
                self.cmd_args.append(str(kwargs[k]))
            setattr(self, k, kwargs[k])
            
        for a in args:
            self.cmd_args.append(str(a))
            self.operands.append(a)

    def get_call(self):
        return [self.executable] + self.cmd_args

    def get_process(self):
        try:
            self.process = sp.Popen(self.get_call(), stdin=self.stdin, stdout=self.stdout, env=self.__class__.env)
            self.stdin = self.process.stdin
            self.stdout = self.process.stdout
        except FileNotFoundError:
            self.process = None
            self.stdin = None
            self.stdout = None 
        
    def communicate(self, **kwargs):
        return self.process.communicate(**kwargs)

    def __str__(self):
        return ' '.join(self.get_call())

    def __repr__(self):
        return '<ComponentRunner "' + str(self) + '">'
         
class NamedPipe(str):

    tmpdir = ''
    opened = []

    def __new__(cls, path=None):
        if not path:
            path = uuid.uuid4().hex
        path = os.path.abspath(os.path.join(cls.tmpdir, path))
        obj = str.__new__(cls, path)
        return obj

    def __init__(self, path=None):
        self.open()

    def open(self):
        try:
            os.mkfifo(self)
            NamedPipe.opened.append(self)
        except FileExistsError:
            print('Cannot create {}. File exists. Use overwrite option to force overwrite.'.format(self))
  
    def close(self):
        try:
            os.remove(self)
        except FileNotFoundError:
            print('File {} not found.'.format(self))

    @classmethod 
    def close_all(cls):
        for p in cls.opened:
            p.close()
        cls.opened = []

    @staticmethod
    def set_tmpdir(tmpdir):
        NamedPipe.tmpdir = tmpdir

    @staticmethod
    def get_opened():
        return NamedPipe.opened
    


if __name__ == '__main__':
    
    # define controller arguments
    parser = argparse.ArgumentParser(description='Run a FCS simulation.')
    parser.add_argument('-p', '--PARAMETERS', dest='parameters')

    # parse argument json
    args = parser.parse_args()
    
    with open(args.parameters, 'r') as f:
        params = json.load(f)

    setup = params['SETUP']
    server = setup['server']

    # setup
    Component.add_path('../bin')
    Component.add_paramfile(args.parameters)
    NamedPipe.set_tmpdir('./tmp')

    N = params['GLOBAL']['molecules']

    photons = []
    fifos = []


### core simulation

    for s in (str(i) for i in range(N)):
        
        dif = Component('dif', seed=s)
        tee = Component('tee', NamedPipe(), stdin=dif.stdout)
        exi = Component('exiAlpha', output=NamedPipe(), stdin=tee.stdout)
        det = Component('detGauss', output=NamedPipe(), stdin=open(tee.operands[0], 'rb'))
        ph2 = Component('ph2', seed=s, excitation=exi.output, detection=det.output, output=NamedPipe())        
        photons.append(ph2.output)

    bkg = Component('bkg')
    mix = Component('mix', *(photons+[bkg.stdout]))
    trc = Component('bin', stdin=mix.stdout)
    
### end simulation


    def shutdown():
        NamedPipe.close_all()
        sys.exit(0)

    # Here we serve
    import websockets, asyncio
    async def handover(websocket, path):
        while(True):
            d = trc.stdout.read(4)
            if d == b'':
                shutdown()
            await websocket.send(str(int.from_bytes(d, byteorder='little')))
            await asyncio.sleep(setup['delay'])

    start_server = websockets.serve(handover, server['sumphotons']['host'], server['sumphotons']['port'])

    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()