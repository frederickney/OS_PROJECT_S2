# -*- coding: utf-8 -*-
#minix est little endian par defaut
from lib.minixfs.constantes import *
from collections import namedtuple
from struct import Struct


class minix_inode(object):
    # inodes can be initializted from given values or
    # from raw bytes contents coming from the device
    def __init__(self, raw_inode=None, num=0, mode=0, uid=0, size=0, time=0, gid=0, nlinks=0, zone=[], indir_zone=0, dblr_indir_zone=0):

        if raw_inode is None:
            self.i_ino = num
            self.i_mode = mode
            self.i_uid = uid
            self.i_size = size
            self.i_time = time
            self.i_gid = gid
            self.i_nlinks = nlinks
            self.i_zone = zone
            self.i_indir_zone = indir_zone
            self.i_dbl_indr_zone = dblr_indir_zone
        #Structure of inode construction
        else:
            struct = Struct('<2H2I2B9H')
            data = namedtuple("minix_inode", "mode uid size time gid nlinks zone1 zone2 zone3 zone4 zone5 zone6 zone7 indir_zone dblr_indir_zone")
            data = data(struct.unpack(raw_inode))
            self.i_ino = num
            self.i_mode = data.mode
            self.i_uid = data.uid
            self.i_size = data.size
            self.i_time = data.time
            self.i_gid = data.gid
            self.i_nlinks = data.nlinks
            self.i_zone = [data.zone1, data.zone2, data.zone3, data.zone4, data.zone5, data.zone6, data.zone7]
            self.i_indir_zone = data.indir_zone
            self.i_dbl_indr_zone = data.dblr_indir_zone

    def __eq__(self, other):
        if isinstance(other, minix_inode):
            return self.i_ino == other.i_ino and \
                self.i_mode == other.i_mode and \
                self.i_uid == other.i_uid and \
                self.i_size == other.i_size and \
                self.i_time == other.i_time and \
                self.i_gid == other.i_gid and \
                self.i_nlinks == other.i_nlinks and \
                self.i_zone == other.i_zone and \
                self.i_indir_zone == other.i_indir_zone and \
                self.i_dbl_indr_zone == other.i_dbl_indr_zone

    def __repr__(self): #afficher
        return "minix_inode("+"num="+str(self.i_ino) + \
            ",mode="+str(self.i_mode) + \
            ",uid="+str(self.i_uid) + \
            ",size="+str(self.i_size) + \
            ",time="+str(self.i_time) + \
            ",gid="+str(self.i_gid) + \
            ",nlinks="+str(self.i_nlinks) + \
            ",zone="+str(eval(repr(self.i_zone))) + \
            ",indir_zone="+str(self.i_indir_zone) + \
            ",dblr_indir_zone="+str(self.i_dbl_indr_zone) + \
            ")"