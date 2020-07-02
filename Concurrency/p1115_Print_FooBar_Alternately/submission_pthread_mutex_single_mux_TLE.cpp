class FooBar {
private:
    int n, counter, counterUpper;
    pthread_mutex_t sharedMux = PTHREAD_MUTEX_INITIALIZER;
    /*
    * This solution is slow because it's basically spinning, although with a "sharedMux".
    */
    
public:
    FooBar(int n) {
        this->n = n;
        this->counter = 0;
        this->counterUpper = (n << 1);
    }

    void foo(function<void()> printFoo) {
        while (true) {
            pthread_mutex_lock(&sharedMux);
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                break;
            }
            if ((counter&1) == 0) {
                //printf("counter == %d, foo\n", counter);
                printFoo();
                ++counter;
            } 
            pthread_mutex_unlock(&sharedMux);
        }
    }

    void bar(function<void()> printBar) {
        while (true) {
            pthread_mutex_lock(&sharedMux);
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                break;
            }
            if ((counter&1) == 1) {
                //printf("counter == %d, bar\n", counter);
                printBar();
                ++counter;
            } 
            pthread_mutex_unlock(&sharedMux);
        }
    }
};
