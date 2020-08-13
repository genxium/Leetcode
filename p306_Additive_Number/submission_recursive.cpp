#define ULL uint64_t

class Solution {
public:
    void dfs(int offset, string &num, ULL a, ULL b, bool &ans, int level) {
        int indent = (level*2);
        if (true == ans) {
            return;
        }
        if (offset > num.length()) {
            return;
        }
        if (offset == num.length()) {
            ans = true;
            return;
        }
        ULL c = a + b;
        string charSeqOfC = to_string(c);
        //printf("%*soffset: %d, a:%d, b:%d, charSeqOfC:%s\n", indent, "", offset, a, b, charSeqOfC.c_str());

        for (int i = 0; i < charSeqOfC.length() && offset+i < num.length(); ++i) {
            if (num[offset+i] != charSeqOfC[i]) {        
                return;
            }
        }
        
        dfs(offset+charSeqOfC.length(), num, b, c, ans, level+1);
    }
    
    bool isAdditiveNumber(string num) {
        /*
        test case #1
        "0"
        
        test case #2
        "11"
        
        test case #3
        "101"
        
        test case #4
        "000"
        */
        int l = num.length();
        if (l < 3) return false;
        // now that "l >= 3"
        bool ans = false;
        int iUpper = ('0' == num[0] ? 1 : (l >> 1)); // "a" shouldn't have more than "(l >> 1)" digits

        for (int i = 0; i < iUpper; ++i) {
            ULL a = stoull(num.substr(0, i+1));
            int jUpper = ('0' == num[i+1] ? i+2 : l-1);

            for (int j = i+1; j < jUpper; ++j) {
                ULL b = stoull(num.substr(i+1, j-i).c_str());
                dfs(j+1, num, a, b, ans, 0);
                if (true == ans) return true;
            }
        }
        return ans;
    }
};
