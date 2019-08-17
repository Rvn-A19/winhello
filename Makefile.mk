# Sample build make.
CC = cl
CXXFLAGS = /W3 /O2 /EHsc
EXTFLAGS = /nologo
LIBS = advapi32.lib

all: winhello.exe

winhello.exe:
    $(CC) $(EXTFLAGS) $(CXXFLAGS) winhello.cc $(LIBS)

rebuild: clean all

clean:
    del winhello.exe winhello.obj
