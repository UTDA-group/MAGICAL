#!gmake

TARGET_ARCH =
ADDCFLAGS = -g
CC = g++
CFLAGS = $(ADDCFLAGS) -I../h
ARCHIVE = text.a
LIBS = ../str/text.a ../list/text.a ../cmp/text.a ../random/text.a ../mem/text.a ../low/text.a
OBJS = wnbvec.o wnprm.o wnio.o wnrnzsgn.o

compile: $(ARCHIVE) examples selftest

$(ARCHIVE): $(OBJS)
	rm -f $(ARCHIVE)
	ar slrcu $(ARCHIVE) $(OBJS)
#	$(AR) $(ARCHIVE) $(OBJS)
#	$(RANLIB) $(ARCHIVE) 

selftest: selftest.o $(ARCHIVE) 
	$(CC) -o selftest selftest.o $(ARCHIVE) $(LIBS) -lm

examples: examples.o $(ARCHIVE) 
	$(CC) -o examples examples.o $(ARCHIVE) $(LIBS) -lm

clean:
	set nonomatch; rm -f *.o
	rm -f $(ARCHIVE)
	rm -f examples selftest
	rm -f core 'e'rrors.txt tmp

man:
	./manscript.csh

hfiles:
	./hscript.csh

all: clean compile hfiles man

