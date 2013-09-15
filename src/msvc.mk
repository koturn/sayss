### This Makefile was written for nmake. ###
CC       = cl
CPP      = cl
INCS     = /Ilibrary
CFLAGS   = /O2 /Wall $(INCS)
CPPFLAGS = /O2 /Wall $(INCS)
LDFLAGS  = /O2
RM       = del /F
TARGET   = sayss.exe
OBJ      = $(TARGET:.exe=.obj)
SRC      = $(TARGET:.exe=.cpp)


all : $(TARGET)

$(OBJ) : $(SRC)


allclean :
	$(RM) $(TARGET) $(OBJ) *.wav *.stackdump *~
clean :
	$(RM) $(TARGET) $(OBJ)
objclean :
	$(RM) $(OBJ)
