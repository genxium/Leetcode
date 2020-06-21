class Solution {
public:
    void multiplyTens(string &num, int countOfTens) {
        for(int i = 0; i < countOfTens; ++i) {
            num.push_back('0');
        }
    }
    
    string multiplyChar(string num, char c, int countOfTens) {
        string reverseRet = "";
        int overflow = 0;
        for (auto it = num.rbegin(); it != num.rend(); ++it) {
            int lhs = *it - '0';
            int rhs = c - '0';
            int tmp = lhs*rhs + overflow;
            overflow = tmp/10;
            char localRes = ('0' + tmp%10);
            reverseRet.push_back(localRes);
        }
        if (overflow > 0) {
            reverseRet.push_back('0' + overflow);
        }
        reverse(reverseRet.begin(), reverseRet.end());
        multiplyTens(reverseRet, countOfTens);
        return reverseRet;
    }
    
    string add(string num1, string num2) {
        string reverseRet = "";
        int overflow = 0;
        auto it1 = num1.rbegin();
        auto it2 = num2.rbegin();
        while (it1 != num1.rend() && it2 != num2.rend()) {
            int lhs = *it1 - '0';
            int rhs = *it2 - '0';
            int tmp = lhs + rhs + overflow;
            overflow = tmp/10;
            char localRes = ('0' + tmp%10);
            reverseRet.push_back(localRes);
            ++it1;
            ++it2;
        }
        
        while (it1 != num1.rend()) {
            int lhs = *it1 - '0';
            int rhs = 0;
            int tmp = lhs + rhs + overflow;
            overflow = tmp/10;
            char localRes = ('0' + tmp%10);
            reverseRet.push_back(localRes);
            ++it1;
        }
        
        while (it2 != num2.rend()) {
            int lhs = *it2 - '0';
            int rhs = 0;
            int tmp = lhs + rhs + overflow;
            overflow = tmp/10;
            char localRes = ('0' + tmp%10);
            reverseRet.push_back(localRes);
            ++it2;
        }
        
        if (overflow > 0) {
            reverseRet.push_back('0' + overflow);
        }
        
        reverse(reverseRet.begin(), reverseRet.end());
        return reverseRet;
    }
    
    string multiply(string num1, string num2) {
        int num1Len = num1.length();
        if (num1Len == 0) return "0";
        if (num1Len == 1) {
            if (num1[0] == '0') return "0";
            if (num1[0] == '1') return num2;
        }
        int num2Len = num2.length();
        if (num2Len == 0) return "0";
        if (num2Len == 1) {
            if (num2[0] == '0') return "0";
            if (num2[0] == '1') return num1;
        }  
        
        string sum = "0";
        for (int i = num2Len-1; i >= 0; --i) {
            string row = multiplyChar(num1, num2[i], num2Len-1-i);
            //printf("%s multiplies %c, shifted %d to left is %s\n", num1.c_str(), num2[i], num2Len-1-i, row.c_str());
            sum = add(sum, row);
        }
        return sum;
    }
};
