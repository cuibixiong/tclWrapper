all: tclWrapperShell.exe 

SRCS += $(wildcard *.cpp)
OBJS += $(patsubst %.cpp, %.o, $(SRCS))

CFLAGS += -g -O0

tclWrapperShell.exe: $(OBJS)
	g++ $(OBJS) -o $@

clean:
	rm -rf $(OBJS)
