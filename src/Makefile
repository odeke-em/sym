CC := gcc
FLAGS := -Wall -Wextra -lm

example1:	example1.c hashMap.o typedefs.h objFuncs.o
firstNPrimes:	firstNPrimes.c hashMap.o typedefs.h objFuncs.o

%.o:	%.c %.h
	$(CC) $(FLAGS) -c $< -o $@

%:	%.o %.c
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -f *.o example1
	# TODO
	# Better executable detection
	# find . -type f | while read path;do $(file $p | grep -i executable && rm $p);done
