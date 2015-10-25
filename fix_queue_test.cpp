#include "fix_queue.h"
#include <iostream>
#include <thread>
#include <chrono>

struct foo
{
    foo(int ii, double dd) : i(ii), d(dd)
    {
    }

	int i;
	double d;
};

util::fix_queue<foo, 1024> g_queue;

void enqueue_thread()
{
    static int i = 0;
    static double d = 0.0;
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++i; d+= 1.1;
        g_queue.enqueue(i, d);
    }
}

void dequeue_thread()
{
    while(true)
    {
        foo* f = g_queue.peek();
        if(f)
        {
		    std::cout << f->i << " " << f->d << std::endl;
		    g_queue.dequeue();
        }
    }
}

int main()
{
    std::thread t1(dequeue_thread);
    std::thread t2(enqueue_thread);

    t1.join();
    t2.join();

	return 0;
}
