typedef vector<pair<char, int>> VPCI;

class Solution {
private:
    VPCI count(string &str) {
        VPCI cnt;
        pair<char, int> activeCnt = {'\0', 0};
        for (int i = 0; i < str.length(); ++i) {
            if (str[i] == activeCnt.first) {
                ++activeCnt.second;
            } else {
                if (activeCnt.first != '\0') cnt.push_back(activeCnt);
                activeCnt = {str[i], 1};
            }
        }
        
        // wrap up
        if (activeCnt.first != '\0') cnt.push_back(activeCnt);
        return cnt;
    }
    
    bool contains(VPCI &lhs, VPCI& rhs) {
        if (lhs.size() != rhs.size()) return false;
        for (int i = 0; i < lhs.size(); ++i) {
            if (lhs[i].first != rhs[i].first) return false;
            if (lhs[i].second < rhs[i].second) return false;
            if (lhs[i].second < 3 && lhs[i].second != rhs[i].second) return false;
        }
        
        return true;
    }
    
public:
    int expressiveWords(string S, vector<string>& words) {
        int ans = 0;
        
        VPCI sCnt = count(S);
        for (auto &word : words) {
            VPCI wordCnt = count(word);
            ans += contains(sCnt, wordCnt);    
        }
        
        return ans;
    }
};
