#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;

class MinStepRecord {
public:
  // In the definition of this solution, a "MinStepRecord" means that "the least steps to reach i is s".
  int i, s;
  void print() {
    printf("i == %d, s == %d\n", this->i, this->s);
  }
};

class MinStepRecordCmp
{
public:
    int operator()(MinStepRecord const *left, MinStepRecord const *right) {
      return (left->i) < (right->i);
    }
    int operator()(MinStepRecord const *left, int const &right) {
      return (left->i) < right;
    }
    int operator()(int const &left, MinStepRecord const *right) {
      return left < (right->i);
    }
};

class Solution {
  public:
    int jump(vector<int>& nums) {
      int n = nums.size();
      if (1 == n) {
        return 0;
      }
      vector<MinStepRecord*> minSteps(n, NULL); // In ascending order w.r.t. "MinStepRecord.i". 

      /*
       * Greedy to take the locally maximum jump doesn't work to achieve least steps.
       * See [2,3,1,1,4] as an example.
       */

      /*
       * The solution here is that we only increment "minSteps" at "nums[i]" which extends the "globalFurthestReachable",
       *
       * ```
       * [case#1]
       *
       * 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
       * x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x
       * ^--------------|
       *    ^-----------------|
       * ```
       *
       * but not which doesn't.
       *
       * ```
       * [case#2]
       *
       * 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
       * x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x
       * ^--------------|
       *    ^--------|
       * ```
       *
       * However, there's an edge case for "increment `minSteps` as long as `nums[i]` extends `globalFurthestReachable`", the "minSteps" to arrive at "nums[12]" should be 2.
       * ```
       * [case#3]
       *
       * 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
       * x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x
       * ^--------------|
       *    ^-----------------|
       *          ^-----------------------|
       * ```
       */
      // Init.
      MinStepRecord* r = new MinStepRecord(); 
      r->i = nums[0]; r->s = 1; // Initially we can reach "nums[0]" as the furthest position in 1 step.
      minSteps[0] = r;
      int minStepsLastIndex = 0;

      // Loop.
      for (int i = 0; i < n; ++i) {
        MinStepRecord* globalFurthestRecord = minSteps[minStepsLastIndex]; 
        int globalFurthestReachable = globalFurthestRecord->i; 
        if (i > globalFurthestReachable) {
          continue;
        }

        int candidateFurthestReachable = (i + nums[i] < n ? i + nums[i] : n-1);
        if (candidateFurthestReachable > globalFurthestReachable) {
          vector<MinStepRecord*>::iterator upper = upper_bound(minSteps.begin(), minSteps.begin()+minStepsLastIndex+1, i, MinStepRecordCmp());
          vector<MinStepRecord*>::iterator lower = lower_bound(minSteps.begin(), minSteps.begin()+minStepsLastIndex+1, i, MinStepRecordCmp());

          // printf("for i == %d:\n", i);
          // printf("\tupper: ");
          // (*upper)->print();
          // printf("\tlower: ");
          // (*lower)->print();
          
          vector<MinStepRecord*>::iterator pPrevRecord; 
          if ((*lower)->i == i) {
            pPrevRecord = lower;
          } else {
            pPrevRecord = upper;
          }
          int newI = candidateFurthestReachable;
          int newS = (*pPrevRecord)->s + 1;
        
          // printf("globalFurthestReachable incremented from %d to %d, jumped under the reach of position %d, now candidateR.s == %d\n", globalFurthestReachable, candidateFurthestReachable, (*pPrevRecord).i, candidateR.s);

          /*
           * Why not find a "`pPrevRecord` with larger `(*pPrevRecord).i`" for each "i"? 
           *
           * Check [case#3] above, and assume that we're at "nums[3]" with "minSteps == [MinStepRecord(i: 5, s: 1), MinStepRecord(i: 7, s: 2)]". Note that we reach this state of "minSteps" due to the deliberate rejection of [case#2], thus the "nums[3]" should take "minSteps" to a new state "minSteps == [MinStepRecord(i: 5, s: 1), MinStepRecord(i: 7, s: 2), MinStepRecord(i: 11, s: 2)]".    
           *
           * To reduce search complexity in future iterations, we can merge all "MinStepRecord"s with the same "MinStepRecord.s" value to the largest "MinStepRecord.i", e.g. in this example after "nums[3]" should result in "minSteps == [MinStepRecord(i: 5, s: 1), MinStepRecord(i: 11, s: 2)]".
           */
          if (globalFurthestRecord->s == newS) {
            minSteps[minStepsLastIndex]->i = newI;
          } else {
            MinStepRecord* newR = new MinStepRecord();
            newR->i = newI;
            newR->s = newS;
            minSteps[++minStepsLastIndex] = newR;
          }
        }
      }
      MinStepRecord const *globalFurthestRecord = minSteps[minStepsLastIndex]; 
      int toRet = globalFurthestRecord->s;
      for (int i = 0; i <= minStepsLastIndex; ++i) {
          delete minSteps[i];
      }
      return toRet;
    }
};

int main() {
  int inputArr[] = {7,0,9,6,9,6,1,7,9,0,1,2,9,0,3};
  vector<int> input(inputArr, inputArr+sizeof(inputArr)/sizeof(int));
  Solution s;
  int res = s.jump(input);
  printf("%d\n", res);
  return 0;
}
