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
    int operator()(MinStepRecord const &v1, MinStepRecord const &v2) 
    {
      return v1.i < v2.i;
    }
};

class Solution {
  public:
    int jump(vector<int>& nums) {
      int n = nums.size();
      if (1 == n) {
        return 0;
      }
      /*
       * Greedy to take the locally maximum jump doesn't work to achieve least steps.
       * See [2,3,1,1,4] as an example.
       */

      /*
       * The solution here is that we only increment "minSteps" at "nums[i]" which extends the "globalFurthestReachable",
       *
       * ```
       * 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
       * x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x
       * ^--------------|
       *    ^-----------------|
       * ```
       *
       * but not which doesn't.
       *
       * ```
       * 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
       * x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x
       * ^--------------|
       *    ^--------|
       * ```
       *
       * However, there's an edge case for "increment `minSteps` as long as `nums[i]` extends `globalFurthestReachable`", the "minSteps" to arrive at "nums[12]" should be 2.
       * ```
       * 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18
       * x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x  x
       * ^--------------|
       *    ^-----------------|
       *          ^-----------------------|
       * ```
       */
      vector<MinStepRecord> minSteps; // In ascending order w.r.t. "MinStepRecord.i". 
      // Init.
      MinStepRecord r; 
      r.i = nums[0]; r.s = 1; // Initially we can reach "nums[0]" as the furthest position in 1 step.
      minSteps.push_back(r);

      // Loop.
      for (int i = 0; i < n; ++i) {
        MinStepRecord globalFurthestRecord = minSteps.back(); 
        int globalFurthestReachable = globalFurthestRecord.i; 
        if (i > globalFurthestReachable) continue;

        int candidateFurthestReachable = (i + nums[i] < n ? i + nums[i] : n-1);
        if (candidateFurthestReachable > globalFurthestReachable) {
          MinStepRecord searchAssistiveR;
          searchAssistiveR.i = i; 
          vector<MinStepRecord>::iterator upper = upper_bound(minSteps.begin(), minSteps.end(), searchAssistiveR, MinStepRecordCmp());
          vector<MinStepRecord>::iterator lower = lower_bound(minSteps.begin(), minSteps.end(), searchAssistiveR, MinStepRecordCmp());

          // printf("for i == %d, searchAssistiveR: ", i);
          // searchAssistiveR.print();

          // printf("\t, upper: ");
          // (*upper).print();

          // printf("\t, lower: ");
          // (*lower).print();
          
          vector<MinStepRecord>::iterator pPrevRecord; 
          if ((*lower).i == i) {
            pPrevRecord = lower;
          } else {
            pPrevRecord = upper;
          }
          MinStepRecord candidateR;
          candidateR.i = candidateFurthestReachable; 
          candidateR.s = (*pPrevRecord).s + 1;
          // printf("globalFurthestReachable incremented from %d to %d, jumped under the reach of position %d, now candidateR.s == %d\n", globalFurthestReachable, candidateFurthestReachable, (*pPrevRecord).i, candidateR.s);
          minSteps.push_back(candidateR);
        }
      }
      MinStepRecord globalFurthestRecord = minSteps.back(); 
      return globalFurthestRecord.s;
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
