# -*- MAKEFILE -*-
#  target: dependencies
#  	action

CC = gcc
src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d) # one dependency file for each source

CFLAGS = -g -lm

all : main

main: $(obj)
	$(CC) -o $@ $^ $(CFLAGS)

-include $(dep) # include all dep files in the makefile

# rule to generate a dep file by using the C preprocessor

%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:h=.o) >$@

.PHONY: clean

clean:
	clear
	rm -f *.o *.txt *.d ~tmp* main 
