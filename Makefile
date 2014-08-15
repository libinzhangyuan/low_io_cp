# flags
CC = gcc
CFLAGS = -Wall -std=c99
LFLAGS =

# args
RELEASE =0
BITS =

# [args] 生成模式. 0代表debug模式, 1代表release模式. make RELEASE=1.
ifeq ($(RELEASE),0)
	# debug
	CFLAGS += -g
else
	release
	CFLAGS += -static -O3 -DNDEBUG
	LFLAGS += -static
endif

# [args] 程序位数. 32代表32位程序, 64代表64位程序, 其他默认. make BITS=32.
ifeq ($(BITS),32)
	CFLAGS += -m32
	LFLAGS += -m32
else
	ifeq ($(BITS),64)
	CFLAGS += -m64
	LFLAGS += -m64
else
	endif
endif


all : low_io_cp
	
low_io_cp : low_io_cp.o
	$(CC) $(LFLAGS) -o $@ $^

low_io_cp.o : low_io_cp.c
	$(CC) $(CFLAGS) -c $<

