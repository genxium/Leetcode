class Foo {
public:
    atomic<int> counter;
    
    Foo() {
        counter = 1;
    }
    
    void blocksOnCounter(int const expectedOld) {
        // This function only serves to spinlock the current KernelThread w.r.t. certain value of "counter", it's NOT TRUE that the whole function being atomic.
        int localContainer = expectedOld;
        while (true) {
            counter.compare_exchange_weak(localContainer, expectedOld+1);
            if (localContainer == expectedOld) break;
            localContainer = expectedOld;
        }
    }

    void first(function<void()> printFirst) {
        int expectedCount = 1;
        counter.compare_exchange_strong(expectedCount, 2);
        //printf("first, counter is now:%d\n", counter.load());
        
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        expectedCount = 2;
        counter.compare_exchange_strong(expectedCount, 3);
    }

    void second(function<void()> printSecond) {
        int expectedCount = 3;
        blocksOnCounter(3);
        //printf("second, counter is now:%d\n", counter.load());

        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();
        expectedCount = 4;
        counter.compare_exchange_strong(expectedCount, 5);
    }

    void third(function<void()> printThird) {
        int expectedCount = 5;
        blocksOnCounter(5);
        //printf("third, counter is now:%d\n", counter.load());

        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }
};
