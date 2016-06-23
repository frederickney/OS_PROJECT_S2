# -*- coding: utf-8 -*-
#Note : minix-fs types are little endian

from lib.minixfs.constantes import *
from lib.minixfs.minix_inode import *
from lib.minixfs.minix_superbloc import *
from lib.minixfs.bloc_device import *
from math import ceil
from array import *
from string import *


def chunks(l, n):
    for i in range(0, len(l), n):
        yield l[i:i+n]


class minix_file_system(object):
    def __init__(self, filename):
        self.disk = bloc_device(BLOCK_SIZE, filename)
        self.superBloc = minix_superbloc(self.disk)
        self.bitmap_inode_offset = 2
        self.bitmap_bloc_offset = self.superBloc.s_imap_blocks + 2
        self.inode_offset = self.bitmap_bloc_offset + self.superBloc.s_zmap_blocks
        self.bloc_offset = self.superBloc.s_firstdatazone
        self.empty_bloc = BLOCK_SIZE * '\0'
        raw_inode_map = self.disk.read_bloc(2, self.superBloc.s_imap_blocks)
        self.inode_map = bitarray(endian='little')
        self.inode_map.frombytes(raw_inode_map)
        raw_zone_map = self.disk.read_bloc(self.bitmap_bloc_offset, self.superBloc.s_zmap_blocks)
        self.zone_map = bitarray(endian='little')
        self.zone_map.frombytes(raw_zone_map)
        self.inodes_list = []
        self.inodes_list.append(minix_inode())
        raw_inodes = self.disk.read_bloc(self.inode_offset, int(ceil(self.superBloc.s_ninodes * 32 / BLOCK_SIZE)))
        for i, chunk in enumerate(chunks(raw_inodes, 32)):
            self.inodes_list.append(minix_inode(chunk, i + 1))
        return
    
    #return the first free inode number available
    #starting at 0 and upto s.n_inodes-1. 
    #The bitmap ranges from index 0 to inod_num-1
    #Inode 0 is never and is always set.
    #according to the inodes bitmap
    def ialloc(self):
        inode = 0
        while self.inode_map[inode]:
            inode += 1
        self.inode_map[inode] = 1
        self.inodes_list[inode] = minix_inode(num=inode)
        return inode

    #toggle an inode as available for the next ialloc() 
    def ifree(self, inodnum):
        self.inode_map[inodnum] = False
        free_range = self.inode_map[inodnum: (inodnum + BLOCK_SIZE)].tobytes()
        self.disk.write_bloc(self.bitmap_bloc_offset + (inodnum / BLOCK_SIZE), free_range)
        return 
    #return the first free bloc index in the volume. The bitmap
    #indicate the index from the bloc zone, add first_datazone then
    #to the bloc index
    def balloc(self):
        test = 0
        while self.zone_map[test]:
            test += 1
        self.zone_map[test] = 1
        blk_null = '\0' * BLOCK_SIZE
        self.disk.write_bloc(test + self.superBloc.s_firstdatazone, blk_null)
        return test + self.superBloc.s_firstdatazone

    #toggle a bloc as available for the next balloc() 
    #blocnum is an index in the zone_map
    def bfree(self, blocnum):
        self.zone_map[blocnum] = False
        free_range = self.zone_map[blocnum:(blocnum + BLOCK_SIZE)].tobytes()
        self.disk.write_bloc(self.inode_offset + (blocnum / BLOCK_SIZE), free_range)
        return
    
    def bmap(self, inode, blk):
        addr_size = 2 #bytes
        half_blk = BLOCK_SIZE / addr_size
        if blk < 7:
            return inode.i_zone[blk]
        blk -= 7
        if blk < half_blk:
            bloc_indirect = self.disk.read_bloc(inode.i_indir_zone)
            offset = blk * (BLOCK_SIZE / half_blk)
            return struct.unpack_from('<H', bloc_indirect, offset)[0]
        blk -= half_blk

        if blk < (half_blk * half_blk):
            bloc_indirect_1 = self.disk.read_bloc(inode.i_dbl_indr_zone)
            offset_1 = blk * (BLOCK_SIZE / (half_blk * half_blk))
            address_indirect_2 = struct.unpack_from("<H", bloc_indirect_1, offset_1)[0]
            bloc_indirect_2 = self.disk.read_bloc(address_indirect_2)
            offset_2 = ((blk % half_blk) * (BLOCK_SIZE / half_blk))
            return struct.unpack_from('<H', bloc_indirect_2, offset_2)[0]
    
    #lookup for a name in a directory, and return its inode number, given inode directory dinode
    def lookup_entry(self, dinode, name):
        for x in range(0, dinode.i_size):
            bloc = self.disk.read_bloc(self.bmap(dinode, x))
            for o in range(0, MINIX_INODE_PER_BLOCK):
                inode = bloc[INODE_SIZE * o : INODE_SIZE * (o + 1)]
                if name in inode[2:16]:
                    return struct.unpack("<H", inode[0:2])[0]
        return

    #find an inode number according to its path
    #ex : '/usr/bin/cat'
    #only works with absolute paths
                   
    def namei(self, path):
        inode = MINIX_ROOT_INO
        if path == '/':
            return inode
        for x in path[1:len(path)].split('/'):
            inode = self.lookup_entry(self.inodes_list[inode], x)
        return inode

    
    def ialloc_bloc(self, inode, blk):
        if (blk < len(inode.i_zone)):
            if (inode.i_zone[blk] == 0):
                inode.i_zone[blk] = self.balloc()

                inode.i_size += BLOCK_SIZE
            return inode.i_zone[blk]
        blk -= len(inode.i_zone)
        if blk < 512:
            if inode.i_indir_zone == 0:
                inode.i_indir_zone = self.balloc()

            indirection_bloc = self.disk.read_bloc(inode.i_indir_zone)
            if indirection_bloc[blk] == 0:
                indirection_bloc[blk] = self.balloc()
                self.disk.write_bloc(inode.i_indir_zone, indirection_bloc)
                inode.i_size +=BLOCK_SIZE
            return indirection_bloc[blk]
        return
    
    #create a new entry in the node
    #name is an unicode string
    #parameters : directory inode, name, inode number
    def add_entry(self, dinode, name, new_node_num):
        for i in range(0, int(round(dinode.i_size / BLOCK_SIZE, 0)) + 1):
            bmap_bloc = self.disk.read_bloc(self.bmap(dinode, i))
            for j in range(0, BLOCK_SIZE / DIRSIZE):
                if struct.unpack_from("<H", bmap_bloc, j * DIRSIZE)[0] == 0:
                    new_block = array('c', bmap_bloc)
                    struct.pack_into("<H", new_block, j*DIRSIZE, new_node_num)
                    struct.pack_into("<14s", new_block, 2 + j*DIRSIZE, name)
                    self.disk.write_bloc(self.bmap(dinode, i), new_block)
                    return
        new_block = array('c', '\0' * BLOCK_SIZE)
        struct.pack_into("<H", new_block, 0, new_node_num)
        struct.pack_into("<14s", new_block, 2, name)
        self.disk.write_bloc(self.ialloc_bloc(dinode, i+1), new_block)
        return

    #delete an entry named "name" 
    def del_entry(self, inode, name):
        name = name.ljust(14, "\0")
        for i in range(0, int(round(inode.i_size / BLOCK_SIZE, 0)) + 1):
            bmap_bloc = self.disk.read_bloc(self.bmap(inode, i))
            for j in range(0, BLOCK_SIZE / DIRSIZE):
                if struct.unpack_from("<14s", bmap_bloc, 2 + j *DIRSIZE)[0] == name:
                    new_block = array('c', self.disk.read_bloc(self.bmap(inode, i)))
                    struct.pack_into("<H", new_block, j*DIRSIZE, 0)
                    self.disk.write_bloc(self.bmap(inode, i), new_block)
                    return
        return -1


