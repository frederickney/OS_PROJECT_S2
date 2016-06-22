# -*- coding: utf-8 -*-
#Note : minix-fs types are little endian

from lib.minixfs.constantes import *
from lib.minixfs.minix_inode import *
from lib.minixfs.minix_superbloc import *
from lib.minixfs.bloc_device import *
import binascii


class minix_file_system(object):
    def __init__(self, filename):
#        self.filename = filename
        
        self.disk = bloc_device(BLOCK_SIZE, "m.img")
        
        self.sss = minix_superbloc(self.disk)
        e = self.sss.s_zmap_blocks
        
        a = bitarray(endian='little')
        a.frombytes(self.disk.read_bloc(2))
        self.inode_map = a
        
        b = bitarray(endian='little')
        b.frombytes(self.disk.read_bloc(3))
        
        c = bitarray(endian='little')
        c.frombytes(self.disk.read_bloc(4))
        
        d = bitarray(endian='little')
        d.frombytes(self.disk.read_bloc(5))
        
        self.zone_map = b + c + d

        return
    
    #return the first free inode number available
    #starting at 0 and upto s.n_inodes-1. 
    #The bitmap ranges from index 0 to inod_num-1
    #Inode 0 is never and is always set.
    #according to the inodes bitmap
    def ialloc(self):
        return

    #toggle an inode as available for the next ialloc() 
    def ifree(self, inodnum):
        return 
    #return the first free bloc index in the volume. The bitmap
    #indicate the index from the bloc zone, add first_datazone then
    #to the bloc index
    def balloc(self):
        return
    
    #toggle a bloc as available for the next balloc() 
    #blocnum is an index in the zone_map
    def bfree(self, blocnum):
        return
    
    def bmap(self, inode, blk):
        return
    
    #lookup for a name in a directory, and return its inode number, given inode directory dinode
    def lookup_entry(self, dinode, name):
        return
    
    #find an inode number according to its path
    #ex : '/usr/bin/cat'
    #only works with absolute paths
                   
    def namei(self, path):
        return
    
    def ialloc_bloc(self, inode, blk):
        return
    
    #create a new entry in the node
    #name is an unicode string
    #parameters : directory inode, name, inode number
    def add_entry(self, dinode, name, new_node_num):
        return

    #delete an entry named "name" 
    def del_entry(self, inode, name):
        return


