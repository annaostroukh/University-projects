 # Project: BMS, Project 1 - Error Correction Code (Makefile)
 # Author: Anna Ostroukh <xostro00>

CC=g++ 
CFLAGS=-c -std=gnu++0x -Wall -pedantic

LIB_CSRC = rs.c galois.c berlekamp.c crcgen.c 
LIB_HSRC = ecc.h
LIB_OBJS = rs.o galois.o berlekamp.o crcgen.o 

TARGET_PROGS = bms1A bms1B 

TARGETS = $(TARGET_LIB) $(TARGET_PROGS) 

all: $(TARGETS)


$(TARGET_LIB): $(LIB_OBJS)
	$(RM) $@
	$(AR) cq $@ $(LIB_OBJS)

bms1A: bms1A.o galois.o berlekamp.o crcgen.o rs.o 
	 $(CC) bms1A.o galois.o berlekamp.o crcgen.o rs.o -o bms1A

bms1A.o: bms1A.cpp 
	$(CC) $(CFLAGS) bms1A.cpp

bms1B: bms1B.o galois.o berlekamp.o crcgen.o rs.o
	 $(CC) bms1B.o galois.o berlekamp.o crcgen.o rs.o -o bms1B

bms1B.o: bms1B.cpp 
	$(CC) $(CFLAGS) bms1B.cpp

clean:
	rm -f *.o bms1A bms1B
	rm -f *.err *.out *.ok
	rm -f *~
	rm -f xostro00.zip rs.c galois.c berlekamp.c crcgen.c ecc.h bms1A.cpp bms1B.cpp ecc.h.gch


 

