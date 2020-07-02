/*
Note that "pthread_cond_wait(&certainCond, &certainMux)" must be called prior to the corresponding "pthread_cond_signal(&certainCond)" to receive the signal, otherwise it would wait forever.
*/
class ZeroEvenOdd {
private:
    int n, counter, counterUpper;
    
    pthread_mutex_t sharedMux = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t zeroCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t oddCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t evenCond = PTHREAD_COND_INITIALIZER;
    
public:
    ZeroEvenOdd(int n) {
        this->n = n;
        this->counter = 0;
        this->counterUpper = (n << 1);
    }

    // printNumber(x) outputs "x", where x is an integer.
    void zero(function<void(int)> printNumber) {
        bool sharedMuxObtained = false;
        while (true) {
            if (false == sharedMuxObtained) {
                pthread_mutex_lock(&sharedMux);    
            }
            sharedMuxObtained = true;
            //printf("zero, obtained sharedMux at counter == %d\n", counter);
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                
                // the order of signaling doesn't matter here, just to help them break as well
                pthread_cond_signal(&oddCond);
                pthread_cond_signal(&evenCond);
                //printf("zero, released sharedMux and breaking at counter == %d\n", counter);
                break;
            }
            if ((counter & 1) == 0) {
                //printf("zero, at counter == %d\n", counter);
                printNumber(0);
                ++counter;
                int halfCount = (counter >> 1);
                if ((halfCount & 1) == 0) {
                    pthread_cond_signal(&oddCond);
                } else {
                    pthread_cond_signal(&evenCond);
                }
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                //printf("zero, released sharedMux at counter == %d\n", counter);
            } else {
                //printf("zero, put into cond_wait at counter == %d\n", counter);
                pthread_cond_wait(&zeroCond, &sharedMux);
                //printf("zero, re-obtained sharedMux at counter == %d\n", counter);
            }
        }   
    }

    void even(function<void(int)> printNumber) {
        bool sharedMuxObtained = false;
        while (true) {
            if (false == sharedMuxObtained) {
                pthread_mutex_lock(&sharedMux);    
            }
            sharedMuxObtained = true;
            //printf("even, obtained sharedMux at counter == %d\n", counter);
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                
                // the order of signaling doesn't matter here, just to help them break as well
                pthread_cond_signal(&oddCond);
                pthread_cond_signal(&zeroCond);
                //printf("even, released sharedMux and breaking at counter == %d\n", counter);
                break;
            }
            int halfCount = (counter >> 1);
            if (counter > 0 && (counter & 1) == 1 && (halfCount & 1) == 1) {
                //printf("even, at counter == %d\n", counter);
                printNumber(halfCount+1);
                ++counter;
                pthread_cond_signal(&zeroCond);
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                //printf("even, released sharedMux at counter == %d\n", counter);
            } else {
                //printf("even, put into cond_wait at counter == %d\n", counter);
                pthread_cond_wait(&evenCond, &sharedMux);
                //printf("even, re-obtained sharedMux at counter == %d\n", counter);
            }
        } 
    }

    void odd(function<void(int)> printNumber) {
        bool sharedMuxObtained = false;
        while (true) {
            if (false == sharedMuxObtained) {
                pthread_mutex_lock(&sharedMux);    
            }
            sharedMuxObtained = true;
            //printf("odd, obtained sharedMux at counter == %d\n", counter);
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                
                // the order of signaling doesn't matter here, just to help them break as well
                pthread_cond_signal(&zeroCond);
                pthread_cond_signal(&evenCond);
                //printf("odd, released sharedMux and breaking at counter == %d\n", counter);
                break;
            }
            int halfCount = (counter >> 1);
            if (counter > 0 && (counter & 1) == 1 && (halfCount & 1) == 0) {
                //printf("odd, at counter == %d\n", counter);
                printNumber(halfCount+1);
                ++counter;
                pthread_cond_signal(&zeroCond);
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                //printf("odd, released sharedMux at counter == %d\n", counter);
            } else {
                //printf("odd, put into cond_wait at counter == %d\n", counter);
                pthread_cond_wait(&oddCond, &sharedMux);
                //printf("odd, re-obtained sharedMux at counter == %d\n", counter);
            }
        } 
    }
};
