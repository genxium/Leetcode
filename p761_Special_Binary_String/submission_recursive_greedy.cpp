/*
test cases
"11011000"
"11110000"
*/
class Solution {
private:
    unordered_map<string, string> cache;
public:
    string dfs(string &S) {
        if (cache.count(S)) return cache[S];
        vector<string> uList;
        int balance = 0, lastStart = 0;
        for (int i = 0; i < S.length(); ++i) {
            balance += (S[i] == '1' ? 1 : -1);
            if (balance > 0) continue;
            uList.push_back(S.substr(lastStart, i-lastStart+1));
            lastStart = i+1;
        }
        
        for (auto &u : uList) {
            // Each "u" is definitely "u = 1 + u.substr(1, u.length()-2) + 0", thus we try to maximize the middle part of it.
            string midOfU = u.substr(1, u.length()-2);
            u = "1" + dfs(midOfU) + "0";
        }
        
        // Greedily arrange the "uList".
        sort(uList.begin(), uList.end(), [](string const& lhs, string const &rhs) {
            return (lhs + rhs > rhs + lhs);
        });
        
        string toRet = "";
        for (auto &u : uList) toRet.append(u);
        
        return cache[S] = toRet;
    }
    
    string makeLargestSpecial(string S) {
        /*
        If a substring "u" of "S" is "special", then "S-u" is also "special", because
        - the number of 0's and 1's are equal in S-u", thus will suffice condition#1, and 
        - every prefix in "S-u" is either "v1 = a prefix of S" or "v2 = removed u from a prefix of S", thus will suffice condition#2.
        
        That said, any "special binary string S" can be decomposed into "S = u1 + u2 + ... + uk" where each "u" is "special" and no more decomposable.
        
        Regarding every '1' as '(' and every '0' as ')' helps a lot in understanding these characteristics.
        */
        
        return dfs(S);
    }
};
