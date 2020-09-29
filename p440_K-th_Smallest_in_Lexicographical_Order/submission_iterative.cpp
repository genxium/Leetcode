#define ULL uint64_t
/*
test case#1
2332
10

test case#2
2332
2001
This case will trigger condition "if (rightBrother > n)" of "stepsBetween(...)".

test case #3
681692778
351251360
*/
class Solution {
public:
    int stepsBetween(ULL n, ULL leftBrother, ULL rightBrother, int starterLevel) {
      int currentLevel = starterLevel;
      // w.r.t. preorder traversal in the "Trie"
      int res = 0;
      while (leftBrother <= n) {
        if (rightBrother > n) {
          //printf("\tstepsBetween, n: %d, leftBrother: %d, rightBrother: %d, currentLevel: %d\n", n, leftBrother, rightBrother, currentLevel);
          res += (n - leftBrother + 1); // the "+1" comes from calibration by the time of writing
          break; // there'd be no need to move down
        }
        
        // rightBrother <= n
        res += (rightBrother - leftBrother); // counts "GapNode"s at the same "currentLevel", e.g. for "leftBrother: 10, rightBrother: 20", the "GapNode"s are "11, 12, ..., 19"
        // move down
        leftBrother *= 10;
        rightBrother *= 10;
        ++currentLevel;
      }
      return res;
    }
  
    int findKthNumber(int n, int k) {
      int cur = 1, level = 1;
      while (k > 1) {
        //printf("n: %d, cur: %d, cur+1: %d, k(remaining): %d\n", n, cur, cur+1, k);
        int steps = stepsBetween((ULL)n, (ULL)cur, (ULL)cur+1, level);
        //printf("\tsteps: %d\n", steps);

        if (steps < k) {
          k -= steps;
          cur += 1; // move right 
        } else {
          // steps >= k
          k -= 1; // consumed 1 step w.r.t. pre-order traversal by moving down 
          cur *= 10; // move down 
          ++level; // in fact we don't need this "level" param, but it helps tracing the code execution and thus better understanding of edge cases
        }
      }
      return cur;
    }
};
