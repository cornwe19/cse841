##
# @file    Makefile
# @author  Austin Gregory
# @created 2013-11-25
##

CC = g++
CFLAGS = -Wall -pedantic

SOURCEEXT = cc
HEADEREXT = hpp

BUILDTARGET = DN

BUILDDIR = ./Build
CONFIGDIR = ./Configuration
DOCSDIR = ./Documents
SOURCEDIR = ./Source
INCLUDEDIR = $(SOURCEDIR)

OUTPUTS = DN-1.data DN-2.data DN-3.data

TARLIST = $(BUILDDIR) $(CONFIGDIR) $(DOCSDIR) $(SOURCEDIR) Makefile

SOURCES = $(wildcard $(SOURCEDIR)/*.$(SOURCEEXT))
OBJECTS = $(patsubst $(SOURCEDIR)/%.$(SOURCEEXT), $(BUILDDIR)/%.o, $(SOURCES))

all: $(BUILDTARGET)

$(BUILDTARGET): $(BUILDDIR)/$(OBJECTS)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) $(OBJECTS) -o $(BUILDTARGET)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.$(SOURCEEXT)
	$(CC) $(CFLAGS) -I $(INCLUDEDIR) -c $< -o $@

clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/*~ $(BUILDTARGET)
	rm -f $(CONFIGDIR)/*~
	rm -f $(DOCSDIR)/*~
	rm -f $(SOURCEDIR)/*~ $(INCLUDEDIR)/*~
	rm -f *~ $(OUTPUTS)

tar:
	make clean
	tar -cvf $(BUILDTARGET).tar $(TARLIST)
