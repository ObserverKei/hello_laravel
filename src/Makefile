OBJ=shopping.out
OBJS += main.o
OBJS += tcpconnect.o
OBJS += controlsql.o
OBJS += controlmod.o
OBJS += createhtml.o
OBJS += search.o
LIB= sqlite3.o

INCLUDE_PATH=../include
CC=gcc
ARMCC=arm-linux-gcc

$(OBJ):$(OBJS) $(LIB)
	$(CC) $^ -o $@ -lpthread -ldl -g 

%.o:%.c
	$(CC) -c $< -o $@ -I$(INCLUDE_PATH) 

.PHONY:
clean:
	rm shopping.out tcpconnect.o
distclean:
	rm $(OBJS) shopping.out
test:
	valgrind --tool=memcheck --leak-check=full ./$(OBJ)
gdb:
	gdb $(OBJ) core
go:
	./$(OBJ)
reset:
	make
	make distclean
	make
	make go
arm:
	$(ARMCC) $(OBJS:o=c) sqlite3.c -o ~/nfs/arm-shopping.out -lpthread -ldl -I$(INCLUDE_PATH) 
