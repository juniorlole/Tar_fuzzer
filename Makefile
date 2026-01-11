CC=gcc

.PHONY: all clean

all: Fuzzer

Fuzzer: src/Fuzzer.c
	$(CC) -o fuzzer src/Fuzzer.c
	chmod 4755 fuzzer

clean:
	rm -f fuzzer
	
succ:
	rm  success_*
	
arch:
	rm archive_*


