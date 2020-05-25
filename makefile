run:
	gcc -Wall -g project.c -o project `pkg-config --cflags --libs gtk+-2.0`
