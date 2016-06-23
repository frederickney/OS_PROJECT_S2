# -*- coding: utf-8 -*-
# emulate a simple bloc device using a file
# reading it only by bloc units
from constantes import *
import socket
import struct
from structure import  *



class bloc_device(object):
    Type = 0
    Handle = 0
    Offset = 0
    Length = 0

    def __init__(self, blksize, pathname):
        self.pathname = pathname
        self.blksize = blksize
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sd = self.socket.connect(("192.168.161.124", 6600))
        self.request_struct = struct.Struct("!I I I I I")
        self.reply_struct = struct.Struct("!I I I")
        self.fd = open(pathname, 'rw+')
        reply_struct = struct.Struct('!I I I')
        self.len_reply = len(reply_struct.pack(0, 0, 0))
        return

    def read_bloc(self, bloc_num, numofblk=1):
        self.Type = 0x0
        self.Handle += 1
        self.Offset = bloc_num * self.blksize
        self.Length = BLOCK_SIZE * numofblk
        request_obj = Request(self.Type, self.Offset, self.Length, self.Handle)
        request = request_obj.get_struct()
        request_packed = self.request_struct.pack(*request)
        self.socket.sendall(request_packed)
        reply_val = self.socket.recv(self.len_reply)
        reply = Reply(reply_val)
        if 0 == reply.verif_struct(self.Handle):
            if 0 == reply.get_error():
                return self.socket.recv(self.Length)
            else:
                return
        else:
            self.fd.seek(self.Offset, 0)
            buffer_read = self.fd.read(self.Length)
            return buffer_read

    def write_bloc(self, bloc_num, bloc):
        self.Type = 0x1
        self.Handle += 1
        self.Offset = bloc_num * self.blksize
        self.Length = self.blksize
        request_obj = Request(self.Type, self.Offset, self.Length, self.Handle)
        request = request_obj.get_struct()
        request_packed = self.request_struct.pack(*request)
        self.socket.sendall(request_packed)
        buffer_to_write = buffer(bloc, 0, self.blksize)
        for block in range(self.blksize):
            self.socket.sendall(buffer_to_write[block])
        reply_val = self.socket.recv(self.len_reply)
        reply = Reply(reply_val)
        if 0 == reply.verif_struct(self.Handle):
            if 0 == reply.get_error():
                return
            else:
                return
        else:
            self.fd.seek(self.blksize * bloc_num)
            for block in range(self.blksize):
                self.fd.write(buffer_to_write[block])
            return