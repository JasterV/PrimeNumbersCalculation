FLAGS = -Wall

all: controlador.c generador.c calculador.c
	gcc controlador.c -o controlador $(FLAGS)
	gcc generador.c -o generador $(FLAGS)
	gcc calculador.c -o calculador $(FLAGS)

controlador: controlador.c infonombre.h
	gcc -I controlador.c -o controlador -I . $(FLAGS)

generador: generador.c
	gcc generador.c -o generador $(FLAGS)
	
calculador: calculador.c infonombre.h
	gcc -I calculador.c -o calculador -I . $(FLAGS)

clean:
	rm -f calculador controlador generador
	ls
