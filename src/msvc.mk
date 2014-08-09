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
OBJ2       = charcode.obj
OBJ3       = $(GETOPT_DIR)\getopt.obj
OBJ4       = $(GETOPT_DIR)\getopt_long.obj
SRC1       = $(OBJ1:.obj=.cpp)
SRC2       = $(OBJ2:.obj=.cpp)
HEADER2    = $(OBJ2:.obj=.h)


all: $(TARGET)

$(TARGET): $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4)
	$(CPP) $(LDFLAGS) $**

$(OBJ1): $(SRC1) $(HEADER2)

$(OBJ2): $(SRC2) $(HEADER2)

$(OBJ3) $(OBJ4):
	cd $(GETOPT_DIR)  &&  $(MAKE) /f msvc.mk  &&  cd ..\..


test:
	./$(TARGET) --help
	./$(TARGET) こんにちは
	./$(TARGET) -l en -o hello.wav "Hello world"


allclean:
	$(RM) $(TARGET) $(OBJ1) $(OBJ2) *.exp *.lib *.wav *.stackdump *~
clean:
	$(RM) $(TARGET) $(OBJ1) $(OBJ2) *.exp *.lib
	cd $(GETOPT_DIR)  &&  $(MAKE) /f msvc.mk $@  &&  cd ..\..
cleanobj:
	$(RM) $(OBJ1) $(OBJ2)
