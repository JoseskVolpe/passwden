#
# makefile
#
# Copyright 2022 Josesk Volpe <joseskvolpe@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
#
#
#


objects = message.o configuration.o main.o

prefix = $(DESTDIR)/usr
bindir = $(prefix)/bin

all: output

output: $(objects)
	gcc $(objects) -o passwden -linih

main.o: main.c
	gcc -c main.c

configuration.o: configuration.c configuration.h
	gcc -c configuration.c configuration.h

message.o: message.c message.h
	gcc -c message.c message.h

install:
	# Make directories
	mkdir -p $(bindir)

	#Copy binaries
	cp -dp --no-preserve=ownership passwden $(bindir)/passwden

uninstall:
	rm -f $(bindir)/passwden

clean:
	rm -f *.o *.gch passwden
