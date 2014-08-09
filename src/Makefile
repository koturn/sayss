### This Makefile was written for GNU Make. ###
CC       ?= g++
CXX      ?= g++
INCS      = -I./library
CFLAGS    = -pipe -Wall -Wextra $(COPTFLAGS)   $(INCS) $(if $(STD), $(addprefix -std=, $(STD)),)
CXXFLAGS  = -pipe -Wall -Wextra $(CXXOPTFLAGS) $(INCS) $(if $(STD), $(addprefix -std=, $(STD)),)
LDFLAGS   = -pipe $(LDOPTFLAGS)
TARGET    = sayss
OBJ       = $(addsuffix .o, $(basename $(TARGET)))


ifeq ($(OS),Windows_NT)
    TARGET   := $(addsuffix .exe, $(TARGET))
else
    TARGET := $(addsuffix .out, $(TARGET))
endif

# priority : DEBUG > LTO > OPT
ifeq ($(OPT),true)
    COPTFLAGS    = -flto -Ofast -march=native
    CXXOPTFLAGS  = -flto -Ofast -march=native
    LDOPTFLAGS   = -flto -Ofast -s
else
    COPTFLAGS    = -O3
    CXXOPTFLAGS  = -O3
    LDOPTFLAGS   = -s
endif
ifeq ($(LTO),true)
    COPTFLAGS    = -flto
    CXXOPTFLAGS  = -flto
    LDOPTFLAGS   = -flto
endif
ifeq ($(DEBUG),true)
    COPTFLAGS    = -O0 -g
    CXXOPTFLAGS  = -O0 -g
endif

%.exe:
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.o, $^) $(LDLIBS) -o $@
%.out:
	$(CXX) $(LDFLAGS) $(filter %.c %.cpp %.o, $^) $(LDLIBS) -o $@


.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJ)


.PHONY: test
test:
	./$(TARGET) --help
	./$(TARGET) こんにちは
	./$(TARGET) -l en -o hello.wav "Hello world"


.PHONY: allclean
allclean:
	$(RM) $(TARGET) $(OBJ) *.wav *.stackdump *~
.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJ)
.PHONY: cleanobj
objclean:
	$(RM) $(OBJ)
