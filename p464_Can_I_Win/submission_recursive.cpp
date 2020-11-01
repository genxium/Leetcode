int const MAXREMAINING = 300;
int const MAXNCHOOSABLEINTEGER = 20;
int const MAXCHOOSABLESTATE = (1 << 21); // 1+2+...+2^20 = (1-2^21)/(1-2) = (2^21 - 1)
unordered_map<int, unordered_map<int, int>> cache; // "cache[remaining][choosableState] == +1" means that whoever faces the StateTuple {remaining, choosableState} can win if played optimally, while "== 0" is undetermined

/*
test case#1
10
0

test case#2
5
50
*/
class Solution {
public:
    // The "StateTuple" is {remaining, choosableState}
    
    int solve(int remaining, int choosableState, int maxChoosableInteger) {
      //printf("solve remaining:%d, choosableState:%d\n", remaining, choosableState);  
      if (0 >= remaining) {
        return cache[remaining][choosableState] = -1;
      }
      //printf("\tneed calculation\n");  
      if (0 != cache[remaining][choosableState]) return cache[remaining][choosableState];
      int toRet = -1; // assume would lose
      for (int i = 1; i <= maxChoosableInteger; ++i) {
        int mask = (1 << i);
        if ((choosableState & mask) == 0) continue;
        int tmp = solve(remaining-i, (choosableState^mask), maxChoosableInteger);
        if (tmp == -1) {
          toRet = +1; // by taking i from remaining, can win
          break;
        }
      }
      return cache[remaining][choosableState] = toRet;
    }
  
    bool canIWin(int maxChoosableInteger, int desiredTotal) {
      if (0 == desiredTotal) {
        // special case
        return true;
      }
      int choosableTotal = 0;
      int choosableState = 0;
      for (int i = 1; i <= maxChoosableInteger; ++i) {
        choosableTotal += i;
        choosableState |= (1 << i);
      } 
      if (choosableTotal < desiredTotal) {
        // special case
        return false;
      }
      
      int result = solve(desiredTotal, choosableState, maxChoosableInteger); 
      return (result == +1);
    }
};
