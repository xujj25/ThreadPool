CC:= g++ -std=c++11 -g
AR:= ar -crv

lib/libthreadpool.a: build/condition_variable.o build/mutex.o build/thread_pool.o
	$(AR) $@ $^
build/condition_variable.o: include/condition_variable.hpp src/condition_variable.cpp
	$(CC) -I ./include -c src/condition_variable.cpp -o $@
build/mutex.o: include/mutex.hpp src/mutex.cpp
	$(CC) -I ./include -c src/mutex.cpp -o $@
build/thread_pool.o: include/thread_pool.hpp include/blocking_queue.hpp src/thread_pool.cpp
	$(CC) -I ./include -c src/thread_pool.cpp -o $@

clean:
	@rm -rf build/*.o lib/libthreadpool.a
