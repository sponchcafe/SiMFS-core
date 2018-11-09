import uuid
import os

########################################################################### 
def new_filename(name: str) -> str:
    ''' Create a rondomized filename based on a readable name

    Arguments
    name : file base name 

    Returns
    Absolute path with a basename of the form "name_<8 random chars>"
    '''

    uid = '_'.join((name, str(uuid.uuid1())[:8]))
    return os.path.abspath(uid)
