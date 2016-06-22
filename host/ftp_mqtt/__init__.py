__author__ = 'Frederick NEY et Stephane OVERLEN'
from host.ftp_mqtt import socket as ftp_mqtt_socket


class Connector():

	CMD_READ = 0x0
	CMD_WRITE = 0x1
	REQUEST = 0x76767676
	REPLY = 0x87878787

	def __init__(self):
		self.socket = ftp_mqtt_socket.Socket()

	def get_address(self):
		return self.socket.get_address()

	def get_port(self):
		return self.socket.get_port()
