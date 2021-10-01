CCC = g++
LIBS = -L/usr/local/lib/ -lhiredis -lmysqlclient
GFLAGS = -Icalculate_load -Iresource_schedule -Iutils -Iutils/mysql -Iutils/redis
OBJS = calculate_load/calculate_load.o resource_schedule/schedule.o utils/resource_info_utils.o utils/mysql/mysql_config.o utils/mysql/mysql_tools.o utils/redis/redis_config.o utils/redis/redis_tools.o main.o
TARGET = main
RM = rm -rf

$(TARGET):$(OBJS)
	$(CCC) -o $(TARGET) $(OBJS) $(LIBS)

$(OBJS):%.o:%.cpp
	$(CCC) -c $(GFLAGS) $< -o $@

clean:
	$(RM) $(TARGET) $(OBJS)

