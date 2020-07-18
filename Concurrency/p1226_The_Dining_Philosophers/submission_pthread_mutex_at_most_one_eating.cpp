#define INVALID -1
#define MAXN 5

class DiningPhilosophers {
private:
  pthread_mutex_t sharedMux = PTHREAD_MUTEX_INITIALIZER;
  
public:
  DiningPhilosophers() {
  }

  void wantsToEat(int philosopher,
                  function<void()> pickLeftFork,
                  function<void()> pickRightFork,
                  function<void()> eat,
                  function<void()> putLeftFork,
                  function<void()> putRightFork) {
    pthread_mutex_lock(&sharedMux);
    pickLeftFork();
    pickRightFork();
    eat();
    putLeftFork();
    putRightFork();
    pthread_mutex_unlock(&sharedMux);
  }
};
