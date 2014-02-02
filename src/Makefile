CC := gcc
FLAGS := -Wall -Wextra

example1:	example1.c hashMap.o typedefs.h objFuncs.o
example2:	example2.c hashMap.o typedefs.h objFuncs.o

%.o:	%.c %.h
	$(CC) $(FLAGS) -c $< -o $@

%:	%.o %.c
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -f *.o example1
	# TODO
	# Better executable detection
	# find . -type f | while read path;do $(file $p | grep -i executable && rm $p);done


