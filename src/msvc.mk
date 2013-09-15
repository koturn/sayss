### This Makefile was written for nmake. ###
CC       = cl
CPP      = cl
INCS     = /Iheader
CFLAGS   = /O2 /Wall $(INCS)
CPPFLAGS = /O2 /Wall $(INCS)
LDFLAGS  = /O2
RM       = del /F
TARGET   = sayss.exe
OBJ      = $(TARGET:.exe=.obj)
SRC      = $(TARGET:.exe=.cpp)


all : $(TARGET)

$(OBJ) : $(SRC)


clean :
	$(RM) $(TARGET) $(OBJ)
objclean :
	$(RM) $(OBJ)
