main: main.o Algoritmos.o ArbolBinarioBusqueda.o Mediciones.o
	gcc -o main main.o Algoritmos.o ArbolBinarioBusqueda.o Mediciones.o -lpthread
main.o: main.c
	gcc -c main.c
Algoritmos.o: Algoritmos.c
	gcc -c Algoritmos.c
ArbolBinarioBusqueda.o: ArbolBinarioBusqueda.c
	gcc -c ArbolBinarioBusqueda.c
Mediciones.o: Mediciones.c
	gcc -c Mediciones.c
clean:
	rm *.o
run: main
	./main
