# -*- coding: utf-8 -*-
# emulate a simple bloc device using a file
# reading it only by bloc units
from lib.minixfs.constantes import *


class bloc_device(object):
    def __init__(self, blksize, pathname):
        self.Handle = 0
        self.blksize = blksize
        self.pathname = pathname
        return

    def read_bloc(self, bloc_num, numofblk=1):
        self.Header = 0x76767676
        self.Type = 0x0
        self.Handle += 1
        self.Offset = bloc_num * BLOCK_SIZE
        self.Length = BLOCK_SIZE * numofblk
        f = open(self.pathname, 'r')
        f.seek(self.Offset, 0)
        Buffer = f.read(self.Length)
        f.close()
        return Buffer

    def write_bloc(self, bloc_num, bloc):
        self.Header = 0x76767676
        self.Type = 0x1
        self.Handle += 1
        self.Offset = bloc_num * BLOCK_SIZE
        self.Length = len(bloc)
        self.Payload = bloc
        f = open(self.pathname, 'rw+')
        f.seek(self.Offset, 0)
        f.write(bloc)
        f.close()
        return 
