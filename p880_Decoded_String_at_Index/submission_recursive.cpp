bool debug = false;
typedef uint64_t ULL;
typedef int64_t LL;

class Solution {
private:
    char dfs(int i, vector<ULL> &indexBy, int k, string& encoded) {
        // Find the k-th char in the decoded string of "encoded[0, ..., i]", note that "k <= indexBy[i]" is always true during "dfs".
        if (0 == i) return encoded[0];
        if (k > indexBy[i-1]) {
            if (isalpha(encoded[i])) return encoded[i]; // It must be "k == indexBy[i]"
            else {
                int lastLen = indexBy[i-1]+1;
                return dfs(i-1, indexBy, k%lastLen, encoded);
            }
        } else {
            // k <= indexBy[i-1]
            return dfs(i-1, indexBy, k, encoded);
        }
    }
    
public:
    string decodeAtIndex(string S, int K) {
        int k = K-1; // 0-based
        vector<ULL> indexBy = {0}; // indexBy[i]: the 0-based index of the decoded string by S[0, ..., i] 
        for (int i = 1; i < S.length(); ++i) {
            ULL len = indexBy.back()+1;
            if (isalpha(S[i])) {
                indexBy.push_back(len);
            } else {
                ULL d = S[i]-'0';
                indexBy.push_back(indexBy.back() + (d-1)*len);
            }
            if (debug) printf("indexBy[i:%d] is %llu\n", i, indexBy[i]);
        }
        
        char ch = dfs(S.length()-1, indexBy, k, S);
        string ans = ""; ans.push_back(ch);
        return ans;
    }
};
