CC = gcc
CFLAGS = -Wall -std=c99

main: main.o helper.o max32list.o
	${CC} main.o helper.o max32list.o ${CFLAGS} -o out/main.bin
main.o: src/helper.h src/max32list.h
	${CC} -c src/main.c ${CFLAGS}
helper.o: src/helper.h
	${CC} -c src/helper.c ${CFLAGS}
max32list.o: src/max32list.h
	${CC} -c src/max32list.c ${CFLAGS}
clean:
	@rm -fr *.o
