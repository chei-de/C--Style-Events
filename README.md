# C--Style-Events
C#-Styled Events for C++

## Lambdas 
```cpp
//No parameters
{
    //Create Event
    Event e;
    //Register
    auto handle = e += [] () { std::cout << "hi" << std::endl; };
    //Invoke
    e.Invoke();

    //Remove
    e -= handle;
}

//Variable parameters
{
    Event<int, float> e;
    auto handle = e += [] (int i, float f) { 
        std::cout << "I is: " << i << " f is: " << f << std::endl;
    };

    e.Invoke(42, 21.f);
}
```

## Functions

```cpp
//Events with free functions
void FreeFunc(int a, int& b) {
    b = a;
}

{
    Event<int, int&> evt;
    //Assign free function
    auto handle = evt += FreeFunc;

    int a = 5;
    int b = 3;

    evt.Invoke(a, b);
    ASSERT_EQ(a, b);
}

```

## Methods

```cpp
class CallableThing
{
    private:
        int a;

    public:
        CallableThing(int a) : a(a) {}
        void AddOffset(int & b) { b = b + a; }
};

{
    //Create Object
    CallableThing c1(2);

    Event<int&> evt1;
    //For bound function use register method of event
    evt1.Register<CallableThing>(&c1, &CallableThing::AddOffset);

    int i = 0;
    evt1.Invoke(i);
    ASSERT_EQ(i, 2);
}

```