CC = gcc
CFLAGS = -I/usr/include/mysql -lmysqlclient -I/usr/include/pthread -lpthread
all:
	$(CC) $(CFLAGS) getmsg.c setmsg.c
