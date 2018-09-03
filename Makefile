all: tclshell.exe 

SRCS += $(wildcard *.cpp)
OBJS += $(patsubst %.cpp, %.o, $(SRCS))

CXXFLAGS += -g -O0 -I$(PWD)/third-party/include

tclshell.exe: $(OBJS)
	g++ $(CXXFLAGS) $(OBJS) -L$(PWD)/third-party/lib -ltcl8.6 -lcurses -o $@

clean:
	rm -rf $(OBJS)
