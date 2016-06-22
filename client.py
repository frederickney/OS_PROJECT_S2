import os
import socket
import structure
import struct

u = structure.structure(0x0, 1024, 1024, 1)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
t = s.connect(("192.168.1.19", 6600))

valeurs = u.get_struct()
packer = struct.Struct('I I I I I')
packed_data = packer.pack(*valeurs)
s.sendall(packed_data)

prompt = "\n" + os.environ["HOME"] + ">> "

#while 1:
#    commande = raw_input(prompt)
#    
#    print(commande)
    
#    s.send(struu)

s.close()
