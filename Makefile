CC = gcc
OBJECTS = hexanalyst.c
LIBS = -lm
CFLAGS = -Wall -O2
BINDIR = $(DESTDIR)/usr/bin
NAME = hexanalyst

hexanalyst: $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

