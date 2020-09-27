#define MOST_SIG (1 << 7)
#define SECOND_MOST_SIG (1 << 6)

class Solution {
public:
    bool startsNOnesAndFollowingZero(int &byte, int n) {
      int lower = 7-n;
      //printf("startsNOnesAndFollowingZero, byte:%d, n:%d, lower:%d\n", byte, n, lower);
      
      for (int i = 7; i > lower; --i) {
        if ((byte & (1 << i)) == 0) {
          return false;
        }
      }
      if ((byte & (1 << lower)) > 0) {
        return false;
      }
      return true;
    }
  
    bool startsWithZero(int &byte) {
      return (
        (MOST_SIG & byte) == 0
      );
    }
  
    bool startsWithOneZero(int &byte) {
      return (
        (MOST_SIG & byte) > 0   
        && 
        (SECOND_MOST_SIG & byte) == 0
      );
    }
      
    void validate(int idx, vector<int> &data, bool &ans) {
      if (idx == data.size()) {
        ans = true;          
        return;
      }
      
      if (ans) {
        return;
      }
      
      if (startsWithZero(data[idx])) {
        //printf("%d leads a valid 1-byte symbol\n", data[idx]);
        validate(idx+1, data, ans);
      }
      
      /*
      TODO
      
      How many leading '1's there're in "data[idx]" is in fact deterministic, use this clue to speed up.
      */
      for (int j = 2; j <= 4; ++j) {
        bool canProceed = true;
        if (!startsNOnesAndFollowingZero(data[idx], j)) {
          //printf("%d doesn't lead a valid %d-byte symbol because it doesn't start with %d 1s and a following 0\n", data[idx], j, j);
          canProceed = false;
          continue;
        }
        for (int k = idx+1; k <= idx+j-1; ++k) {
          if (k >= data.size()) {
            canProceed = false;
            break;
          }
          if (!startsWithOneZero(data[k])) {
            //printf("%d doesn't lead a valid %d-byte symbol because data[k:%d]:%d doesn't start with 10\n", data[idx], j, k, data[k]);
            canProceed = false;
            break;
          }
        }
        if (!canProceed) {
          continue;
        }
        //printf("%d leads a valid %d-byte symbol\n", data[idx], j);
        validate(idx+j, data, ans);
      }
    }
  
    bool validUtf8(vector<int>& data) {
      bool ans = false;
      validate(0, data, ans);
      return ans;
    }
};
