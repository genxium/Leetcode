bool debug = false;

typedef pair<int, int> PII; // a.k.a. Interval
int const INVALID = -1;
int n = INVALID;
PII computeNewPos(PII const &t) {
    if (t.first == -1) {
        return {0, t.second-0};
    } else if (t.second == n) {
        return {n-1, n-1-t.first};
    } else {
        int newPos = ((t.first + t.second) >> 1); 
        return {newPos, min(newPos-t.first, t.second-newPos)};
    }
}

class Comp {
public:    
    bool operator () (PII const &lhs, PII const &rhs) const {
        PII newPosL = computeNewPos(lhs);
        PII newPosR = computeNewPos(rhs);
        
        if (newPosL.second == newPosR.second) return newPosL.first < newPosR.first;
        return newPosL.second > newPosR.second;
    }
};

/*
test cases
["ExamRoom","seat","seat","seat","seat","leave","seat"]
[[10],[],[],[],[],[4],[]]
["ExamRoom","seat","seat","seat","leave","leave","seat"]
[[10],[],[],[],[0],[4],[]]
["ExamRoom","seat","seat","seat","leave","leave","seat","seat","seat","seat","seat","seat","seat","seat","seat","leave"]
[[10],[],[],[],[0],[4],[],[],[],[],[],[],[],[],[],[0]]
["ExamRoom","seat","seat","seat","seat","seat","seat"]
[[7],[],[],[],[],[],[]]
["ExamRoom","seat","seat","seat","seat","leave","seat","seat","seat","seat","leave"]
[[7],[],[],[],[],[1],[],[],[],[],[4]]
["ExamRoom","seat","seat","seat","leave","leave","seat","seat","seat","seat","seat","seat","seat"]
[[8],[],[],[],[0],[7],[],[],[],[],[],[],[]]
["ExamRoom","seat","seat","seat","seat","leave","seat","seat","seat","seat","seat","seat","leave","seat"]
[[9],[],[],[],[],[4],[],[],[],[],[],[],[3],[]]
*/
class ExamRoom {
private: 
    set<PII, Comp> pq;
    unordered_map<int, pair<PII, PII>> occupiedSeats;
public:
    ExamRoom(int N) {
        n = N;
        PII interval1 = {-1, -1}, interval2 = {-1, N}, interval3 = {N, N}; 
        pq.insert(interval1);
        pq.insert(interval2);
        pq.insert(interval3);

        occupiedSeats[-1] = make_pair(interval1, interval2);  // assume that seats "-1" and "N" are occupied
        occupiedSeats[N] = make_pair(interval2, interval3);
    }
    
    int seat() {       
        if (debug) printf("Calling seat, pq.size() == %d\n", pq.size());
        int newPos = INVALID;
        while (!pq.empty()) {
            auto it = pq.begin();
            auto t = *it;
            pq.erase(it);
            if (!occupiedSeats.count(t.first) || !occupiedSeats.count(t.second)) {
                // 1 end is already removed
                continue;
            }
            
            PII newPosPair = computeNewPos(t);
            newPos = newPosPair.first;
            
            PII interval1 = {t.first, newPos}, interval2 = {newPos, t.second};
            pq.insert(interval1);
            pq.insert(interval2);
            
            if (debug) printf("\tcandidate t:[%d, %d], newPos == %d, inserting interval1:[%d, %d], interval2:[%d, %d]\n", t.first, t.second, newPos, interval1.first, interval1.second, interval2.first, interval2.second);
            
            // update "occupiedSeats" to help fast removal
            occupiedSeats[t.first].second = interval1;
            occupiedSeats[t.second].first = interval2;
            
            occupiedSeats[newPos].first = interval1;
            occupiedSeats[newPos].second = interval2;
            
            break;
        }
        
        return newPos;
    }
    
    void leave(int p) {
        PII interval1 = occupiedSeats[p].first, interval2 = occupiedSeats[p].second;
        occupiedSeats.erase(p);
        pq.erase(interval1);
        pq.erase(interval2);
        PII newInterval = {interval1.first, interval2.second};
        occupiedSeats[interval1.first].second = newInterval;
        occupiedSeats[interval2.second].first = newInterval;
        if (debug) printf("Leaving p:%d, merging interval1:[%d, %d], interval2:[%d, %d], inserting newInterval:[%d, %d]\n", p, interval1.first, interval1.second, interval2.first, interval2.second, newInterval.first, newInterval.second);
        pq.insert(newInterval);
    }
};

/**
 * Your ExamRoom object will be instantiated and called as such:
 * ExamRoom* obj = new ExamRoom(N);
 * int param_1 = obj->seat();
 * obj->leave(p);
 */
