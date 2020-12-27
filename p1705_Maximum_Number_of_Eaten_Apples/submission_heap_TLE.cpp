typedef vector<int> VI;
bool const debug = false;

class Solution {
public:
    int eatenApples(vector<int>& apples, vector<int>& days) {
        int n = apples.size();
        priority_queue<VI, vector<VI>, greater<VI>> pq;
        
        for (int i = 0; i < n; ++i) {
            if (0 == apples[i]) continue;
            pq.push({i, i+days[i], apples[i]});
        }
        
        int d = 0, ans = 0;
        while (!pq.empty()) {
            auto t = pq.top(); pq.pop();
            if (debug) printf("Checking {st:%d, ed:%d, apples:%d} for day:%d\n", t[0], t[1], t[2], d);
            if (t[1] <= d) continue;

            if (t[0] > d)  d = t[0];

            if (debug) printf("Ate 1 apple on day:%d\n", d);
            --t[2];
            ++ans;
            ++d;
            if (t[2] > 0) pq.push(t);
        }
        return ans;
    }
};
