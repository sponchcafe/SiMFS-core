from cmdutils import NamedPipe, Component
import argparse
import sys
import json
import websockets, asyncio

def shutdown():
    NamedPipe.close_all()
    sys.exit(0)


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
    NamedPipe.set_tmpdir('./tmp')
    Component.add_path('../bin')
    Component.add_paramfile(args.parameters)
    Component.debug = False

    N = params['GLOBAL']['molecules']

    photons = []
    fifos = []


    ### core simulation
    for s in (str(i) for i in range(N)):
        
        dif = Component('sim_dif', seed=s)
        tee = Component('tee', NamedPipe(), stdin=dif.stdout)
        exi = Component('sim_exiAlpha', output=NamedPipe(), stdin=tee.stdout)
        det = Component('sim_detGauss', output=NamedPipe(), input=tee.operands[0])
        ph2 = Component('sim_ph2', seed=s, excitation=exi.output, detection=det.output, output=NamedPipe())        
        photons.append(ph2.output)

    bkg = Component('sim_bkg', output=NamedPipe())
    mix = Component('sim_mix', *(photons+[bkg.output]))
    trc = Component('sim_sum', stdin=mix.stdout)

    ### end simulation
    print('done')

    # Here we serve
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