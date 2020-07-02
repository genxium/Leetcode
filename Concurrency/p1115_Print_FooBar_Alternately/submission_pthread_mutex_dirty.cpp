class FooBar {
private:
    int n;
    pthread_mutex_t fooMux = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t barMux = PTHREAD_MUTEX_INITIALIZER;
    
    /*
    This solution is dirty because it uses "thread#b to unlock a mutex held by thread#a", which generally results in "undefined behaviour" for most "pthread_mutex_t types".
    
    Moreover, if we use "pthread_mutex_t type `PTHREAD_MUTEX_ERRORCHECK`", this dirty solution will fail.
    
    // pthread_mutex_t fooMux = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
    // pthread_mutex_t barMux = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
    */

public:
    FooBar(int n) {
        this->n = n;
        
        // starter 
        pthread_mutex_lock(&barMux);
    }

    void foo(function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            pthread_mutex_lock(&fooMux);
        	// printFoo() outputs "foo". Do not change or remove this line.
            printFoo();
            pthread_mutex_unlock(&barMux); // The problem description says that "the same instance of `FooBar` will be passed to two different threads".
        }
    }

    void bar(function<void()> printBar) {
        for (int i = 0; i < n; i++) {
            pthread_mutex_lock(&barMux);
        	// printBar() outputs "bar". Do not change or remove this line.
            printBar();
            pthread_mutex_unlock(&fooMux);
        }
    }
};
