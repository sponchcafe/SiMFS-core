import subprocess as sp
import os
import json
import uuid

class Component():

    env = os.environ.copy()
    running = []
    debug = False
 
    def __init__(self, executable, *args, stdin=sp.PIPE, stdout=sp.PIPE, **kwargs):
        self.executable = executable
        self.cmd_args = []
        self.operands = []
        self.stdin = stdin
        self.stdout = stdout
        self.set_args(*args, **kwargs)
        self.env = self.__class__.env
        self.get_process()
        self.__class__.running.append(self)

        if self.__class__.debug:
            self.print_debug()
            
    def print_debug(self):
       print(16*'-')
       print(self.__repr__())
       print(16*'-')
       for k, v in self.__dict__.items():
           if k == 'env':
               v = '{... '+f"PARAMETERS={self.env['PARAMETERS']}"+' ...}'
           print(k, v)
       print()
            

    @classmethod
    def add_path(cls, execpath): 
        cls.env['PATH'] = cls.env['PATH'] + ':' + os.path.abspath(execpath)

    @classmethod
    def add_paramfile(cls, paramfile):
        cls.env['PARAMETERS'] = paramfile

    def set_args(self, *args, **kwargs):
        paramfile = json.load(open(self.__class__.env.get('PARAMETERS'), 'r'))
        try:
            for k in paramfile[self.executable]:
                setattr(self, k, paramfile[self.executable][k])
        except KeyError:
            pass
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
            self.pid = self.process.pid
        except:
            self.process = None
            self.stdin = None
            self.stdout = None 
            self.pid = None
            
    def communicate(self, **kwargs):
        return self.process.communicate(**kwargs)

    def __str__(self):
        return ' '.join(self.get_call())

    def __repr__(self):
        return '<ComponentRunner "' + str(self)[:64] + '">'
         
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
    
