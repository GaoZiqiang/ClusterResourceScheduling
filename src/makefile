CCC = g++
LIBS = -L/usr/local/lib/ -lhiredis -lmysqlclient
# CATALOGS = calculate_load resource_schedule utils/mysql utils/redis
# GFLAGS = -Icalculate_load -Iresource_schedule -Iutils -Iutils/mysql -Iutils/redis
SRC = $(wildcard calculate_load/*.cpp resource_schedule/*.cpp utils/mysql/*.cpp utils/redis/*.cpp utils/*.cpp ./*.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRC))
TARGET = main
RM = rm -rf

$(TARGET) : $(OBJS)
	$(CCC) -o $(TARGET) $(OBJS) $(LIBS)

$(OBJS) : %.o:%.cpp
	$(CCC) -c $^ -o $@

.PHONY : clean
clean:
	$(RM) $(TARGET) $(OBJS)


