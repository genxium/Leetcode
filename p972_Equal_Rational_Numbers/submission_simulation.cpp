bool debug = false;
typedef vector<int> VI;
/*
test cases
"0.(52)"
"0.5(25)"
"0.1666(6)"
"0.166(66)"
"0.9(9)"
"1."
"0.(9)"
"1."
"99.(9)"
"100."
"99.999(9)"
"100."
"99.999(99999)"
"100."
"0.(0)"
"0"
"0.000000000000(0000000)"
"0"
"0.100000000000(000000)"
"0.1"
"0.100000000000(999999)"
"0.1"
"350.(111)"
"350.(11)"
"350.(123)"
"350.(12312)"
*/
class Solution {
public:
    VI addOne(VI& input) {
        VI out;
        int inc = 1;
        for (int i = input.size()-1; i >= 0; --i) {
            int v = input[i]+inc;
            out.push_back(v%10);
            inc = v/10;
        }
        if (inc > 0) out.push_back(inc);
        reverse(out.begin(), out.end());
        if (debug) {
            for (int i = 0; i < input.size(); ++i) {
                printf("%d", input[i]);
            }
            printf(" added 1 to be ");
            for (int i = 0; i < out.size(); ++i) {
                printf("%d", out[i]);
            }
            puts("");
        }
        return out;
    }
    
    void roundRepeating0(vector<VI>& input) {
        if (3 != input.size()) return;
        VI reference0 = {0};
        if (!isSameRepeatingPattern(reference0, input[2])) return;
        input.pop_back();
    }
    
    void roundRepeating9(vector<VI>& input) {
        if (3 != input.size()) return;
        VI reference9 = {9};
        if (!isSameRepeatingPattern(reference9, input[2])) return;
        input.pop_back();
        if (0 < input[1].size()) {
            VI newPart1 = addOne(input[1]); 
            if (newPart1.size() > input[1].size()) {
                // the xxx.99999(9) case
                input.pop_back(); 
                VI newPart0 = addOne(input[0]);
                input[0] = newPart0;
            } else {
                input[1] = newPart1;
            }
        } else {
            // the xxx.(9) case
            input.pop_back(); 
            VI newPart0 = addOne(input[0]);
            input[0] = newPart0;
        }
    }
    
    vector<VI> split(string& s) {
        vector<VI> res;
        VI buff;
        for (char c : s) {
            if ('.' == c || '(' == c || ')' == c) {
                res.push_back(buff); // buff could be empty at this point
                buff.clear();
            } else {
                buff.push_back(c-'0');
            }
        }
        if (res.empty() || !buff.empty()) res.push_back(buff); // don't push back empty decimal parts
        roundRepeating9(res);
        roundRepeating0(res);
        if (2 == res.size()) {
            // delete trailing 0s for part1
            while (!res[1].empty() && 0 == res[1].back()) res[1].pop_back();
            if (res[1].empty()) res.pop_back();
        }
        return res;
    }

    void shift(VI& part1, VI& part2, int slots) {
        int i = 0, n = part2.size();
        while (i < slots) {
            part1.push_back(part2[i%n]);
            ++i;
        }
        VI newPart2; // shifted repeating part
        for (int j = 0; j < n; ++j) newPart2.push_back(part2[(i+j)%n]);
        part2 = newPart2;
    }
    
    bool isSameRepeatingPattern(VI& lhs, VI& rhs) {
        if (lhs == rhs) return true;
        if (lhs.size() == rhs.size()) return false; // because strict equality was just checked
        if (lhs.size() > rhs.size()) swap(lhs, rhs);
        int n1 = lhs.size(), n2 = rhs.size(); // n2 >= n1
        int extendedN = n1; 
        while (extendedN < n2) extendedN += n1; // handles the "same repeating digit" case
        for (int i = 0; i < extendedN; ++i) {
            if (rhs[i%n2] != lhs[i%n1]) return false;
        }
        return true;
    }
    
    bool isRationalEqual(string s, string t) {
        vector<VI> sl = split(s);
        vector<VI> tl = split(t);
        if (debug) {
            printSplitted(sl);
            printSplitted(tl);
        }
        if (sl.size() != tl.size()) return false;
        if (sl[0] != tl[0]) return false;
        if (1 == sl.size()) return true;
        if (sl[1].size() != tl[1].size()) {
            if (3 != sl.size()) return false; // no repeating part, definitely not equal
            // align them
            int slots = abs((int)sl[1].size() - (int)tl[1].size());
            if (sl[1].size() < tl[1].size()) {
                shift(sl[1], sl[2], slots);
            } else {
                shift(tl[1], tl[2], slots);
            }
        }
        if (debug) {
            printf("After shifting\n");
            printSplitted(sl);
            printSplitted(tl);
        }
        if (sl[1] != tl[1]) return false;
        if (2 == sl.size()) return true;
        return isSameRepeatingPattern(sl[2], tl[2]);
    }
    
    void printSplitted(vector<VI>& input) {
        for (int i = 0; i < input[0].size(); ++i) {
            printf("%d", input[0][i]);
        }
        if (input.size() >= 2) {
            printf(".");
            for (int i = 0; i < input[1].size(); ++i) {
                printf("%d", input[1][i]);
            }
        }
        if (input.size() >= 3) {
            printf("(");
            for (int i = 0; i < input[2].size(); ++i) {
                printf("%d", input[2][i]);
            }
            printf(")");
        }
        puts("");
    }
};
