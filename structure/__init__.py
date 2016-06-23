__author__ = 'Frederick NEY Stephane OVERLEN'
from struct import Struct
from collections import namedtuple


class Request(object):
	__HEADER = 0x76767676

	def __init__(self, type, offset, length, handle):
		self.TYPE = type
		self.OFFSET = offset
		self.LENGTH = length
		self.HANDLE = handle

	def get_struct(self):
		struct = namedtuple("cmd_data", "header type offset length handle")
		return struct(self.__HEADER, self.TYPE, self.OFFSET, self.LENGTH, self.HANDLE)


class Reply(object):
	HEADER = 0x87878787

	def __init__(self, reply):
		structure = Struct("!3I")
		reply_struct = structure.unpack(reply)
		struct = namedtuple("message", "header error handle")
		struct = struct(reply_struct)
		self.__HEADER = struct.header
		self.__ERROR = struct.error
		self.__HANDLE = struct.handle


	def verif_struct(self, handle):
		if self.__HEADER == self.HEADER:
			if self.__HANDLE == handle:
				return 0
			else:
				return -2
		else:
			return -1

	def get_error(self):
		return self.__ERROR

