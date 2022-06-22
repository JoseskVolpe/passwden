objects = main.o

prefix = $(DESTDIR)/usr

bindir = $(prefix)/bin

all: output

output: main.o
	gcc $(objects) -o passwden

main.o: main.c
	gcc -c main.c

install:
	# Make directories
	mkdir -p $(bindir)

	#Copy binaries
	cp -dp --no-preserve=ownership passwden $(bindir)/passwden

uninstall:
	rm -f $(bindir)/passwden

clean:
	rm *.o passwden
