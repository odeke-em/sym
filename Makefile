CC := gcc
THREAD_LIB := -pthread

symtable:	symTable.c hashlist.o
	    $(CC)  hashlist.o symTable.c -o sym

hashlist.o:   hashlist/hashList.c hashlist/hashList.h
	    $(CC) -c hashlist/hashList.c -o hashlist.o

clean:	  
	    rm -f *.o
