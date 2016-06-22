# -*- coding: utf-8 -*-
from lib.minixfs.constantes import *
from struct import *


class minix_superbloc(object):
    def __init__(self, bloc_device):
        bloc = bloc_device.read_bloc(MINIX_SUPER_BLOCK_NUM)
        (self.s_ninodes, self.s_nzones, self.s_imap_blocks, self.s_zmap_blocks, self.s_firstdatazone, self.s_log_zone_size, self.s_max_size, self.s_magic, self.s_state) = unpack('<6HL2H', bloc[0:MINIX_SUPER_BLOCK_SIZE])
