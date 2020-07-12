class H2O {
private:
    int expectingH = 2;
    int expectingO = 1;
    int waitingH = 0;
    int waitingO = 0;
    pthread_mutex_t sharedMux = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t allowingH = PTHREAD_COND_INITIALIZER;
    pthread_cond_t allowingO = PTHREAD_COND_INITIALIZER;
    
    void resetIfApplicable() {
        if (0 < expectingH || 0 < expectingO) return;
        this->expectingH = 2;
        this->expectingO = 1;
    }

    void awakeIfApplicable() {
        if (0 < waitingH) {
            pthread_cond_signal(&allowingH);
        }
        if (0 < waitingO) {
            pthread_cond_signal(& allowingO);
        }
    }
    
public:
    H2O() {

    }

    void hydrogen(function<void()> releaseHydrogen) {
        pthread_mutex_lock(&sharedMux);
        while (0 == expectingH && 0 < expectingO) {
            // Using a while-loop here to avoid "spurious awaking".
            ++waitingH;
            pthread_cond_wait(&allowingH, &sharedMux);
            --waitingH;
        }
        --expectingH;
        // releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen();
        
        resetIfApplicable();  
        awakeIfApplicable();
        pthread_mutex_unlock(&sharedMux);
        
    }

    void oxygen(function<void()> releaseOxygen) {
        pthread_mutex_lock(&sharedMux);
        while (0 == expectingO && 0 < expectingH) {
            // Using a while-loop here to avoid "spurious awaking".
            ++waitingO;
            pthread_cond_wait(&allowingO, &sharedMux);
            --waitingO;
        }
        --expectingO;
        // releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen();
        resetIfApplicable();
        awakeIfApplicable();
        pthread_mutex_unlock(&sharedMux);
    }
};
