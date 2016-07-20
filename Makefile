include Makefile.inc
HEADERS := $(wildcard *.h)
SOURCES := $(wildcard *.c)
CFLAGS=-Wall -pedantic -std=gnu99
EXE=ftp_mqtt
HOST=HOST
OBJ=bin/main.o bin/socket.o bin/cmd_interpreter.o bin/get_request.o bin/reply.o bin/file_opener.o bin/file_access.o bin/stdin_filter.o bin/server.o
OBJHOST=bin/onlineClient.o bin/cmd_interpreter.o bin/socket.o bin/stdin_filter.o bin/file_opener.o bin/file_access.o bin/get_request.o bin/request.o bin/get_reply.o

all: $(EXE) $(HOST)

compile: CFLAGS+= -g
compile: $(OBJ)

release: CFLAGS += -O3
release: $(OBJ)

$(HOST): $(OBJHOST)
	@echo "linking $@..."
	@$(CC) -o $@ $^

$(EXE): $(OBJ)
	@echo "linking $@..."
	@$(CC) -o $@ $^

bin/%.o: src/%.c
	@echo "compiling $<..."
	@mkdir -p bin/
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@echo "cleaning..."
	@rm -rf $(EXE) $(OBJ)
	@rm -rf $(HOST) $(OBJHOST)
	@rm -rf bin

doc:doc_c/index.html doc_python/index.html

doc_c/index.html: Doxyfile $(HEADERS) $(SOURCES)
	@echo "generating doc with doxygen..."
	@doxygen Doxyfile

doc_python/index.html: Doxyfile_python $(HEADERS) $(SOURCES)
	@echo "generating doc with doxygen..."
	@doxygen Doxyfile_python

clean_doc:
	@echo "cleaning doc..."
	@rm -rf doc_c
	@rm -rf doc_python