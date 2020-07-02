/*
Note that "pthread_cond_wait(&certainCond, &certainMux)" must be called prior to the corresponding "pthread_cond_signal(&certainCond)" to receive the signal, otherwise it would wait forever.
*/
class FizzBuzz {
private:
    int n;
    int counter, counterUpper;
    pthread_mutex_t sharedMux = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t numCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t fizzCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t buzzCond = PTHREAD_COND_INITIALIZER;
    pthread_cond_t fizzbuzzCond = PTHREAD_COND_INITIALIZER;
    
public:
    FizzBuzz(int n) {
        this->n = n;
        this->counter = 1;
        this->counterUpper = n+1;
    }

    // printFizz() outputs "fizz".
    void fizz(function<void()> printFizz) {
        bool sharedMuxObtained = false;
        while (true) {
            if (false == sharedMuxObtained) {
                pthread_mutex_lock(&sharedMux);
            }
            sharedMuxObtained = true;
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                
                // just signal every one else, each would respond or wait again or break
                pthread_cond_signal(&numCond);
                pthread_cond_signal(&buzzCond);
                pthread_cond_signal(&fizzbuzzCond);
                break;
            }
            
            if ((counter % 3) == 0 && (counter % 5) != 0) {
                printFizz();
                ++counter;
                
                // just signal every one else, each would respond or wait again or break
                pthread_cond_signal(&numCond);
                pthread_cond_signal(&buzzCond);
                pthread_cond_signal(&fizzbuzzCond);
                
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
            } else {
                pthread_cond_wait(&fizzCond, &sharedMux);
            }
        }
    }

    // printBuzz() outputs "buzz".
    void buzz(function<void()> printBuzz) {
        bool sharedMuxObtained = false;
        while (true) {
            if (false == sharedMuxObtained) {
                pthread_mutex_lock(&sharedMux);
            }
            sharedMuxObtained = true;
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                
                // just signal every one else, each would respond or wait again or break
                pthread_cond_signal(&numCond);
                pthread_cond_signal(&fizzCond);
                pthread_cond_signal(&fizzbuzzCond);
                break;
            }
            
            if ((counter % 3) != 0 && (counter % 5) == 0) {
                printBuzz();
                ++counter;
                
                // just signal every one else, each would respond or wait again or break
                pthread_cond_signal(&numCond);
                pthread_cond_signal(&fizzCond);
                pthread_cond_signal(&fizzbuzzCond);
                
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
            } else {
                pthread_cond_wait(&buzzCond, &sharedMux);
            }
        }
    }

    // printFizzBuzz() outputs "fizzbuzz".
	void fizzbuzz(function<void()> printFizzBuzz) {
        bool sharedMuxObtained = false;
        while (true) {
            if (false == sharedMuxObtained) {
                pthread_mutex_lock(&sharedMux);
            }
            sharedMuxObtained = true;
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                
                // just signal every one else, each would respond or wait again or break
                pthread_cond_signal(&numCond);
                pthread_cond_signal(&buzzCond);
                pthread_cond_signal(&fizzCond);
                break;
            }
            
            if ((counter % 3) == 0 && (counter % 5) == 0) {
                printFizzBuzz();
                ++counter;
                
                // just signal every one else, each would respond or wait again or break
                pthread_cond_signal(&numCond);
                pthread_cond_signal(&buzzCond);
                pthread_cond_signal(&fizzCond);
                
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
            } else {
                pthread_cond_wait(&fizzbuzzCond, &sharedMux);
            }
        }
    }

    // printNumber(x) outputs "x", where x is an integer.
    void number(function<void(int)> printNumber) {
        bool sharedMuxObtained = false;
        while (true) {
            if (false == sharedMuxObtained) {
                pthread_mutex_lock(&sharedMux);
            }
            sharedMuxObtained = true;
            if (counter >= counterUpper) {
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
                
                // just signal every one else, each would respond or wait again or break
                pthread_cond_signal(&fizzCond);
                pthread_cond_signal(&buzzCond);
                pthread_cond_signal(&fizzbuzzCond);
                break;
            }
            
            if ((counter % 3) != 0 && (counter % 5) != 0) {
                printNumber(counter);
                ++counter;
                
                // just signal every one else, each would respond or wait again or break
                pthread_cond_signal(&fizzCond);
                pthread_cond_signal(&buzzCond);
                pthread_cond_signal(&fizzbuzzCond);
                
                pthread_mutex_unlock(&sharedMux);
                sharedMuxObtained = false;
            } else {
                pthread_cond_wait(&numCond, &sharedMux);
            }
        }
    }
};
