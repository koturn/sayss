### This Makefile was written for nmake. ###
CC         = cl
CPP        = cl
INCS       = /Ilibrary /Ilibrary\getopt
CFLAGS     = /O2 /Wall $(INCS)
CPPFLAGS   = /O2 /Wall $(INCS)
LDFLAGS    = /O2
RM         = del /F

GETOPT_DIR = library\getopt
TARGET     = sayss.exe
OBJ1       = $(TARGET:.exe=.obj)
OBJ2       = $(GETOPT_DIR)\getopt.obj
OBJ3       = $(GETOPT_DIR)\getopt_long.obj
SRC1       = $(TARGET:.exe=.cpp)


all : $(TARGET)

$(TARGET) : $(OBJ1) $(OBJ2) $(OBJ3)
	$(CPP) $(LDFLAGS) $**

$(OBJ1) : $(SRC1)

$(OBJ2) $(OBJ3) :
	cd $(GETOPT_DIR)  &&  $(MAKE) /f msvc.mk  &&  cd ..\..


allclean :
	$(RM) $(TARGET) $(OBJ1) *.wav *.stackdump *~
clean :
	$(RM) $(TARGET) $(OBJ1)
	cd $(GETOPT_DIR)  &&  $(MAKE) /f msvc.mk $@  &&  cd ..\..
objclean :
	$(RM) $(OBJ1)
