class OrderedInterval {
public:
    int orderedIdx;
    int st, ed;
    
    OrderedInterval(int aSt, int aEd) {
        st = aSt;
        ed = aEd;
    }
};

class OrderedIntervalCompare {
public:
    bool operator() (OrderedInterval const& lhs, OrderedInterval const& rhs) {
        if (lhs.ed != rhs.ed) {
            return lhs.ed < rhs.ed;
        }
        return lhs.st < rhs.st; 
        /* Longer comes first. */
    }
};

/*
test case#1
[[10,16],[2,8],[1,6],[7,12]]

test case#2
[[9,12],[1,10],[4,11],[8,12],[3,9],[6,9],[6,7]]

test case#3
[[0,9],[1,8],[7,8],[1,6],[9,16],[7,13],[7,10],[6,11],[6,9],[9,13]]
*/
class Solution {
public:
    int findMinArrowShots(vector<vector<int>>& points) {
        int n = points.size();
        if (0 == n) return 0;
        vector<int> orderedX;
        vector<OrderedInterval> orderedList;
        for (auto &point : points) {
            OrderedInterval single(point[0], point[1]);
            orderedList.push_back(single);
            orderedX.push_back(point[0]);
            orderedX.push_back(point[1]);
        }
        sort(orderedX.begin(), orderedX.end());
        vector<int> orderedDedupedX;
        orderedDedupedX.push_back(orderedX[0]);
        for (int i = 1; i < orderedX.size(); ++i) {
            if (orderedX[i] > orderedDedupedX.back()) {
                orderedDedupedX.push_back(orderedX[i]);
            }
        }
        
        sort(orderedList.begin(), orderedList.end(), OrderedIntervalCompare());
        
        unordered_map<int, list<int>> startXToOrderedIntervals;
        unordered_map<int, list<int>> endXToOrderedIntervals;
        int idx = 0;
        for (auto &single : orderedList) {
            single.orderedIdx = idx;
            startXToOrderedIntervals[single.st].push_back(idx);
            endXToOrderedIntervals[single.ed].push_back(idx);
            ++idx;
            
            // printf("orderedList[%d] is {st: %d, ed: %d}\n", single.orderedIdx, single.st, single.ed);
        }
        
        int ans = 0;
        unordered_set<int> activeOrderedIntervals;
        unordered_set<int> exitedOrderedIntervals;
        for (int &x : orderedDedupedX) {
            // printf("@x == %d\n", x);
            // entering
            list<int> &orderIntervalsIdsWithXToStart = startXToOrderedIntervals[x];
            for (auto enterOrderedIdx : orderIntervalsIdsWithXToStart) {
                if (exitedOrderedIntervals.find(enterOrderedIdx) != exitedOrderedIntervals.end()) continue;
                activeOrderedIntervals.insert(enterOrderedIdx);
            }
            
            // exiting
            list<int> &orderIntervalsIdsWithXToEnd = endXToOrderedIntervals[x];
            if (orderIntervalsIdsWithXToEnd.empty()) {
                // printf("no orderIntervalsIdsWithXToEnd associated with this 'x'\n");
                continue;
            }
            for (auto &exitOrderedIdx : orderIntervalsIdsWithXToEnd) {
                if (exitedOrderedIntervals.find(exitOrderedIdx) != exitedOrderedIntervals.end()) continue;
                ++ans;
                // printf("exiting orderedList[%d] {st:%d, ed:%d}\n", exitOrderedIdx, orderedList[exitOrderedIdx].st, orderedList[exitOrderedIdx].ed);
                // By shooting this "orderedList[exitOrderedIdx]" all the other "OrderedInterval"s in the "activeIntervalIndices" can be bursted together.
                for (auto thatIdx : activeOrderedIntervals) {
                    // printf("\ttogether with orderedList[%d] {st:%d, ed:%d}\n", thatIdx, orderedList[thatIdx].st, orderedList[thatIdx].ed);
                    exitedOrderedIntervals.insert(thatIdx);
                }
                activeOrderedIntervals.clear();
                break; // processing "1 exitOrderedIdx per x" is enough
            }

        }
        
        return ans;
    }
};
