### This Makefile was written for GNU Make. ###
CC       = clang
CXX      = clang
INCLUDE  = -I./header
CFLAGS   = -pipe -Wall -Wextra -march=native $(INCLUDE)
CXXFLAGS = -pipe -Wall -Wextra -march=native $(INCLUDE)
LDFLAGS  = -pipe -s
TARGET   = sayss
OBJ      = $(addsuffix .o, $(basename $(TARGET)))


ifeq ($(OS),Windows_NT)
    TARGET   := $(addsuffix .exe, $(TARGET))
    # CFLAGS   += -finput-charset=utf-8 -fexec-charset=cp932
    # CXXFLAGS += -finput-charset=utf-8 -fexec-charset=cp932
else
    TARGET := $(addsuffix .out, $(TARGET))
endif

%.exe :
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.o, $^) $(LDLIBS) -o $@
%.out :
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.o, $^) $(LDLIBS) -o $@


.PHONY : all
all : $(TARGET)

$(TARGET) : $(OBJ)


.PHONY : clean
clean :
	$(RM) $(TARGET) $(OBJ)
.PHONY : objclean
objclean :
	$(RM) $(OBJ)
