class ZeroEvenOdd {
private:
    int n;
    pthread_mutex_t zeroMux = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t oddMux = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t evenMux = PTHREAD_MUTEX_INITIALIZER;

public:
    ZeroEvenOdd(int n) {
        this->n = n;
        
        // starter
        pthread_mutex_lock(&oddMux);
        pthread_mutex_lock(&evenMux);
    }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber) {
        for (int i = 0; i < n; ++i) {
            pthread_mutex_lock(&zeroMux);
            //printf("zero, zeroMux obtained: 0\n");
            printNumber(0);
            if ((i&1) == 1) {
                pthread_mutex_unlock(&evenMux);
            } else {
                pthread_mutex_unlock(&oddMux);
            }
        }   
    }

    void even(function<void(int)> printNumber) {
        for (int i = 2; i <= n; i+=2) {
            pthread_mutex_lock(&evenMux);
            //printf("even, evenMux obtained: %d\n", i);
            printNumber(i);
            pthread_mutex_unlock(&zeroMux);
        }
    }

    void odd(function<void(int)> printNumber) {
        for (int i = 1; i <= n; i+=2) {
            pthread_mutex_lock(&oddMux);
            //printf("odd, oddMux obtained: %d\n", i);
            printNumber(i);
            pthread_mutex_unlock(&zeroMux);
        }
    }
};
