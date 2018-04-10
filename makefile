CC=mpicc
CFLAGS=-I

Main2_5: Main2_5.c MD5.c
	$(CC) -o Main2_5 Main2_5.c MD5.c -I.
