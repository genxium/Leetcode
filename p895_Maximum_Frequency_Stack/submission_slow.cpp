bool debug = false;
typedef pair<int, vector<int>> OCCURRENCE_T; // {value, stack of occurrence times of this value}, a "vector" is used here instead of "stack" to make use of fast continuous mem copying
struct OccurrenceCmp {
    bool operator() (OCCURRENCE_T const& lhs, OCCURRENCE_T const& rhs) const {
        return lhs.second.back() > rhs.second.back();
    }
};

typedef set< OCCURRENCE_T, OccurrenceCmp > OCCURRENCE_SET_T; // thus "begin()" is the value whose locally latest occurrence is also GLOBALLY latest 

typedef OCCURRENCE_SET_T::iterator OCCURRENCE_SET_IT_T;

/*
test cases
["FreqStack","push","push","push","push","push","push","pop","pop","pop","pop"]
[[],[5],[7],[5],[7],[4],[5],[],[],[],[]]
["FreqStack","push","push","push","push","push","push","pop","pop","pop","pop"]
[[],[7],[7],[5],[5],[4],[5],[],[],[],[]]
["FreqStack","push","push","push","push","push","push","pop","push","pop","push","pop","push","pop","push","pop","pop","pop","pop","pop","pop"]
[[],[4],[0],[9],[3],[4],[2],[],[6],[],[1],[],[1],[],[4],[],[],[],[],[],[]]
*/
class FreqStack {
private:
    int t;
    map<int, OCCURRENCE_SET_T, greater<int>> freqToVal; // freq -> OCCURRENCE_SET_T
    unordered_map< int, pair<int, OCCURRENCE_SET_IT_T> > valToFreq; // value -> {freq, OCCURRENCE_SET_IT_T}
public:
    FreqStack() {
        t = 0;
    }
    
    void push(int val) {
        if (debug) printf("push %d\n", val);
        if (valToFreq.count(val)) {
            int oldFreq = valToFreq[val].first;
            if (debug) printf("\toldFreq == %d\n", oldFreq);
            OCCURRENCE_SET_IT_T oldIt = valToFreq[val].second;
            OCCURRENCE_T newOccurrence = {val, vector<int>()};
            move(oldIt->second.begin(), oldIt->second.end(), back_inserter(newOccurrence.second));
            newOccurrence.second.push_back({t});

            freqToVal[oldFreq].erase(valToFreq[val].second);
            if (debug) printf("\tval erased in oldFreq set\n");
            int newFreq = ++valToFreq[val].first;
            OCCURRENCE_SET_IT_T newIt = freqToVal[newFreq].insert(newOccurrence).first;
            if (debug) printf("\tval added in newFreq set\n");
            valToFreq[val].second = newIt;
            if (debug) printf("\tnewIt assigned\n");
        } else {
            if (debug) printf("\ta new element\n");
            int newFreq = 1;
            OCCURRENCE_T newOccurrence = { val, vector<int> {t} };
            OCCURRENCE_SET_IT_T newIt = freqToVal[newFreq].insert(newOccurrence).first;
            valToFreq[val] = {newFreq, newIt};
        }
        ++t;
    }
    
    int pop() {
        if (debug) printf("popping\n");
        auto targetMapIt = freqToVal.begin(); // larger key comes first
        auto &targetOccurrenceSet = targetMapIt->second;
        OCCURRENCE_SET_IT_T oldIt = targetOccurrenceSet.begin(); // the one with globally latest occurrence when freq is a tie 
        int oldFreq = targetMapIt->first;
        int targetVal = oldIt->first;
        if (debug) printf("\treturning %d of oldFreq: %d\n", targetVal, oldFreq);
        int newFreq = --valToFreq[targetVal].first;
        OCCURRENCE_T newOccurrence = {targetVal, vector<int>()};
        move(oldIt->second.begin(), oldIt->second.begin() + oldIt->second.size()-1, back_inserter(newOccurrence.second));
        if (debug) printf("\t%d occurrences remained, which should be equal to newFreq:%d\n", newOccurrence.second.size(), newFreq);

        // Regardless of "0 <> newFreq", the freq of "targetVal" must change
        targetOccurrenceSet.erase(oldIt);
        if (debug) printf("\toldIt removed from freqToVal[oldFreq]\n");
        if (targetOccurrenceSet.empty()) {
            freqToVal.erase(targetMapIt);
        }
        if (0 == newFreq) {
            // no need to hold a new list element
            // delete newOccurrence.second;
            valToFreq.erase(targetVal);
        } else {
            OCCURRENCE_SET_IT_T newIt = freqToVal[newFreq].insert(newOccurrence).first;
            if (debug) printf("\tval added in newFreq set\n");
            valToFreq[targetVal].second = newIt;
        }
        return targetVal;
    }
};

/**
 * Your FreqStack object will be instantiated and called as such:
 * FreqStack* obj = new FreqStack();
 * obj->push(val);
 * int param_2 = obj->pop();
 */
