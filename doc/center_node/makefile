CC = gcc
GCC = g++
CFLAGS = -lpthread -lmysqlclient
OBJS = web_serv.o mysql_config.o mysql_tools.o
TARGET = server
RM = rm -rf

$(TARGET):$(OBJS)
	$(GCC) -o $(TARGET) $(OBJS) $(CFLAGS)

$(OBJS):%.o:%.cpp
	$(GCC) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)
