#define MOST_SIG (1 << 7)
#define SECOND_MOST_SIG (1 << 6)

class Solution {
public:
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
      
      int countLeadingOnes = 0;
      int j = 8;
      while (j--) {
        int mask = (1 << j);
        if (
          (data[idx] & mask) == 0
        ) break;
        ++countLeadingOnes;
      }
      
      if (2 <= countLeadingOnes && countLeadingOnes <= 4) {
        bool canProceed = true;
        for (int k = idx+1; k <= idx+countLeadingOnes-1; ++k) {
          if (k >= data.size()) {
            canProceed = false;
            break;
          }
          if (!startsWithOneZero(data[k])) {
            canProceed = false;
            break;
          }
        }
        if (canProceed) {
          //printf("%d leads a valid %d-byte symbol\n", data[idx], countLeadingOnes);
          validate(idx+countLeadingOnes, data, ans);
        }
      }
    }
  
    bool validUtf8(vector<int>& data) {
      bool ans = false;
      validate(0, data, ans);
      return ans;
    }
};
