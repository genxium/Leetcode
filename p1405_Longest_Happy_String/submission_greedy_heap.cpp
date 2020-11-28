class Solution {
public:
    string longestDiverseString(int a, int b, int c) {
        priority_queue<pair<int,char>> pq; // max-heap of {remained, character}
        if (a) pq.push({a,'a'});
        if (b) pq.push({b,'b'});
        if (c) pq.push({c,'c'});
        string ans="";
        while (pq.size()>1) {
            auto cand1 = pq.top(); pq.pop();
            auto cand2 = pq.top(); pq.pop();
            if (cand1.first>=2) {
                ans += cand1.second;
                ans += cand1.second;
                cand1.first -= 2;
            } else{
                ans += cand1.second;
                --cand1.first;
            }
            
            /*
            * [WARNING]
            * It's comparing "cand2.remained" with "modified cand1.remained"!
            */
            if (cand2.first>=2 && cand2.first >= cand1.first) {
                ans += cand2.second;
                ans += cand2.second;
                cand2.first -= 2;
            } else {
                // now that either "cand2.first <= 1" or "cand2.first < (modified)cand1.first", in the latter case we should try to save "cand2"
                ans += cand2.second;
                --cand2.first;
            }
            if (cand1.first > 0) pq.push(cand1);
            if (cand2.first > 0) pq.push(cand2);
        }
        if (pq.empty()) return ans;
        if (pq.top().first >= 2) {
            ans += pq.top().second;
            ans += pq.top().second;
        } else{
            ans += pq.top().second;
        }
        return ans;
    }
};
