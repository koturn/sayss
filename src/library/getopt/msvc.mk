CC      = cl
CFLAGS  = /O2 /Wall
LDFLAGS = /O2
RM      = del /F
TARGET1 = getopt.obj
TARGET2 = getopt_long.obj
SRC1    = $(TARGET1:.obj=.c)
SRC2    = $(TARGET2:.obj=.c)


.SUFFIXES : .c .obj
.c.obj :
	$(CC) $(CFLAGS) $** /c /Fo$@


all : $(TARGET1) $(TARGET2)

$(TARGET1) : $(SRC1)

$(TARGET2) : $(SRC2) $(TARGET1:.obj=.h)


clean :
	$(RM) $(TARGET1) $(TARGET2)
