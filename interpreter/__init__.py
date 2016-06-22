__author__ = 'Frederick NEY et Stephane OVERLEN'

import host
import structure
from lib.minixfs.minixfs import *
from lib.minixfs.hexdump import *
from lib.minixfs.constantes import *
from lib.minixfs.bloc_device import *
from lib.minixfs.minix_inode import *
from lib.minixfs.minix_superbloc import *


def read(args):
	if 0 == len(args):
		return 1
	elif '"' in args:
		cmd = split_data(args, '"')
		cmd = remove_unsuported_data(cmd)
		return 0
	else:
		cmd = args
		return 0


def split_data(args, char_to_split):
	data = args.split(char_to_split)
	return data


def remove_unsuported_data(args):
	while '' in args:
		args.remove('')
	return args


def write(args):
	cmd = []
	if 0 == len(args):
		return 1
	elif '"' in args:
		tmp = split_data(args, '"')
		tmp = remove_unsuported_data(tmp)
		if 1 == len(tmp):
			return 3
		if 2 == len(tmp):
			if '>' in tmp[0]:
				cmd = split_data(tmp[0], ' ')
				cmd.append(tmp[1])
				cmd = remove_unsuported_data(cmd)
				return 0
			elif '>' in tmp[1]:
				cmd = split_data(tmp[1], ' ')
				cmd.insert(0, tmp[0])
				cmd = remove_unsuported_data(cmd)
				return 0
			else:
				return 2
		if 3 == len(tmp):
			cmd = tmp
			return 0
	elif ' ' in args:
		tmp = split_data(args, ' ')
		tmp = remove_unsuported_data(tmp)
		if 1 == len(tmp):
			return 3
		elif 2 == len(tmp):
			return 4
		elif 3 == len(tmp):
			if '>' in tmp[1]:
				return 0
			else:
				return 2
		else:
			return -1
	else:
		return 3


class Cmd(object):
	HOST = host.Host
	READ = 'read'
	WRITE = 'write'


	def analyse_cmd(self, args, cmd):
		if self.READ == cmd:
			if 0 != len(args):
				return read(args)
			else:
				return 1
		elif self.WRITE == cmd:
			if 0 != len(args):
				return write(args)
			else:
				return 1
		else:
			return -1

