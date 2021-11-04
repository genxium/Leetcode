/*
Note that "pthread_cond_wait(&certainCond, &certainMux)" must be called prior to the corresponding "pthread_cond_signal(&certainCond)" to receive the signal, otherwise it would wait forever.
*/

class Foo {
public:
    int counter = 0; // both reading & writing guarded by "sharedMux"
    bool secondWaitingOrCalled = false, thirdWaitingOrCalled = false; // both reading & writing guarded by "sharedMux" guarded by "sharedMux" too
    pthread_mutex_t sharedMux = PTHREAD_MUTEX_INITIALIZER;
    
    pthread_cond_t secondCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t thirdCond = PTHREAD_COND_INITIALIZER;
    
    Foo() {

    }

    void first(function<void()> printFirst) {
        pthread_mutex_lock(&sharedMux);
        
        printFirst();    
        ++counter;
        
        pthread_mutex_unlock(&sharedMux);

        // If "second()" were called must have released the held "sharedMux" and thus entered "waiting for secondCond" state.
        pthread_cond_signal(&secondCond);
    }

    void second(function<void()> printSecond) {
        pthread_mutex_lock(&sharedMux);
        while (1 > counter) {
            pthread_cond_wait(&secondCond, &sharedMux);
        }
        
        printSecond();    
        ++counter;
        
        pthread_mutex_unlock(&sharedMux);   
        
        // If "third()" were called earlier, it'd have released the held "sharedMux" and thus entered "waiting for thirdCond" state.
        pthread_cond_signal(&thirdCond);
    }

    void third(function<void()> printThird) {
        pthread_mutex_lock(&sharedMux);
        while (2 > counter) {
            pthread_cond_wait(&thirdCond, &sharedMux);
        }
        
        printThird();
        ++counter;
        
        pthread_mutex_unlock(&sharedMux);   
    }
};
