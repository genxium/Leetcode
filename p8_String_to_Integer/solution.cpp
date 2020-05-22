#include <limits.h>
#include <string>
#include <stdio.h>
using namespace std;

class Solution {
public:
    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }
    
    int myAtoi(string str) {
        int tmp = 0;
        char sign = ' ';
        bool validDigitStarted = false;
        bool duringConsecutiveNondigits = false;
        int firstNonspaceIdx = 0;
        while(' ' == str[firstNonspaceIdx]) {
            ++firstNonspaceIdx;
        }
        for (int i = firstNonspaceIdx; i < str.length(); ++i) {
            if (true == duringConsecutiveNondigits) {
                continue;
            } else {
                // false == duringConsecutiveNondigits
                if ('+' == str[i] || '-' == str[i]) {
                    if (false == validDigitStarted) {
                        if (' ' != sign) return 0;
                        sign = str[i];                        
                    } else {
                        duringConsecutiveNondigits = true;
                    }
                } else if (isDigit(str[i])) {
                    if (' ' == sign) {
                        sign = '+';
                    }
                    int newDiff = (str[i] - '0');
                    if ('+' == sign) {
                        int residue = (INT_MAX % 10);
                        int afterDiv = (INT_MAX/10); 
                        if (tmp > afterDiv || (tmp == afterDiv && newDiff > residue)) {
                          tmp = INT_MAX;
                        } else {
                          tmp = tmp*10L + newDiff; 
                        }
                    } else {
                        int residue = (INT_MIN % (-10));
                        int afterDiv = (INT_MIN/10); 
                        if (tmp < afterDiv || (tmp == afterDiv && -newDiff < residue)) {
                          tmp = INT_MIN;
                        } else {
                          tmp = tmp*10L - newDiff; 
                        }
                    }
                    validDigitStarted = true;
                } else {
                    if (false == validDigitStarted) {
                        return 0;
                    } else {
                        duringConsecutiveNondigits = true;
                    }
                }
            }            
        }
        
        return tmp;
    }
};

int main() {
  string input = "-6147483648";
  Solution sIns;
  int output = sIns.myAtoi(input);
  printf("%d\n", output);
  return 0;
}
