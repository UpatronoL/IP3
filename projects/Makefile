PROGRAM = test

# The '-rpath $(CURDIR)/frameworks' flag stores the path for the runtime
# libraries in the executable itself.  If the frameworks are moved from their
# compile time location then the executable will stop working.
#
# Frameworks installed in /Library/Frameworks/ are searched automatically.
# Installing frameworks/*.framework in /Library/Frameworks (by copying them)
# allows the -rpath flag to be removed from LDLIBS and makes the executable
# file independent of the compile time location of these frameworks.  Using '-F
# /Library/Frameworks' in CFLAGS and LDFLAGS will search that location for
# header files and libraries at compile time, completely removing any
# dependency on the 'frameworks' directory.

CFLAGS  = -I /opt/homebrew/include
LDFLAGS = -L /opt/homebrew/lib
LDLIBS  = -lSDL2_net -rpath /opt/homebrew/lib

$(PROGRAM) : $(PROGRAM).o

$(PROGRAM).o : $(PROGRAM).c

run : $(PROGRAM)
	./$(PROGRAM)

clean : .FORCE
	rm -f $(PROGRAM) $(PROGRAM).o  *~

.FORCE :
