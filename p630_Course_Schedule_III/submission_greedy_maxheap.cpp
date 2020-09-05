/*
In https://github.com/genxium/Leetcode/blob/master/p630_Course_Schedule_III/submission_bruteforce_TLE.cpp, the most time wasting part is the traversal of "stCollection" for each "courses[i]".

In that TLE approach, the nature of "stCollection" is pretty EXHAUSTIVE, i.e. some "st[k]"s in "stCollection" might represent REDUNDANT INFORMATION of combinations of "courses[]". For example, having "courses[]"
- courses[0] (duration: 2, dueDay: 2)
- courses[1] (duration: 2, dueDay: 4)
- courses[2] (duration: 1, dueDay: 5)

will result in

- st[0] (finishedCourseCount: 1, earliestDayToAchieve: 1) represents "courses[2]" which has an overall smallest "duration"
- st[1] (finishedCourseCount: 2, earliestDayToAchieve: 3) represents "courses[2], courses[1]"
- st[2] (finishedCourseCount: 3, earliestDayToAchieve: 5) represents "courses[0], courses[1], courses[2]"

where "st[2]" was actually derived BEFORE "st[1] & st[0]" within the last round at "courses[2]", and backthen "st[1]" was still (finishedCourseCount: 2, earliestDayToAchieve: 4).

The trick to improve is keeping track of "stCollection.back()" alone. Upon the evaluation of each "courses[i]", if it's INVALID for expanding "stCollection.back()", we can actually be GREEDY to reduce "stCollection.back().earliestDayToAchieve" 
- find the "targetCourseToReplace" where "targetCourseToReplace.duration == max{course.duration | course in stCollection.back().representingCourses}"
- if "courses[i].duration > targetCourseToReplace.duration", do nothing
- update "stCollection.back()" respectively.
*/

#define VI vector<int>
#define STCOLLECTIONTYPE list<StateTuple>

class CourseCompare {
public:
    bool operator() (VI const &lhs, VI const &rhs) const {
        return lhs[1] < rhs[1];
    }
};

class StateTuple {
public:
    int finishedCourseCount;
    int earliestDayToAchieve;
    priority_queue<VI, vector<VI>, less<VI>> representingCourses;
    
    StateTuple(int a, int b, priority_queue<VI, vector<VI>, less<VI>> c) {
        finishedCourseCount = a;
        earliestDayToAchieve = b;
        representingCourses = c;
    }
};

class Solution {
public:
    int scheduleCourse(vector<vector<int>>& courses) {
        /*
        test case #1
        [[100, 200], [200, 1300], [1000, 1250], [2000, 3200]]
        
        test case #2
        [[5,5],[4,6],[2,6]]
        
        test case #3
        [[5,15],[3,19],[6,7],[2,10],[5,16],[8,14],[10,11],[2,19]]
        */
        int n = courses.size();
        sort(courses.begin(), courses.end(), CourseCompare());
        /*
        printf("sorted courses\n");
        for (int i = 0; i < n; ++i) {
            VI course = courses[i];
            printf("\tcourse.duration:%d, course.dueDay:%d\n", course[0], course[1]);
        }
        */
        // init
        VI firstCourse = courses[0];
        priority_queue<VI, vector<VI>, less<VI>> representingCourses; 
        representingCourses.push(firstCourse);
        StateTuple stCollectionBack(1, firstCourse[0], representingCourses);
        
        // loop
        for (int i = 1; i < n; ++i) {
            VI course = courses[i];
            int prevShouldFinishOnOrBeforeDay = course[1] - course[0];
            
            if (stCollectionBack.earliestDayToAchieve <= prevShouldFinishOnOrBeforeDay) {
                stCollectionBack.finishedCourseCount += 1;
                stCollectionBack.earliestDayToAchieve = (stCollectionBack.earliestDayToAchieve + course[0]);
                stCollectionBack.representingCourses.push(course);
            } else {
                VI targetCourseToReplace = stCollectionBack.representingCourses.top();
                if (targetCourseToReplace[0] > course[0]) {
                    /*
                    As "courses[]" is sorted w.r.t. ascending order of ".dueDay", it's guaranteed that "targetCourseToReplace" won't be a valid candidate for "stCollectionBack" anymore after popped out, because by reaching here 
                    - "targetCourseToReplace.duration > course.duration", 
                    - && "targetCourseToReplace.dueDay <= course.dueDay", 
                    - && "SUM(stCollectionBack.representingCourses.duration) + course.duration > course.dueDay".    
                    */
                    stCollectionBack.representingCourses.pop();
                    stCollectionBack.representingCourses.push(course);
                    stCollectionBack.earliestDayToAchieve -= (targetCourseToReplace[0] - course[0]);
                }
            }
        }
                
        return stCollectionBack.finishedCourseCount;
    }
};
