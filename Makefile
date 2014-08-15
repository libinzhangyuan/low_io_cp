CFLAGS=-std=c99

low_io_cp : low_io_cp.o

low_io_cp.o : low_io_cp.c
	cc -c low_io_cp.c

