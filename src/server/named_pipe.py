import os
import logging
import uuid

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
        self._open()

    def _open(self):
        try:
            os.mkfifo(self)
            NamedPipe.opened.append(self)
        except FileExistsError:
            logging.log(logging.WARN, f'Pipe {self} not created, file exists.')

    def close(self):
        try:
            os.remove(self)
        except FileNotFoundError:
            logging.log(logging.WARN, f'Pipe {self} not removed, no such file.')
            
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
