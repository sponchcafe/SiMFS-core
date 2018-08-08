from subprocess import Popen, PIPE
import socket
import json
import threading
import os
from named_pipe import NamedPipe

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(('localhost', 0))

class Diffusion:

    exec_path = '/Users/tzickmantel/Entwicklung/sim/build/src/components/dif/simfs_dif'


    
    @staticmethod
    def validate(params):
        proc = Popen([Diffusion.exec_path, 'list'], stdin=PIPE, stdout=PIPE)
        out, err = proc.communicate(bytes(json.dumps(params), 'utf-8'))
        if not err:
            return json.loads(str(out, 'utf-8'))

    @staticmethod
    def run(params):
        port = sock.getsockname()[1]
        sock.listen(5)
        NamedPipe.close_all()
        pipe = NamedPipe(params['coordinate_output'])
        log = Diffusion.validate(params)
        t1 = threading.Thread(target=Diffusion._pipe2socket_, args=(params['coordinate_output'], ))
        t1.start()
        t2 = threading.Thread(target=Diffusion._run_process_, args=(params,))
        t2.start()
        log['coordinate_output'] = '{}:{}'.format(*sock.getsockname())
        return log
    
    @staticmethod
    def _run_process_(params):
        proc = Popen([Diffusion.exec_path], stdin=PIPE, stdout=PIPE)
        proc.communicate(input=bytes(json.dumps(params), 'utf-8'))

    @staticmethod
    def _pipe2socket_(pipe):
        client_sock, addr = sock.accept()
        with open(pipe, 'rb') as f:
            while True:
                chunk = f.read(1024*1024)
                if not chunk:
                    break
                chunk_size = len(chunk)
                bytes_left = chunk_size
                while bytes_left > 0:
                    bytes_left -= client_sock.send(chunk[(chunk_size-bytes_left):])

            client_sock.close()








    
    
