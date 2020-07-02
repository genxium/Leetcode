/*
Note that "pthread_cond_wait(&certainCond, &certainMux)" must be called prior to the corresponding "pthread_cond_signal(&certainCond)" to receive the signal, otherwise it would wait forever.
*/
class FooBar {
private:
    int n, counter, counterUpper;
    pthread_mutex_t sharedMux = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t fooCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t barCond = PTHREAD_COND_INITIALIZER;
    
public:
    FooBar(int n) {
        this->n = n;
        this->counter = 0;
        this->counterUpper = (n << 1);
    }

    void foo(function<void()> printFoo) {
        bool sharedMuxObtained = false;
        while (true) {
            if (false == sharedMuxObtained) {
                pthread_mutex_lock(&sharedMux);
            }
            sharedMuxObtained = true;
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                
                // just to help it break as well
                pthread_cond_signal(&barCond);
                break;
            }
            if ((counter&1) == 0) {
                //printf("counter == %d, foo\n", counter);
                printFoo();
                ++counter;
                pthread_cond_signal(&barCond); // Is the corresponding "pthread_cond_wait(&barCond, &sharedMux)" called by now?
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
            } else {
                //printf("counter == %d, foo put into cond_wait\n", counter);
                pthread_cond_wait(&fooCond, &sharedMux);
                //printf("counter == %d, foo re-obtained sharedMux\n", counter);
            }
        }
    }

    void bar(function<void()> printBar) {
        bool sharedMuxObtained = false;
        while (true) {
            if (false == sharedMuxObtained) {
                pthread_mutex_lock(&sharedMux);
            }
            sharedMuxObtained = true;
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                
                // just to help it break as well
                pthread_cond_signal(&fooCond);
                break;
            }
            if ((counter&1) == 1) {
                //printf("counter == %d, bar\n", counter);
                printBar();
                ++counter;
                pthread_cond_signal(&fooCond); // Is the corresponding "pthread_cond_wait(&fooCond, &sharedMux)" called by now?
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
            } else {
                //printf("counter == %d, bar put into cond_wait\n", counter);
                pthread_cond_wait(&barCond, &sharedMux);
                //printf("counter == %d, bar re-obtained sharedMux\n", counter);
            }
        }
    }
};

