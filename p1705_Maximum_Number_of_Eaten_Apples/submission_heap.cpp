typedef vector<int> VI;
typedef pair<int, int> PII;
bool const debug = false;

/*
test cases
[1,2,3,5,2]
[3,2,1,4,2]
[3,0,0,0,2]
[3,0,0,0,2]
[1]
[2]
[3,1,1,0,0,2]
[3,1,1,0,0,2]
[2,2,2]
[6,4,2]
[0,19,19,19,11,14,33,0,28,7,0,28,7,0,21,16,0,22,0,13,8,0,19,0,0,2,26,2,22,0,8,0,0,27,19,16,24,0,20,26,20,7,0,0,29,0,0,16,19,0,0,0,29,30,17,0,23,0,0,26,24,13,3,0,21,0,18,0]
[0,5,1,16,7,10,54,0,40,2,0,23,4,0,20,18,0,40,0,22,8,0,35,0,0,3,24,1,8,0,10,0,0,2,38,8,4,0,36,33,14,9,0,0,56,0,0,21,27,0,0,0,14,20,18,0,42,0,0,44,3,8,3,0,10,0,27,0]
*/
class Solution {
public:
    int eatenApples(vector<int>& apples, vector<int>& days) {
        int n = apples.size();
        priority_queue<PII, vector<PII>, greater<PII>> pq;

        int d = 0, ans = 0, i = 0;
        while (i < n || !pq.empty()) {
            if (debug) printf("Checking i:%d, d:%d\n", i, d);
            if (i < n) {
                /*
                DON'T push all tuples into the heap by initialization, as we pop only the tuple with earlier "ed", it's possible that the next "u = pq.top()" has later "ed" but earlier "st" than the current "t" -- as shown in the 1st test case.

                To avoid such hassle, we push into the heap by the order of "st" and try to pop & process immediately for the current "d".
                */
                pq.push({i+days[i], apples[i]});
                ++i;
            }
            while (!pq.empty() && pq.top().first <= d) pq.pop(); // pops out already rotten tuples
            if (!pq.empty()) {
                auto t = pq.top(); pq.pop();
                ++ans;
                if (debug) printf("\tAte 1 more apple(total:%d) on day:%d by tuple {ed:%d, apples:%d}\n", ans, d, t.first, t.second);
                --t.second;
                if (t.second > 0) pq.push(t);
            }
            ++d;
        }
        return ans;
    }
};
