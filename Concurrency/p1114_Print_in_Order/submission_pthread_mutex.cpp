class Foo {
public:
    pthread_mutex_t condMux2 = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t condMux3 = PTHREAD_MUTEX_INITIALIZER;
    
    Foo() {
        pthread_mutex_init(&condMux2, NULL);
        pthread_mutex_init(&condMux3, NULL);
        
        pthread_mutex_lock(&condMux2);
        pthread_mutex_lock(&condMux3);
    }

    void first(function<void()> printFirst) {
        // printFirst() outputs "first". Do not change or remove this line.
        printFirst();
        pthread_mutex_unlock(&condMux2);
    }

    void second(function<void()> printSecond) {
        pthread_mutex_lock(&condMux2);

        // printSecond() outputs "second". Do not change or remove this line.
        printSecond();
        
        pthread_mutex_unlock(&condMux3);
    }

    void third(function<void()> printThird) {
        pthread_mutex_lock(&condMux3);
        
        // printThird() outputs "third". Do not change or remove this line.
        printThird();
    }
};
