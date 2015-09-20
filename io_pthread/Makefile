EXEC = io_pthread
OBJS += func/main.o
OBJS += func/dir_op.o
OBJS += func/bmp_show.o
OBJS += func/lish_show.o


CC = arm-linux-gcc
INC = -I.
CFLAGS = ${INC} -g -lpthread

all:${EXEC}
	cp ./io_pthread /nfs
${EXEC} : ${OBJS}
	${CC}  ${LDFLAGS} -o $@ ${OBJS}   -lpthread

%.o:%.c %.h
	$(CC)   -c -o $@ $^ -lpthread

.PHONY : clean
clean :
	
	-rm -f ${OBJS} ${EXEC}
