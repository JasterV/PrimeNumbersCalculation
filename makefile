FLAGS = -Wall

all: controlador.c generador.c calculador.c
	gcc controlador.c -o controlador $(FLAGS)
	gcc generador.c -o generador $(FLAGS)
	gcc calculador.c -o calculador $(FLAGS)

controlador: controlador.c common.h
	gcc -I controlador.c -o controlador -I . $(FLAGS)

generador: generador.c common.h
	gcc generador.c -o generador $(FLAGS)
	
calculador: calculador.c common.h
	gcc -I calculador.c -o calculador -I . $(FLAGS)

clean:
	rm -f calculador controlador generador
	ls
