# -*- coding: utf-8 -*-
# emulate a simple bloc device using a file
# reading it only by bloc units
from constantes import *
import socket
import struct
from structure import  *



class bloc_device(object):
    Header = 0x76767676
    Type = 0
    Handle = 0
    Offset = 0
    Length = 0

    def __init__(self, blksize, pathname):
        self.pathname = pathname
        self.blksize = blksize
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        #self.sd = self.socket.connect(("192.168.1.19", 6600))
        self.request_struct = struct.Struct("!5I")
        self.reply_struct = struct.Struct("!3I")
        self.fd = open(pathname, 'rw+')
        return

    def read_bloc(self, bloc_num, numofblk=1):
        self.Header = 0x76767676
        self.Type = 0x0
        self.Handle += 1
        self.Offset = bloc_num * BLOCK_SIZE
        self.Length = BLOCK_SIZE * numofblk
        self.fd.seek(self.Offset, 0)
        buffer_read = self.fd.read(self.Length)
        return buffer_read

    def write_bloc(self, bloc_num, bloc):
        self.fd.seek(self.blksize * bloc_num)
        buffer_to_write = buffer(bloc, 0, BLOCK_SIZE)
        for block in range(BLOCK_SIZE):
            self.fd.write(buffer_to_write[block])
        return