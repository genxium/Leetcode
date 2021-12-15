typedef vector<int> VI;
bool const debug = false;

class CourseCompare {
public:
    bool operator() (VI const &lhs, VI const &rhs) const {
        if (lhs[1] != rhs[1]) {
            return lhs[1] < rhs[1];
        } else {
            // due at the same date
            return lhs[0] > rhs[0]; // the one that starts earlier comes first   
        }
    }
};

class StateTuple {
public:
    int finishedCourseCount;
    int earliestDayToAchieve;
    
    StateTuple(int a, int b) {
        finishedCourseCount = a;
        earliestDayToAchieve = b;
    }
};

/*
test cases
[[100, 200], [200, 1300], [1000, 1250], [2000, 3200]]
[[5,5],[4,6],[2,6]]
[[5,15],[3,19],[6,7],[2,10],[5,16],[8,14],[10,11],[2,19]]
[[3,2],[4,3]]
*/
class Solution {
public:
    int scheduleCourse(vector<vector<int>>& courses) {
        int n = courses.size();
        sort(courses.begin(), courses.end(), CourseCompare());
        /*
        If there's now a collection of "StateTuple"s, e.g. {st[k]}, then for each "course", some new "st"s can be derived by 
        - for each existing "st[k].earliestDayToAchieve <= (course.dueDay-course.duration)", add          
          ```
          newSt(
            finishedCourseCount: st[k].finishedCourseCount + 1, 
            earliestDayToAchieve: st[k].earliestDayToAchieve + course.duration
          )
          ```
        - reduce "{st[k]}" for having equal "finishedCourseCount" but later "earliestDayToAchieve".
        
        The "reduction" step will result in a collection "{st[k]}" such that each "st[k].finishedCourseCount" is DISTINCT.
        */
        
        vector<StateTuple> stCollection;  
        stCollection.reserve(n); // reduce memory alloc time
        
        // loop
        for (int i = 0; i < n; ++i) {
            VI course = courses[i];
            int prevShouldFinishOnOrBeforeDay = course[1] - course[0];
            if (prevShouldFinishOnOrBeforeDay < 0) continue;
            if (debug) printf("i:%d, stCollection.size() == %d, prevShouldFinishOnOrBeforeDay:%d\n", i, stCollection.size(), prevShouldFinishOnOrBeforeDay);

            // deliberately in reverse-order to avoid contaminating "data from previous round"
            auto prevIt = stCollection.rend();
            for (auto it = stCollection.rbegin(); it != stCollection.rend(); ++it) {
                if (it->earliestDayToAchieve > prevShouldFinishOnOrBeforeDay) {
                    prevIt = it;
                    continue;
                }
                StateTuple candidateSt(it->finishedCourseCount+1, it->earliestDayToAchieve+course[0]);
                if (prevIt == stCollection.rend()) {
                    stCollection.push_back(candidateSt);
                } else if (candidateSt.earliestDayToAchieve < prevIt->earliestDayToAchieve) {
                    prevIt->earliestDayToAchieve = candidateSt.earliestDayToAchieve;
                }
                prevIt = it;
            }
            // wrap up
            StateTuple candidateSt(1, course[0]); // the "only 1 course finished StateTuple"
            if (prevIt == stCollection.rend()) {
                stCollection.push_back(candidateSt);
            } else if (candidateSt.earliestDayToAchieve < prevIt->earliestDayToAchieve) {
                prevIt->earliestDayToAchieve = candidateSt.earliestDayToAchieve;
            }
        }
                
        return stCollection.size();
    }
};
