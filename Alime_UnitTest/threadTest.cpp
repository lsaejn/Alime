#include "easyTest.h"
#include "../Alime/Alime/base/thread/thread.h"
#include "../Alime/Alime/base/thread/SpinLock.h"
#include "../Alime/Alime/base/thread/countDownLatch.h"

#include <string>
#include <stdio.h>



TEST_UNIT(test_SpinLock)
{
    int count = 3;
    Alime::CountDownLatchXp latch(count);
    Alime::Thread t1([&latch]() {
        //Sleep(3000);
        latch.countDown();
        });
    Alime::Thread t2([&latch]() {
        Sleep(3000);
        latch.countDown();
        });
    Alime::Thread t3([&latch]() {
        Sleep(3000);
        latch.countDown();
        });
    latch.wait();


}




void threadFunc()
{
    printf("tid=%d\n", Alime::this_thread::get_id());
}

void threadFunc2(int x)
{
    printf("tid=%d, x=%d\n", Alime::this_thread::get_id(), x);
}

void threadFunc3()
{
    printf("tid=%d\n", Alime::this_thread::get_id());
    Alime::this_thread::sleep_for(500);
}

class Foo
{
public:
    explicit Foo(double x)
        : x_(x)
    {
    }

    void memberFunc()
    {
        printf("tid=%d, Foo::x_=%f\n", Alime::this_thread::get_id(), x_);
    }

    void memberFunc2(const std::string& text)
    {
        printf("tid=%d, Foo::x_=%f, text=%s\n", Alime::this_thread::get_id(), x_, text.c_str());
    }

private:
    double x_;
};

/*
to save time, we use muduo::thread 's testCase
*/

TEST_UNIT(test_Thread)
{

    Alime::Thread t1(threadFunc, false);
    t1.start();
    printf("t1.tid=%d\n", t1.get_id());
    t1.join();

    Alime::Thread t2(std::bind(threadFunc2, 42), false);
    t2.SetThreadName(t2.get_id(), "balabala");
    t2.start();

    printf("t2.tid=%d\n", t2.get_id());
    t2.join();

    Foo foo(87.53);
    Alime::Thread t3(std::bind(&Foo::memberFunc, &foo), false);
    t2.SetThreadName(t2.get_id(), "thread for member function without argument");
    t3.start();
    t3.join();

    Alime::Thread t4(std::bind(&Foo::memberFunc2, std::ref(foo), std::string("Shuo Chen")), false);
    t4.start();
    t4.join();

    {
        Alime::Thread t5(threadFunc3);
        // t5 may destruct eariler than thread creation.
    }
    Alime::this_thread::sleep_for(200);
    {
        Alime::Thread t6(threadFunc3, false);
        t6.start();
        Alime::this_thread::sleep_for(200);
        // t6 destruct later than thread creation.
    }
    Alime::this_thread::sleep_for(200);
    printf("number of created threads %d\n", Alime::Thread::numCreated());
}
