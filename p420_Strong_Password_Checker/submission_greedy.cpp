#define VIOLATION_A 'A'
#define VIOLATION_B 'B'
#define VIOLATION_C 'C'
#define VIOLATION_D 'D'
#define VIOLATION_E 'E'
#define VIOLATION_F 'F'
#define INVALID -1

/*
 * Types of violations.
 * A. length < 6
 * B. length > 20
 * C. No uppercase
 * D. No lowercase
 * E. No digit 
 * F. 3 consecutively repeating characters
 *
 * Types of resolutions
 * A -> insert at side without causing consecutive repetition
 * B -> delete a "non-last uppercase", "non-last lowercase" or "non-last digit", 
 * C,D,E -> insert the first missing symbol type, or 
 *       -> replace a "non-last uppercase", "non-last lowercase" or "non-last digit" with the first missing symbol type  
 * F -> delete at sides, or 
 *   -> replace every 3rd with a different character, hopefully with the first missing symbol type (every 3rd repeating character is definitely a "non-last uppercase/lowercase/digit")  
 *
 * A guess is that "as long as the string is free from VIOLATION_A & VIOLATION_B, then using `replace` is always a valid resolution to the rest violations". It's not obvious that this guess is correct, considering that we might accidentally create VIOLATION_C/VIOLATION_D/VIOLATION_E -- yet in fact as long as VIOLATION_A doesn't exist, we can always replace without causing VIOLATION_C/VIOLATION_D/VIOLATION_E but only with resolving them!  
 * */

#define PI pair<int, int>

class HeapCompare {
public:
    bool operator() (PI const& lhs, PI const &rhs) const {
        // Let smaller (mod 3) result come first when popping, because for deletion, a smaller (mod 3) result means "best reduction for the later replacement operations", i.e. deleting a character from a (mod 3 == 0) interval immediately reduces the later required replacement by 1. 
        int lMod = (lhs.second-lhs.first)%3;
        int rMod = (rhs.second-rhs.first)%3;
        return lMod > rMod;  
    }  
};

#define VIO_T unordered_set<char>
#define REP_INTERVAL_COLLECTION_T priority_queue<PI, vector<PI>, HeapCompare> 

/*
test case#1
""

test case#2
"aaaaa"

test case#3
"aaaaajsaidfuIUjocijie19918sadnjaJJJJJJjjjjjusuuuudsaub1"

test case#4
"aaa123"

test case#5
"aaa111"

test case#6
"aaa11"

test case#7
"aaa111aaa111aaa111aaa111"

test case#8
"aaa111aaa111aaa111aaaaaa"

test case#9
"aaa111aaa111aaa111aaaaaaa"

test case#10
"aaa111aaa111aaa111aaaaaaaaaa111aaa111aaa11111111aaaaaaa1111111aa1111111"
*/
class Solution {
    private:
        void solveWhenLengthComplies(VIO_T &violations, REP_INTERVAL_COLLECTION_T &overThreeRepetitionIntervals, int &opCount) {
            //printf("solveWhenLengthComplies, already has violations.size() == %d, overThreeRepetitionIntervals.size() == %d, opCount == %d\n", violations.size(), overThreeRepetitionIntervals.size(), opCount);
            // uses only replacement 
            if (violations.find(VIOLATION_F) == violations.end()) {
                if (violations.find(VIOLATION_C) != violations.end()) ++opCount; 
                if (violations.find(VIOLATION_D) != violations.end()) ++opCount; 
                if (violations.find(VIOLATION_E) != violations.end()) ++opCount; 
            } else {
                int charLackCount = 0; 
                if (violations.find(VIOLATION_C) != violations.end()) ++charLackCount; 
                if (violations.find(VIOLATION_D) != violations.end()) ++charLackCount; 
                if (violations.find(VIOLATION_E) != violations.end()) ++charLackCount; 
                //printf("\tinitiated charLackCount == %d\n", charLackCount);

                int pureRepetitionResolutoinReplacementCount = 0;
                while (!overThreeRepetitionIntervals.empty()) {
                    auto &interval = overThreeRepetitionIntervals.top(); 
                    int intervalLength = interval.second - interval.first; 
                    int replacementCount = intervalLength/3; 
                    while (replacementCount--) {
                        ++pureRepetitionResolutoinReplacementCount;
                        if (0 < charLackCount) {
                            --charLackCount; // use the lacking character to partition repetition violating intervals
                        }
                    }
                    overThreeRepetitionIntervals.pop();
                }
                //printf("\tcharLackCount == %d, pureRepetitionResolutoinReplacementCount == %d\n", charLackCount, pureRepetitionResolutoinReplacementCount);
                opCount += (pureRepetitionResolutoinReplacementCount + charLackCount);
            }
        }

        void solve(string &s, int &opCount) {
            VIO_T violations;
            REP_INTERVAL_COLLECTION_T overThreeRepetitionIntervals;

            if (s.length() < 6) violations.insert(VIOLATION_A); 
            if (s.length() > 20) violations.insert(VIOLATION_B); 

            char prevCh = '\0';
            pair<int, int> activeInterval = {INVALID, INVALID}; // [closedSt, openEd)

            int uppercaseCount = 0, lowercaseCount = 0, digitCount = 0;  
            for (int i = 0; i < s.length(); ++i) {
                char ch = s[i];
                if (isdigit(ch)) ++digitCount;
                if (isalpha(ch)) {
                    if (ch >= 'a') ++lowercaseCount; 
                    else ++uppercaseCount;
                }
                if (ch != prevCh) {
                    if (INVALID == activeInterval.first) {
                        activeInterval.first = i;  
                        //printf("Updated activeInterval.first == %d\n", activeInterval.first);
                    } else {
                        if (INVALID == activeInterval.second) {
                            activeInterval.second = i; 
                            //printf("Updated activeInterval.first == %d, activeInterval.second == %d\n", activeInterval.first, activeInterval.second);
                            if (activeInterval.second - activeInterval.first >= 3) {
                                overThreeRepetitionIntervals.push({activeInterval.first, activeInterval.second}); 
                            }
                            // reset
                            activeInterval.first = i;
                            activeInterval.second = INVALID;
                        }
                    }
                }  
                prevCh = ch;
            }
            
            // wrap up
            activeInterval.second = s.length(); 
            if (activeInterval.second - activeInterval.first >= 3) {
                overThreeRepetitionIntervals.push({activeInterval.first, activeInterval.second}); 
            }

            if (0 == uppercaseCount) violations.insert(VIOLATION_C);
            if (0 == lowercaseCount) violations.insert(VIOLATION_D);
            if (0 == digitCount) violations.insert(VIOLATION_E);

            if (0 < overThreeRepetitionIntervals.size()) violations.insert(VIOLATION_F); 

            if (violations.find(VIOLATION_A) == violations.end() && violations.find(VIOLATION_B) == violations.end()) {
                solveWhenLengthComplies(violations, overThreeRepetitionIntervals, opCount);
                return;
            }

            if (violations.find(VIOLATION_B) == violations.end()) {
                // violations.find(VIOLATION_A) != violations.end() 
                // too few characters
                int charLackCount = 0; 
                if (violations.find(VIOLATION_C) != violations.end()) ++charLackCount; 
                if (violations.find(VIOLATION_D) != violations.end()) ++charLackCount; 
                if (violations.find(VIOLATION_E) != violations.end()) ++charLackCount; 

                int pureStrLengthDiff = 6 - s.length(); 
                if (pureStrLengthDiff >= charLackCount) opCount += pureStrLengthDiff;
                else opCount += charLackCount; // e.g. "aaaaa" has "charLackCount == 2" but "pureStrLengthDiff == 1"
            } else {
                // violations.find(VIOLATION_B) != violations.end() 
                // too many characters 
                int repetitionDeletionCount = 0;
                while (s.length() - repetitionDeletionCount > 20 && !overThreeRepetitionIntervals.empty()) {
                    auto interval = overThreeRepetitionIntervals.top(); overThreeRepetitionIntervals.pop();
                    PI newInterval = {interval.first, interval.second-1}; // delete the last character of this interval
                    if (newInterval.second - newInterval.first >= 3) {
                        overThreeRepetitionIntervals.push(newInterval);
                    }
                    ++repetitionDeletionCount;
                }

                int pureDeletionCount = 0;
                while (s.length() - repetitionDeletionCount - pureDeletionCount > 20) {
                    ++pureDeletionCount;
                }

                opCount += (repetitionDeletionCount + pureDeletionCount);
                violations.erase(VIOLATION_B);
                if (overThreeRepetitionIntervals.empty()) {
                    violations.erase(VIOLATION_F);
                }
                solveWhenLengthComplies(violations, overThreeRepetitionIntervals, opCount);
            }
        } 

    public:
        int strongPasswordChecker(string s) {
            int ans = 0;         
            solve(s, ans);
            return ans;
        }
};
