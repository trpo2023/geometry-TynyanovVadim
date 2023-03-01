all: geometry

geometry: geometry.c
	gcc -Wall -Werror -o geometry geometry.c

open:
	./geometry

delete:
	rm geometry
