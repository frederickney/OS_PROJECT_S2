/**
 * @project OS PROJECT S2
 * @file file_access.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 02/06/2016 18:06
 * @revision 1.0.1
 */

#include <fcntl.h>
#include <sys/wait.h>
#include "socket.h"
#include "file_access.h"
#include "file_opener.h"

int genfile(char *filename, char *workfile) {
	int STD;
	pid_t  pid;
	pid = fork();
	int status;
	if (pid < 0) {
		write(STDERR_FILENO, FORKERROR, strlen(FORKERROR));
	}
	else if (pid == 0) {
		write(STDIN_FILENO, DD, strlen(DD));
		char *IF, *OF, *bs;
		bs = "bs=1024";
		printf("%s\n", bs);
		IF = malloc(sizeof(char) * 1027);
		OF = malloc(sizeof(char) * 1027);
		IF = strcpy(IF, "if=");
		OF = strcpy(OF, "of=");
		IF = strcat(IF, filename);
		OF = strcat(OF, workfile);
		printf("%s\n", IF);
		printf("%s\n", OF);
		char **argv;
		argv = malloc(sizeof(char *) * 4);
		for (int i = 0; i < 4; i++) {
			argv[i] = malloc(sizeof(char) * 1024);
		}
		argv[0] = strcpy(argv[0], "/bin/dd");
		argv[1] = strcpy(argv[1], IF);
		argv[2] = strcpy(argv[2], OF);
		argv[3] = strcpy(argv[3], bs);
		STD = open("/dev/null", O_RDONLY);
		dup2(STD, STDERR_FILENO);
		dup2(STD, STDOUT_FILENO);
		close(STD);
		write(STDIN_FILENO, GEN_FILE, strlen(GEN_FILE));
		if (0 > execvp("/bin/dd", argv)) {
			return EXIT_FAILURE;
		}
		else {
			return EXIT_SUCCESS;
		}
	}
	else {
		while (wait(&status) != pid);
		if (file_exist(workfile)) {
			write(STDIN_FILENO, DONE, strlen(DONE));
			return 0;
		}
		else {
			write(STDIN_FILENO, FAIL, strlen(FAIL));
			exit(EXIT_FAILURE);
		}
	}
	return -1;
}

/**
 * reset offset to the beginning of the file and set the offset to specific location
 *
 *
 * @param int fd: file descriptor
 * @param int offset: it's at this offset that we will start writing or reading the file
 */
void set_offset(int fd, int offset) {
	lseek(fd, 0, SEEK_SET);
	lseek(fd, offset, SEEK_SET);
}

/**
 * read file function
 *
 *
 * @param int fd: file descriptor.
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @return int: error code.
 */
uint32_t read_file(int fd, data *data_struct) {
	int bread = 0;
	set_offset(fd, data_struct->request->offset);
	if(0 > (bread = read(fd, data_struct->payload, data_struct->request->length))) {
		return 3;
	}
	else {
		if(bread != data_struct->request->length) {
			return 2;
		}
		else {
			return 0;
		}
	}
}

/**
 * write file function
 *
 *
 * @param int fd: file descriptor.
 * @param data *data_struct: structure containing payload, error, request and reply.
 * @return int: error code.
 */
uint32_t write_file(int fd, data *data_struct) {
	int bwrite = 0;
	set_offset(fd, data_struct->request->offset);
	if(0 > (bwrite = write(fd, data_struct->payload, data_struct->request->length))) {
		return 3;
	}
	else {
		if(bwrite != data_struct->request->length) {
			return 2;
		}
		else {
			return 0;
		}
	}
}

int log_pid(char *pathname, pid_t pid) {
	int fd;
	if (file_exist(pathname)) {
		fd = open_file(pathname);
		set_offset(fd, SEEK_SET);
		if (0 > pid) {
			read(fd, &pid, sizeof(pid_t));
			return pid;
		}
		else {
			return pid;
		}
	}
	else if (0 < pid){
		if (0 == create_file(pathname)) {
			fd = open_file(pathname);
			set_offset(fd, SEEK_SET);
			if (0 < pid) {
				write(fd, &pid, sizeof(pid_t));
				return 0;
			}
			else {
				return -1;
			}
		}
		else
			return -2;
	}
	else {
		return -10;
	}
}