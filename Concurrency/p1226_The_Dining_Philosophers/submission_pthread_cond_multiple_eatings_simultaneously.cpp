#define INVALID -1
#define MAXN 5

class DiningPhilosophers {
private:
  pthread_mutex_t sharedMux = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t philosopherEatCond[MAXN];
  bool forksAvailable[MAXN];  
  
  pair<int, int> getForkIdPair(int byPhilosopherId) {
    // Hereby assume that the "left fork of 0-th philosopher is indexed 0", then increases in clockwise manner.
    switch (byPhilosopherId) {
      case 0:
        return {0, 4};
      case 1:
        return {1, 0};
      case 2:
        return {2, 1};
      case 3:
        return {3, 2};
      case 4:
        return {4, 3};
      default:
        return {};
    }
  } 
    
public:
  DiningPhilosophers() {
    memset(forksAvailable, true, sizeof(forksAvailable));    
    for (int i = 0; i < MAXN; ++i) {
      philosopherEatCond[i] = PTHREAD_COND_INITIALIZER;
    }
  }

  void wantsToEat(int philosopher,
                  function<void()> pickLeftFork,
                  function<void()> pickRightFork,
                  function<void()> eat,
                  function<void()> putLeftFork,
                  function<void()> putRightFork) {
    auto forkPair = getForkIdPair(philosopher);
    int leftForkId = forkPair.first;
    int rightForkId = forkPair.second;
    
    int leftPhilosopherId = (philosopher+1)%MAXN;
    int rightPhilosopherId = (philosopher-1+MAXN)%MAXN;
    
    pthread_mutex_lock(&sharedMux);
    while(false == forksAvailable[leftForkId] || false == forksAvailable[rightForkId]) {
      pthread_cond_wait(&(philosopherEatCond[philosopher]), &sharedMux);
      // by now it's possible that another philosopher has picked up 1 of the forks needed here, we must re-check while the current thread is still holding "sharedMux"
    }
    
    // by now the current thread is holding "sharedMux" and "forksAvailable[leftForkId] && forksAvailable[rightForkId]"
    pickLeftFork();
    forksAvailable[leftForkId] = false;
    pickRightFork();
    forksAvailable[rightForkId] = false;
    pthread_mutex_unlock(&sharedMux); // allows others to acquire "sharedMux", thus multiple eatings simultaneously

    eat();
    
    pthread_mutex_lock(&sharedMux);
    putLeftFork();
    forksAvailable[leftForkId] = true;
    putRightFork();
    forksAvailable[rightForkId] = true;
    
    pthread_cond_signal(&(philosopherEatCond[leftPhilosopherId])); // doesn't guarantee that his/her required forks are BOTH available after returning from the respective "pthread_cond_wait(...)"
    pthread_cond_signal(&(philosopherEatCond[rightPhilosopherId])); // doesn't guarantee that his/her required forks are BOTH available after returning from the respective "pthread_cond_wait(...)"
    pthread_mutex_unlock(&sharedMux);
  }
};
