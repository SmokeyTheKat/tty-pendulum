PREFIX = ~/.local

all:
	gcc ./tty-pendulum.c -o ./tty-pendulum -Wall -lm
install:
	mkdir -p ${PREFIX}/bin/
	cp ./tty-pendulum ${PREFIX}/bin/
