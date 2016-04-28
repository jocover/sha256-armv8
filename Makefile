CC = gcc
CFLAGS = -O3 -mcpu=generic+crypto
sha256:sha256.c
	 $(CC) $(CFLAGS) sha256.c sha256-armv8-aarch64.S -o sha256
