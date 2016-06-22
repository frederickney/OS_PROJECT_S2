__author__ = 'Frederick NEY et Stephane OVERLEN'
import socket


class Socket ():
	__PORT = 6600
	__ADDRESS = ''

	def __init__(self):
		self.__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	def get_port(self):
		return self.__PORT

	def get_address(self):
		return self.__ADDRESS

	def set_address(self, address):
		self.__ADDRESS = address

	def set_port(self, port):
		self.__PORT = port

	def connect(self):
		return self.__socket.connect((self.__ADDRESS, self.__PORT))