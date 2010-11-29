#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

class Parent {
public:
	Parent() : p_status(&status), thread_enable(0) {}
	void start_server() {
		int ret = pthread_create(&tid, NULL, thread_func, NULL);
		if (ret == 0)
			printf("Start a thread\n");
		else
			printf("Can't start a thread: %s\n", strerror(ret));
	}
	static void *thread_func(void *data) {
		while (state == 1) {
			printf("In thread function, %d\n", count++);
			sleep(1);
		}
		return NULL;
	}
	void stop() {
		state = 0;
		if (pthread_join(tid, (void**)p_status) == 0)
			printf("End a thread\n");
		else
			printf("Can't end a thread: %d\n", status);
	}
private:
	static int state, count;
	pthread_t tid;
	int status, *p_status;
	int thread_enable;
};

int Parent::state = 1;
int Parent::count = 0;

class A : public Parent {
public:
	void start() {
		Parent::start_server();
	}
	void stop() {
		Parent::stop();
	}
};

int main()
{
	A a;
	a.start();
	sleep(3);
	a.stop();
	return 0;
}

