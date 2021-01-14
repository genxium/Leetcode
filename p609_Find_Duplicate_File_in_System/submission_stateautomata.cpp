int const STATE_READING_ACTUAL_PATH_PREFIX = 0;
int const STATE_READING_ACTUAL_PATH = 1;
int const STATE_READING_ACTUAL_CONTENT = 2;

/*
test cases
["root/a 1.txt(abcd) 2.txt(efgh)","root/c 3.txt(abcd)","root/c/d 4.txt(efgh)","root 4.txt(efgh)"]
["root/a 1.txt(abcd) 2.txt(efsfgh)","root/c 3.txt(abdfcd)","root/c/d 4.txt(efggdfh)"]
*/
class Solution {
public:
    vector<vector<string>> findDuplicate(vector<string>& paths) {
        unordered_map<string, vector<string>> counter;
        for (int i = 0; i < paths.size(); ++i) {
            string path = paths[i];
            int s = STATE_READING_ACTUAL_PATH_PREFIX;
            string actualPathPrefix = "";
            string actualPath = "";
            string buff = "";
            for (auto ch : path) {
                if (' ' == ch) {
                    if (buff.empty()) continue;
                    actualPathPrefix = buff;
                    buff = "";
                    s = STATE_READING_ACTUAL_PATH;
                    continue;
                }
                if (')' == ch) {
                    counter[buff].push_back(actualPathPrefix + "/" + actualPath);
                    s = STATE_READING_ACTUAL_PATH;
                    buff = "";
                    continue;
                }
                
                if ('(' == ch) {
                    actualPath = buff;
                    s = STATE_READING_ACTUAL_CONTENT;
                    buff = "";
                    continue;
                }
                
                buff.push_back(ch);
            }
        }
        
        vector<vector<string>> ans;
        for (auto it : counter) {
            vector<string> row;
            if (it.second.size() < 2) continue;
            for (auto p : it.second) row.push_back(p);
            ans.push_back(row);
        }
        
        return ans;
    }
};
