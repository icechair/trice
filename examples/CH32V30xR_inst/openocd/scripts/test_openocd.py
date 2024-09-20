from openocd import OpenOcd

with OpenOcd(port=50001) as oocd:
    #oocd.halt()
    registers = oocd.read_registers(['pc', 'sp'])

    print('Program counter: 0x%x' % registers['pc'])
    print('Stack pointer: 0x%x' % registers['sp'])

    #oocd.resume()
