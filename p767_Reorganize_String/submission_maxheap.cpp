typedef pair<int, char> PIC;

/*
test cases
"aab"
"aaab"
*/
class Solution {
public:
    string reorganizeString(string S) {
        unordered_map<int, int> counter;
        for (auto ch : S) ++counter[ch];
        
        priority_queue<PIC, vector<PIC>, less<PIC>> pq;
        for (auto &it : counter) pq.push({it.second, it.first});
        
        string ans = "";
        char lastCh = '\0';
        while (!pq.empty()) {
            auto t = pq.top(); pq.pop();
            if (t.second != lastCh) {
                ans.push_back(t.second);
                lastCh = t.second;
                --t.first;
                if (t.first > 0) pq.push(t);
            } else {
                // t.second == lastCh
                if (pq.empty()) return "";
                auto u = pq.top(); pq.pop();
                ans.push_back(u.second);
                lastCh = u.second;
                --u.first;
                if (u.first > 0) pq.push(u);
                pq.push(t);
            }
        }
        
        return ans;
    }
};
