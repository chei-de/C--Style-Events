#pragma once

#include <gtest/gtest.h>
#include <tuple>
#include "Event.hpp"

//Events with lambdas
TEST(EventTest, EventLambdaWorking)
{
    bool works = false;
    Event evt;
    evt += [&works] () {works = true;};

    ASSERT_FALSE(works);
    evt.Invoke();
    ASSERT_TRUE(works);
}

TEST(EventTest, EventLambdaParameterWorking)
{
    bool works = false;
    ASSERT_FALSE(works);

    Event<bool> evt;
    evt += [&works](bool status) {
        works = status;
    };

    evt.Invoke(true);
    ASSERT_TRUE(works);

    evt.Invoke(true);
    ASSERT_TRUE(works);

    evt.Invoke(false);
    ASSERT_FALSE(works);
}

//Events with free functions
void EventTestingFunc(int a, int& b) {
    b = a;
}

TEST(EventTest, EventFreeFuncWorking)
{
    Event<int, int&> evt;
    evt += EventTestingFunc;

    int a = 5;
    int b = 3;

    ASSERT_NE(a, b);

    evt.Invoke(a, b);

    ASSERT_EQ(a, b);
}

//Events with members
class CallableThing
{
    private:
        int a;

    public:
        CallableThing(int a) : a(a) {}
        void AddOffset(int & b) { b = b + a; }
};


TEST(EventTest, EventMemberFunctionWorking)
{
    CallableThing c1(2);
    CallableThing c2(5);

    Event<int&> evt1;
    evt1.Register<CallableThing>(&c1, &CallableThing::AddOffset);

    int i = 0;
    evt1.Invoke(i);
    ASSERT_EQ(i, 2);


    evt1.Register(&c2, &CallableThing::AddOffset);
    evt1.Invoke(i); // 2 + 7...

    ASSERT_EQ(i, 9);
}

TEST(EventTest, EventMemberFunctionWorking2)
{
    CallableThing c1(2);
    CallableThing c2(5);

    auto pair = std::make_pair(&c1, &CallableThing::AddOffset);
    Event<int&> evt1;
    evt1 += std::make_tuple(&c1, &CallableThing::AddOffset);

    int i = 0;
    evt1.Invoke(i);
    ASSERT_EQ(i, 2);


    evt1 += std::make_tuple(&c2, &CallableThing::AddOffset);
    evt1.Invoke(i); // 2 + 7...

    ASSERT_EQ(i, 9);
}

//Utilitytests
TEST(EventTest, EventAddRemoveWorking)
{
    //Create TestFunc
    auto lbd = [] (int& f, int i) { f += i; };

    //Register 4 funcs
    Event<int&, int> evt1;
    auto t1 = evt1 += lbd;
    auto t2 = evt1 += lbd;
    auto t3 = evt1 += lbd;
    auto t4 = evt1 += lbd;

    int counter = 0;
    int increment = 1;

    //Invoke the func, counter should be incremented 4 times
    evt1.Invoke(counter, 1);
    ASSERT_EQ(counter, 4);


    //Unregister two, counter should be incremented 2 times
    evt1.Unregister(t1);
    evt1 -= t2;
    evt1.Invoke(counter, 1);
    ASSERT_EQ(counter, 6);

    //Reregister two, counter should be incremented 4 times
    t1 = evt1 += lbd;
    t2 = evt1 += lbd;
    evt1.Invoke(counter, 1);
    ASSERT_EQ(counter, 10);

    //Unregister all, counter should not be incremented
    evt1 -= t1;
    evt1 -= t2;
    evt1.Unregister(t3);
    evt1.Unregister(t4);
    evt1.Invoke(counter, 1);
    ASSERT_EQ(counter, 10);
}
