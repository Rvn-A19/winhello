# Sample build make.
CC = cl
CXXFLAGS = /W3 /O2 /EHsc
EXTFLAGS = /nologo
LIBS = advapi32.lib
COMPILE = /c

all: winhello.exe

winhello.exe: pathjoin.obj userinformation.obj
    $(CC) $(EXTFLAGS) $(CXXFLAGS) winhello.cc pathjoin.obj userinformation.obj $(LIBS)

pathjoin.obj:
    $(CC) $(EXTFLAGS) $(CXXFLAGS) $(COMPILE) pathjoin.cc


userinformation.obj:
    $(CC) $(EXTFLAGS) $(CXXFLAGS) $(COMPILE) userinformation.cc


rebuild: clean all

clean:
    del winhello.exe *.obj
