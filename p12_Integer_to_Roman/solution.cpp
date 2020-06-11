#include <stdio.h>
#include <list>
#include <string>
using namespace std;

char ones[]  = {'I', 'X', 'C', 'M'};
char fives[] = {'V', 'L', 'D'};

class Solution {
  public:
    string singleDigitToRoman(int digit, int shiftedCount) {
      string res = "";
      if (digit < 4) {
        for (int i = 0; i < digit; ++i) res.push_back(ones[shiftedCount]);
      } else if (digit == 4) {
        res.push_back(ones[shiftedCount]);
        res.push_back(fives[shiftedCount]);
      } else if (digit == 5) {
        res.push_back(fives[shiftedCount]);
      } else if (digit <= 8) {
        res.push_back(fives[shiftedCount]);
        for (int i = 0; i < (digit - 5); ++i) res.push_back(ones[shiftedCount]);
      } else {
        // digit == 9
        res.push_back(ones[shiftedCount]);
        res.push_back(ones[shiftedCount+1]);
      }
      return res;
    }

    string intToRoman(int num) {
      string res = "";

      list<int> digits;
      while (num > 0) {
        int residue = (num%10);
        digits.push_front(residue);
        num /= 10;
      }

      int idx = 0;
      for (list<int>::iterator it = digits.begin(); it != digits.end(); ++it, ++idx) {
        int shiftedCount = (digits.size() - idx - 1);
        // printf("shiftedCount == %d, digits.size() == %d, idx == %d\n", shiftedCount, digits.size(), idx);
        string fragment = this->singleDigitToRoman(*it, shiftedCount);
        res.append(fragment);
      }

      return res;
    }
};

int main() {
  int input = 1994;
  Solution s;
  string res = s.intToRoman(input);
  printf("%s\n", res.c_str());
  return 0;
}
