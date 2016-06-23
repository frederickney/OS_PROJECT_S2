import os
import socket
import struct
from structure import *

u = Request(0x0, 1024, 1024, 1)

SD = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect = SD.connect(("192.168.161.124", 6600))
valeurs = u.get_struct()
request_struct = struct.Struct('!I I I I I')
reply_struct = struct.Struct('!I I I')
request_obj = Request(0x0, 1024, 1024, 1)
request = request_obj.get_struct()
request_packed = request_struct.pack(*request)
SD.sendall(request_packed)
reply_val = SD.recv(len(reply_struct.pack(0, 0, 0)))
print('initial len ' + str(len(reply_val)))
reply = Reply(reply_val)
print('verif ' + str(reply.verif_struct(1)))
print('error ' + str(reply.get_error()))
Buffer = SD.recv(1024)
print(Buffer)
prompt = "\n" + os.environ["HOME"] + ">> "

#while 1:
#    commande = raw_input(prompt)
#    
#    print(commande)
    
#    s.send(struu)

connect.close()
