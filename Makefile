#
# Student makefile for Cache Lab
# Note: requires a 64-bit x86-64 system 
# had -std=c99 initially
CC = gcc
CFLAGS = -g -Wall -std=c99 -m64
SHELL = /bin/bash

all: csim test-trans tracegen
	# Generate a handin tar file each time you compile
	-tar -cvf ${USER}-handin.tar  csim.c trans.c 

csim: csim.c cachelab.c cachelab.h testing_csim.c testing_csim.h
	$(CC) $(CFLAGS) -o csim csim.c cachelab.c testing_csim.c -lm 

test-trans: test-trans.c trans.o cachelab.c cachelab.h
	$(CC) $(CFLAGS) -o test-trans test-trans.c cachelab.c trans.o 

testing_trans_isaac: testing_trans.c trans.o cachelab.c cachelab.h trans.h
	$(CC) $(CFLAGS) -o testing_trans_isaac testing_trans.c cachelab.c trans.o 

tracegen: tracegen.c trans.o cachelab.c
	$(CC) $(CFLAGS) -O0 -o tracegen tracegen.c trans.o cachelab.c

trans.o: trans.c
	$(CC) $(CFLAGS) -O0 -c trans.c

CSIM_ARG_SETS = \
    "-s 1 -E 1 -b 1 -t traces/yi2.trace" \
    "-s 4 -E 2 -b 4 -t traces/yi.trace" \
    "-s 2 -E 1 -b 4 -t traces/dave.trace" \
    "-s 2 -E 1 -b 3 -t traces/trans.trace" \
    "-s 2 -E 2 -b 3 -t traces/trans.trace" \
    "-s 2 -E 4 -b 3 -t traces/trans.trace" \
    "-s 5 -E 1 -b 5 -t traces/trans.trace" \
    "-s 5 -E 1 -b 5 -t traces/long.trace"
test_csim:
	for csim_arg_set in $(CSIM_ARG_SETS); do \
		if diff <(./csim $$csim_arg_set) <(./csim-ref $$csim_arg_set) >/dev/null; then \
			echo "passed $$csim_arg_set"; \
		fi; \
	done 

# if diff <(./csim "$$csim_arg_set") <(./csim-ref "$$csim_arg_set") >/dev/null; then \
# 	echo "passed $$csim_arg_set"; \
# else \
# 	echo "FAIL $$csim"; \
# 	diff <(./csim "$$csim_arg_set") <(./csim-ref "$$csim_arg_set"); \
# fi; \
	
# if [[ -f $$file ]]; then \

# 	if diff <(command1) <(command2) >/dev/null ; then \
# if diff <(command1) <(command2) >/dev/null ; then \
# 	echo "The outputs are identical" \
# else \
# 	echo "The outputs are different" \
# fi


#
# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -f *.tar
	rm -f csim
	rm -f test-trans tracegen
	rm -f trace.all trace.f*
	rm -f .csim_results .marker
