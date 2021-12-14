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

typedef vector<int> VI;

class CourseCompare {
public:
    bool operator() (VI const &lhs, VI const &rhs) const {
        return lhs[1] < rhs[1];
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
        vector<VI> validCourses;
        validCourses.reserve(courses.size());
        for (VI &course : courses) {
            if (course[1] < course[0]) continue;
            validCourses.push_back(course);
        }
        int n = validCourses.size();
        if (0 == n) return 0;
        sort(validCourses.begin(), validCourses.end(), CourseCompare());
        
        // init
        VI& firstCourse = validCourses[0];
        priority_queue<int, vector<int>, less<int>> representingCourses; 
        int finishedCourseCount = 1; 
        int earliestDayToAchieve = firstCourse[0];
        representingCourses.push(firstCourse[0]);
        
        // loop
        for (int i = 1; i < n; ++i) {
            VI& course = validCourses[i];
            int prevShouldFinishOnOrBeforeDay = course[1] - course[0];
            
            if (earliestDayToAchieve <= prevShouldFinishOnOrBeforeDay) {
                finishedCourseCount += 1;
                earliestDayToAchieve = (earliestDayToAchieve + course[0]);
                representingCourses.push(course[0]);
            } else {
                int targetCourseDurationToReplace = representingCourses.top();
                if (targetCourseDurationToReplace > course[0]) {
                    // In this case, it's always beneficial to replace an existing longer duration by the current shorter duration (and later due)
                    representingCourses.pop();
                    representingCourses.push(course[0]);
                    earliestDayToAchieve -= (targetCourseDurationToReplace - course[0]);
                }
            }
        }
                
        return finishedCourseCount;
    }
};


