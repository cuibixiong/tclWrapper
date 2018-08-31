all: tclWrapperShell.exe 

SRCS += $(wildcard *.cpp)
OBJS += $(patsubst %.cpp, %.o, $(SRCS))

CXXFLAGS += -g -O0 

tclWrapperShell.exe: $(OBJS)
	g++ $(OBJS) -ltcl8.5 -o $@

clean:
	rm -rf $(OBJS)
