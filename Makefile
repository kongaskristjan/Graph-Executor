#CXX = clang++

CXXFLAGS = -Wall -Wextra -std=c++14 -I.

CXXFLAGS += -g # debug mode
#CXXFLAGS += -O2 -D NDEBUG # performance mode

LIBS = -pthread
BIN = test

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
HDRS = $(wildcard *.hpp)

all: $(BIN)
	@echo "all compilation done"

$(OBJS): $(HDRS) Makefile

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(BIN) *.o *~
