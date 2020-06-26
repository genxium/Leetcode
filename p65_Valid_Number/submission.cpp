#define GRAND_STATE_BEFORE_E 0
#define GRAND_STATE_AFTER_E 1

#define SINGLE_STATE_EMPTY_BEFORE_DECIMAL_POINT 0
#define SINGLE_STATE_EMPTY_AFTER_DECIMAL_POINT 1
#define SINGLE_STATE_SIGN_DIGESTED 2
#define SINGLE_STATE_DIGIT_BEFORE_DECIMAL_POINT_DIGESTING 3
#define SINGLE_STATE_DIGIT_AFTER_DECIMAL_POINT_DIGESTING 4

class Solution {
public:
    bool validTerminationOfSingleStateEmptyAfterDecimalPoint(string s) {
        if (1 == s.length()) {
            return false;
        }
        
        if (2 == s.length() && ('-' == s[0] || '+' == s[0])) {
            return false;
        }
        return true;
    }
        
    void dfs(string &trimmedStr, int &segStart, int offset, int &grandState, int &singleState, bool &rejected) {
        if (true == rejected) {
            //printf("already rejected spot\n");
            return;
        }
        if (offset >= trimmedStr.length()) {
            if (SINGLE_STATE_EMPTY_AFTER_DECIMAL_POINT == singleState) {
                bool tmp = validTerminationOfSingleStateEmptyAfterDecimalPoint(trimmedStr.substr(segStart, offset-segStart));
                if (false == tmp) {
                    //printf("rejected spot#0\n");
                    rejected = true;
                    return;
                }
            }
            
            //printf("finished traversing successfully, grandState: %d, singleState: %d\n", grandState, singleState);
            return;
        }
        switch (singleState) {
            case SINGLE_STATE_EMPTY_BEFORE_DECIMAL_POINT:
                // process only the valid state transitions
                if (trimmedStr[offset] == '+' || trimmedStr[offset] == '-') {
                    singleState = SINGLE_STATE_SIGN_DIGESTED;
                    ++offset;
                } else if (trimmedStr[offset] == '.') {
                    if (grandState == GRAND_STATE_BEFORE_E) {
                        singleState = SINGLE_STATE_EMPTY_AFTER_DECIMAL_POINT;
                        ++offset;
                    } else {
                        // I don't know why "99e2.5" is not a valid number in this problem
                        //printf("confusing rejected spot#1\n");
                        rejected = true;
                        return;
                    }
                } else if (isdigit(trimmedStr[offset])) {
                    singleState = SINGLE_STATE_DIGIT_BEFORE_DECIMAL_POINT_DIGESTING;
                    ++offset;
                } else {
                    //printf("rejected spot#1\n");
                    rejected = true;
                    return;
                }
                dfs(trimmedStr, segStart, offset, grandState, singleState, rejected);
                break;
            case SINGLE_STATE_EMPTY_AFTER_DECIMAL_POINT:
                if (isdigit(trimmedStr[offset])) {
                    singleState = SINGLE_STATE_DIGIT_AFTER_DECIMAL_POINT_DIGESTING;
                    ++offset;
                } else if (trimmedStr[offset] == 'e') {
                    bool tmp = validTerminationOfSingleStateEmptyAfterDecimalPoint(trimmedStr.substr(segStart, offset-segStart));
                    if (false == tmp) {
                        //printf("rejected spot#2.1\n");
                        rejected = true;
                        return;
                    }
                    if (grandState == GRAND_STATE_BEFORE_E) {
                        grandState = GRAND_STATE_AFTER_E;
                        singleState = SINGLE_STATE_EMPTY_BEFORE_DECIMAL_POINT;
                        ++offset;
                        segStart = offset;
                    } else {
                        //printf("rejected spot#2.2\n");
                        rejected = true;
                        return;
                    }
                } else {
                    //printf("rejected spot#2.3\n");
                    rejected = true;
                    return;
                }
                dfs(trimmedStr, segStart, offset, grandState, singleState, rejected);
                break;
            case SINGLE_STATE_SIGN_DIGESTED:
                if (trimmedStr[offset] == '.') {
                    if (grandState == GRAND_STATE_BEFORE_E) {
                        singleState = SINGLE_STATE_EMPTY_AFTER_DECIMAL_POINT;
                        ++offset;
                    } else {
                        // I don't know why "99e2.5" is not a valid number in this problem
                        //printf("confusing rejected spot#2\n");
                        rejected = true;
                        return;
                    }
                } else if (isdigit(trimmedStr[offset])) {
                    singleState = SINGLE_STATE_DIGIT_BEFORE_DECIMAL_POINT_DIGESTING;
                    ++offset;
                } else {
                    //printf("rejected spot#3\n");
                    rejected = true;
                    return;
                }
                dfs(trimmedStr, segStart, offset, grandState, singleState, rejected);
                break;
            case SINGLE_STATE_DIGIT_BEFORE_DECIMAL_POINT_DIGESTING:
                if (trimmedStr[offset] == '.') {
                    if (grandState == GRAND_STATE_BEFORE_E) {
                        singleState = SINGLE_STATE_EMPTY_AFTER_DECIMAL_POINT;
                        ++offset;
                    } else {
                        // I don't know why "99e2.5" is not a valid number in this problem
                        //printf("confusing rejected spot#3\n");
                        rejected = true;
                        return;
                    }
                } else if (isdigit(trimmedStr[offset])) {
                    singleState = SINGLE_STATE_DIGIT_BEFORE_DECIMAL_POINT_DIGESTING;
                    ++offset;
                } else if (trimmedStr[offset] == 'e') {
                    if (grandState == GRAND_STATE_BEFORE_E) {
                        grandState = GRAND_STATE_AFTER_E;
                        singleState = SINGLE_STATE_EMPTY_BEFORE_DECIMAL_POINT;
                        ++offset;
                    } else {
                        //printf("rejected spot#4\n");
                        rejected = true;
                        return;
                    }
                } else {
                    //printf("rejected spot#5\n");
                    rejected = true;
                    return;
                }
                dfs(trimmedStr, segStart, offset, grandState, singleState, rejected);
                break;
            case SINGLE_STATE_DIGIT_AFTER_DECIMAL_POINT_DIGESTING:
                if (isdigit(trimmedStr[offset])) {
                    singleState = SINGLE_STATE_DIGIT_AFTER_DECIMAL_POINT_DIGESTING;
                    ++offset;
                } else if (trimmedStr[offset] == 'e') {
                    if (grandState == GRAND_STATE_BEFORE_E) {
                        grandState = GRAND_STATE_AFTER_E;
                        singleState = SINGLE_STATE_EMPTY_BEFORE_DECIMAL_POINT;
                        ++offset;
                    } else {
                        //printf("rejected spot#6\n");
                        rejected = true;
                        return;
                    }
                } else {
                    //printf("rejected spot#7\n");
                    rejected = true;
                    return;
                }
                dfs(trimmedStr, segStart, offset, grandState, singleState, rejected);
                break;
            default:
                return;
        }
    }
    
    void ltrim(string& str, const string& chars = "\t\n\v\f\r ") {
        str.erase(0, str.find_first_not_of(chars));
    }
 
    void rtrim(string& str, const string& chars = "\t\n\v\f\r ") {
        str.erase(str.find_last_not_of(chars) + 1);
    }
 
    void trim(std::string& str, const std::string& chars = "\t\n\v\f\r ") {
        rtrim(str, chars);
        ltrim(str, chars);
    }
    
    bool isNumber(string s) {
        /*
        test cases
        "" => false
        ".1." => false
        "." => false
        "46.e3" => true
        "46.e3." => true
        "46.e-3" => true 
        "46.e-3." => false ?????
        "46.e+-3." => false
        "46.e-." => false
        "46.e." => false
        ".e1" => false
        "199." => true
        " -." => false
        " +-." => false
        " -.- " => false
        " -.-8 " => false
        " -.8" => true
        ".9" => true
        ".99" => true
        "8.99" => true
        "e8.99" => false
        "-.9" => true
        "  -1.99e  " => false
        "  -1.99e2.5  " => false ?????
        "  -1.99e2  " => true
        "  -1.99e-2  " => true
        "  .99e-2  " => true
        "  -.99e-2  " => true
        */
        trim(s);
        if (0 == s.length()) {
            return false;
        }
        bool rejected = false;
        int offset = 0, segStart = 0;
        int grandState = GRAND_STATE_BEFORE_E;
        int singleState = SINGLE_STATE_EMPTY_BEFORE_DECIMAL_POINT;
        //printf("The trimmed s == %s\n", s.c_str());
        dfs(s, segStart, offset, grandState, singleState, rejected);
        // wrap up
        if (grandState == GRAND_STATE_AFTER_E) {
            if (singleState != SINGLE_STATE_DIGIT_BEFORE_DECIMAL_POINT_DIGESTING && singleState != SINGLE_STATE_DIGIT_AFTER_DECIMAL_POINT_DIGESTING) {
                //printf("rejected spot#8\n");
                rejected = true;   
            }
        }
        //printf("final rejected: %d\n", rejected);
        return !rejected;
    }
};
