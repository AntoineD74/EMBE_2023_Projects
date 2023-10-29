OBJS = main.o modbus.o crc.o

all: command

command: $(OBJS)
	g++ -o command $(OBJS)

%.o : src/%.c
	g++ -o $@ -c $< -I include
