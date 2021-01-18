bool const debug = false;
int const INVALID = -1;
int const START = 0, END = 1;
typedef vector<int> VI;

class Solution {
public:
    VI parseLog(string &log) {
        string buff = "";
        int id = INVALID;
        int type = INVALID;
        for (auto ch : log) {
            switch (ch) {
                case ':': {
                    if (INVALID == id) {
                        id = stoi(buff);
                        buff = "";
                    } else {
                        if (0 == buff.compare("start")) {
                            type = START;
                        } else {
                            type = END;
                        }
                        buff = "";
                    }
                }
                break;
                default:
                    buff.push_back(ch);
                break;
            }
        }
        
        int timestamp = stoi(buff);
        return {id, type, timestamp};
    }
    
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> ans(n, 0);
        stack<VI> stk;
        
        for (auto& log : logs) {
            VI tmp = parseLog(log);
            if (tmp[1] == END) {
                auto t = stk.top(); stk.pop();
                int id = tmp[0];
                int totDuration = tmp[2]-t[2]+1; 
                int incExclusiveDuration = totDuration-t[3];
                ans[id] += incExclusiveDuration;
                if (!stk.empty()) {
                    // Consider the case "AB----BC-----CA"
                    stk.top()[3] += totDuration;                 
                }
            } else {
                // {tmp with accDeductionDuration}
                stk.push({tmp[0], tmp[1], tmp[2], 0});
            }
        }
        return ans;
    }
};
