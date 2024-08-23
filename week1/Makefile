CFLAGS = -Wall

all : hello

% : %.c
	cc -o $@ $<

clear : .FORCE
	rm -f hello hello.o 

.FORCE:
