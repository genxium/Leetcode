bool debug = false;
typedef pair<double, int> PDI;

/*
test cases
[10,20,5]
[70,50,30]
2
[3,1,10,10,1]
[4,8,2,2,7]
3
*/
class Solution {
public:
    double mincostToHireWorkers(vector<int>& quality, vector<int>& wage, int K) {
        vector<PDI> items;
        for (int i = 0; i < quality.size(); ++i) {
            double slope = 1.0*wage[i]/quality[i];
            items.push_back({slope, quality[i]});
        }

        sort(items.begin(), items.end(), [](PDI const &lhs, PDI const &rhs) {
            if (lhs.first != rhs.first) return lhs.first < rhs.first;
            return lhs.second < rhs.second;
        });

        /*
        The target is to find a minimum "s*SUM{items[i].second} where s == MAX{items[i].first} and K == COUNT{items[chosen]}".

        If we traverse all items by the ascending order of "items[i].first a.k.a. the slope", then it's just a matter of maintaining the "first K smallest items[i].second".
        */

        double ans = DBL_MAX;
        priority_queue<int> pq; // max-heap of "items[i].second"
        int qs = 0; // sum of "items[i].second" in "pq"
        for (int i = 0; i < items.size(); ++i) {
            auto &item = items[i]; 
            auto &s = item.first; // Always the largest by far.
            auto &currentQ = item.second;
            if (debug) printf("Checking items[i:%d] {s: %.3lf, currentQ: %d}, items.size(): %lu\n", i, s, currentQ, items.size());
            if (K == pq.size()) {
                auto t = pq.top();
                if (debug) printf("\tcomparing t:%d v.s. currentQ:%d\n", t, currentQ);
                if (t < currentQ) continue;
                pq.pop();
                qs -= t;
                if (debug) printf("\tpopped, now that s:%.3lf, qs:%d, pq.size(): %lu\n", s, qs, pq.size());
            }

            pq.push(currentQ);
            qs += currentQ;
            if (debug) printf("\tpushed, now that s:%.3lf, qs:%d, pq.size(): %lu\n", s, qs, pq.size());

            if (K == pq.size()) {
                ans = min(ans, s*qs);
            }
        }

        return ans;
    }
};
