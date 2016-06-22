__author__ = 'Frederick NEY et Stephane OVERLEN'

import sys
import host
import cmd
import interpreter


class Switch(object):
	value = None

	def __new__(class_, value):
		class_.value = value
		return True


def case(*args):
		return any((arg == Switch.value for arg in args))


def usage():
		print(sys.argv[0])
		print("\tWrite:")
		print("\t\twrite <text to write into file [between double cote]> > <file name [double cote needed if space(s)]>")
		print("\tRead:")
		print("\t\tread <file name [double cote needed if space(s)]>")
		print("\tHelp:\n\t\th or help")
		print("\tExiting:\n\t\tquit or exit")


def get_error(int, line, cmd):
		while Switch(int):
			if case(0):
				break
			if case(1):
				print("Missing arguments to: " + cmd)
				print("See usages using h or help")
				break
			if case(2):
				print("Unsupported operator: " + cmd + " " + line)
				print("See usages using h or help")
				break
			if case(3):
				print("Write: Missing operand '>' after text \"" + cmd + " " + line + "\"")
				print("See usages using h or help")
				break
			if case(4):
				print("Write: Missing file name after '>' \"" + cmd + " " + line + "\"")
				print("See usages using h or help")
				break
			print("Unsupported command: " + cmd + " " + line)
			print("See usages using h or help")
			break


class Main(cmd.Cmd):
	CMD = ['read', 'write', 'exit', 'quit', 'help', 'h']
	interpreter = interpreter.Cmd()

	def complete_read(self, text, line, begidx, endidx):
		if not text:
			return

	def complete_write(self, text, line, begidx, endidx):
		if not text:
			return

	def do_help(self, line):
		usage()

	def do_h(self, line):
		usage()

	def do_write(self, line):
		get_error(self.interpreter.analyse_cmd(line, 'write'), line, 'write')

	def do_read(self, line):
		get_error(self.interpreter.analyse_cmd(line, 'read'), line, 'read')

	def do_quit(self, line):
		exit(1)

	def do_exit(self, line):
		exit(1)

	def do_EOF(self, line):
		return True

if __name__ == '__main__':
	print("Use h or help to display command usage")
	connection = host.ftp_mqtt.Connector()
	connection.socket.set_address("192.168.1.19")
	sd = connection.socket.connect()
	cmd_interpreter = Main()
	cmd_interpreter.sd = sd
	cmd_interpreter.connection = connection
	cmd_interpreter.cmdloop()
