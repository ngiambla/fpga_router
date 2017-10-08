# Pick a platform below.  X11 for Linux/Mac/Unix, and WIN32 for windows.
# In either X11 or WIN32, Postscript is also simultaneously available.
# You can also pick NO_GRAPHICS, which will allow your code to compile without
# change on any platform, but no graphics will display (all calls are stubbed
# out).
# 
# Compiling to support X11 requires the X11 development libraries. On Ubuntu, use
# 'sudo apt-get install libx11-dev' to install.
# Please look online for information on how to install X11 on other Linux distributions.

# Please note that a Mac can run the X11 graphics if the X11 library is installed.

PLATFORM = X11
#PLATFORM = WIN32
#PLATFORM = NO_GRAPHICS

#HDR = graphics.h easygl_constants.h
#SRC = graphics.cpp example.cpp 

HDR = graphics.h circutil.h circuit.h lblck.h sblck.h router.h
SRC = graphics.cpp circutil.cpp circuit.cpp lblck.cpp sblck.cpp router.cpp

#EXE = example

EXE = circutil
BACKUP_FILENAME=`date "+backup-%Y%m%d-%H%M.zip"`
FLAGS = -g -Wall -D$(PLATFORM)
UNAME := $(shell uname)

# Need to tell the linker to link to the X11 libraries.
# WIN32 automatically links to the win32 API libraries (no need for flags)

$(System is $(UNAME))

ifeq ($(UNAME), Darwin)
	ifeq ($(PLATFORM),X11)
	   GRAPHICS_LIBS = -lX11 -L/usr/X11/lib -I/usr/X11/include
	endif
else
	ifeq ($(PLATFORM),X11)
	   GRAPHICS_LIBS = -lX11
	endif
endif

$(EXE):
	g++ $(FLAGS) -std=c++11 -o circutil circutil.cpp circuit.cpp lblck.cpp sblck.cpp router.cpp graphics.cpp cconfig.cpp -lpthread $(GRAPHICS_LIBS)

# $(EXE): graphics.o circutil.o circuit.o lblck.o sblck.o router.o
# 	g++ $(FLAGS) graphics.o circutil.o circuit.o lblck.o sblck.o router.o $(GRAPHICS_LIBS) -lpthread -o $(EXE)


# graphics.o: graphics.cpp $(HDR)
# 	g++ -c $(FLAGS) graphics.cpp

# example.o: example.cpp $(HDR)
# 	g++ -c $(FLAGS) example.cpp



# backup:
# # Back up the source, makefile and Visual Studio project & solution. 
# 	echo Backing up your files into ${BACKUP_FILENAME}
# 	zip ${BACKUP_FILENAME} $(SRC) $(HDR) makefile easygl.sln easygl.vcxproj

# src:
# 	g++ -std=c++11 -o circutil circutil.cpp circuit.cpp lblck.cpp sblck.cpp router.cpp graphics.cpp -lpthread

clean:
	rm $(EXE)
