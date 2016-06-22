from host import ftp_mqtt
import structure

__author__ = 'Frederick NEY et Stephane OVERLEN'


class Host ():

	def __init__(self):
		self.ftp_mqtt = ftp_mqtt.Connector()