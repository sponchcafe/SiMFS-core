from IPython.core.magic import Magics, magics_class, line_magic
import json
from pysimfs import Simulation, Component
from . import basepath
import os

###############################################################################
@magics_class
class SimfsDefaultMagics(Magics):

    ###########################################################################
    shortcuts = {
            'dif': os.path.join('mol', 'simfs_dif'),
            'cnf': os.path.join('mol', 'simfs_cnf'),
            'sft': os.path.join('mol', 'simfs_sft'),
            'fcs': os.path.join('fcs', 'simfs_fcs'),
            'pls': os.path.join('fcs', 'simfs_pls'),
            'efi': os.path.join('fcs', 'simfs_efi'),
            'ph2': os.path.join('ph2', 'simfs_ph2'),
            'spl': os.path.join('utl', 'simfs_spl'),
            'mix': os.path.join('utl', 'simfs_mix'),
            'buf': os.path.join('utl', 'simfs_buf'),
            'img': os.path.join('utl', 'simfs_img')
            }

    ###########################################################################
    @line_magic
    def simfs_default(self, line):

        call = os.path.join(basepath, SimfsDefaultMagics.shortcuts[line])

        params = Simulation.call_simfs(call, 'list')[0]
        varname = f'{line.split("/")[-1]}_params'
        params = json.dumps(params, indent=4).replace('"', "'")

        self.shell.set_next_input(
                f'# %simfs_default {line}\n{varname} = {params}', 
                replace=True
                )
