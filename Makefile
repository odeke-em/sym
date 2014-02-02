CC := gcc
FLAGS := -Wall -Wextra

example1:	example1.c hashMap.o typedefs.h objFuncs.o
	$(CC) $(FLAGS) example1.c objFuncs.o hashMap.o -o example1

hashMap.o:	hashMap.[ch] typedefs.h objFuncs.o
	$(CC) $(FLAGS)  -c hashMap.c -o hashMap.o

objFuncs.o:	objFuncs.[ch] typedefs.h
	$(CC) $(FLAGS) -c objFuncs.c -o objFuncs.o

clean:
	rm -f *.o example1
	# TODO
	# Better executable detection
	# find . -type f | while read path;do $(file $p | grep -i executable && rm $p);done


