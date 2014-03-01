CC := gcc
FLAGS := -Wall -Wextra
LIBS  := -lm -lpthread

refCount:	test/refCount.c objFuncs.o
example1:	example1.c hashMap.o typedefs.h objFuncs.o
primeLib:	primeLib.c hashMap.o typedefs.h objFuncs.o

%.o:	%.c %.h
	$(CC) $(FLAGS) -c $< -o $@

%:	%.o %.c
	$(CC) $(FLAGS) $< -o $@ $(LIBS)

clean:
	rm -f *.o example1
	# TODO
	# Better executable detection
	# find . -type f | while read p;do $(file $p | grep -i executable && rm $p);done
