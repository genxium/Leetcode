bool const debug = false;
typedef vector<int> VI;

/*
test cases
[[1,2],[2,3],[3,4]]
[[1,2],[2,3],[3,4],[1,2]]
[[1,4],[4,4],[2,2],[3,4],[1,1]]
[[1,100000]]
[[1,1],[1,2],[1,3],[1,4],[1,5],[1,6],[1,7]]
[[1,2],[2,2],[3,3],[3,4],[3,4]]
[[1,2],[1,2],[3,3],[1,5],[1,5]]
[[1,5],[1,5],[1,5],[2,3],[2,3]]
*/
class Solution {
public:
    int maxEvents(vector<vector<int>>& events) {
        sort(events.begin(), events.end(), [](VI const& lhs, VI const &rhs) {
            if (lhs[0] != rhs[0]) {
                return lhs[0] < rhs[0];
            }
            return lhs[1] < rhs[1];
        });

        priority_queue<int, VI, greater<int>> pq;
        int ans = 0, currentDay = 0, i = 0, n = events.size();
        while (i < n || !pq.empty()) {
            if (pq.empty()) {
                currentDay = events[i][0];
            }

            while (i < n && events[i][0] <= currentDay) {
                // Cache a set of "events" that could be attended on "currentDay"
                pq.push(events[i++][1]);
            }

            if (!pq.empty()) {
                // On "currentDay", attend the one that ends earliest.
                ++ans;
                ++currentDay;
                pq.pop(); 
            }

            while (!pq.empty() && pq.top() < currentDay) {
                // Remove the "cached events" that could no longer be attended
                pq.pop();
            }
        }

        return ans;
    }
};
