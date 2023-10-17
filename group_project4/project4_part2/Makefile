SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = motorController

all: $(TARGET)

$(TARGET): $(OBJS)
	g++ -o $(TARGET) $(OBJS)

%.o : lib/%.cpp
	g++ -o $@ -c $<
