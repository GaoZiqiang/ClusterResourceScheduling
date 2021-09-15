all:main

main:main.cpp resource_schedule/* calculate_load/* utils/redis/* utils/mysql/* utils/eign_utils.h utils/resource_info_utils.h utils/resource_info_utils.cpp utils/split_str.h
	g++ main.cpp resource_schedule/* calculate_load/* utils/redis/* utils/mysql/* utils/eign_utils.h utils/resource_info_utils.h utils/resource_info_utils.cpp utils/split_str.h -o main -L/usr/local/lib/ -lhiredis -lmysqlclient

clean:
	rm -rf main

