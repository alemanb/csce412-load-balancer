CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = loadbalancer
SRCS = main.cpp Request.cpp RequestQueue.cpp WebServer.cpp LoadBalancer.cpp \
       Firewall.cpp Config.cpp Logger.cpp Switch.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Header dependencies, so editing a .h rebuilds what uses it
main.o: Config.h Firewall.h LoadBalancer.h Logger.h Switch.h
Request.o: Request.h
RequestQueue.o: RequestQueue.h Request.h
WebServer.o: WebServer.h Request.h
LoadBalancer.o: LoadBalancer.h Config.h Firewall.h Logger.h RequestQueue.h WebServer.h Request.h
Firewall.o: Firewall.h
Config.o: Config.h
Logger.o: Logger.h
Switch.o: Switch.h LoadBalancer.h

run: $(TARGET)
	./$(TARGET)

docs:
	doxygen Doxyfile

clean:
	rm -f $(TARGET) $(TARGET).exe *.o

.PHONY: all run docs clean
