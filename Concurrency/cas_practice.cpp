#include <stdio.h>
#include <atomic>
using namespace std;

atomic<int> counter;

void blocksOnCounter(int const expectedOld) {
  // This function only serves to spinlock the current KernelThread w.r.t. certain value of "counter", it's NOT TRUE that the whole function being atomic.
  int localContainer = expectedOld;
  while (true) {
    // counter.compare_exchange_weak(localContainer, expectedOld+1);
    counter.compare_exchange_strong(localContainer, expectedOld+1);
    int afterCompareExchangeCounterVal = counter.load();
    printf("afterCompareExchangeCounterVal == %d\n", afterCompareExchangeCounterVal);
    if (localContainer == expectedOld) {
      // Although this whole function is not guaranteed atomic, both "localContainer" and "expectedOld" are local to this function (thus local to the current KernelThread), making this checkvalve effective, i.e. only passable if another KernelThread sets "counter" to value "expectedOld".
      break;
    }
    localContainer = expectedOld;
  }
}

int main() {
  /*
  g++ --std=c++11 -o cas_practice cas_practice.cpp
  */
  counter = 0;
  //int expectedOld = 0; // won't block and will only print "afterCompareExchangeCounterVal == 1" once
  int expectedOld = 99; // will block and keep printing "afterCompareExchangeCounterVal == 0"
  blocksOnCounter(expectedOld);
  return 0;
}
