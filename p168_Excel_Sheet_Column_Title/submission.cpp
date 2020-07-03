/*
q: 0 ... 0  | 1 ...  1  | ... | 25 ...  25  | 26  ...  26  | 27  ... 27  
r: 0 ... 25 | 0 ...  25 | ... | 0  ...  25  | 0   ...  25  | 0   ... 25
v: 0 ... 25 | 26 ... 51 | ... | 650 ... 675 | 676 ...  701 | 702 ... 727
s: A ... Z  | AA ... AZ | ... | YA ...  YZ  | ZA  ...  ZZ  | AAA ... AAZ
*/
#define BASE 26
class Solution {
public:
    string convertToTitle(int n) {
        string toRet = "";
        --n;
        do {
            int r = (n%BASE);
            toRet.push_back('A' + r);
            int q = n/BASE;
            n = q-1;
            if (n == 0) {
                toRet.push_back('A');
            }
        } while (n > 0);
        reverse(toRet.begin(), toRet.end());
        return toRet;
    }
};
