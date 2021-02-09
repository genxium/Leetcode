bool debug = false;

int const STATE_IDLE = 0;
int const STATE_CMT_ST_1 = 1;
int const STATE_IN_LINE_CMT = 2;
int const STATE_IN_BLOCK_CMT = 3;
int const STATE_BLOCK_CMT_ED_1 = 4;

class Solution {
public:
    vector<string> removeComments(vector<string>& source) {
        int state = STATE_IDLE;
        
        vector<string> ans;
        for (int i = 0; i < source.size(); ++i) {
            string& line = source[i];
            bool startedWithMultilineBlockCmt = false;
            int commentSt = -1;
            if (state == STATE_IN_BLOCK_CMT) {
                commentSt = 0;
                startedWithMultilineBlockCmt = true;
            } else {
                state = STATE_IDLE;
            }
            if (debug) {
                printf("i:%d, state:%d, commentSt:%d\n", i, state, commentSt);
            }
            vector<pair<int, int>> toRemoveIntervals;
            for (int j = 0; j < line.length(); ++j) {
                char ch = line[j];
                switch (state) {
                    case STATE_IDLE:
                        if (ch == '/') {
                            state = STATE_CMT_ST_1;
                        }
                        break;
                    case STATE_CMT_ST_1:
                        if (ch == '/') {
                            state = STATE_IN_LINE_CMT;
                            commentSt = j-1;
                        } else if (ch == '*') {
                            state = STATE_IN_BLOCK_CMT;
                            commentSt = j-1;
                        } else {
                            state = STATE_IDLE;
                        }
                        break;
                    case STATE_IN_BLOCK_CMT:
                        if (ch == '*') {
                            state = STATE_BLOCK_CMT_ED_1;
                        }
                        break;
                    case STATE_BLOCK_CMT_ED_1:
                        if (ch == '/') {
                            state = STATE_IDLE;
                            toRemoveIntervals.push_back({commentSt, j+1});
                        } else if (ch == '*') {
                            // keep 
                        } else {
                            state = STATE_IN_BLOCK_CMT;
                        }
                        break;
                }
            }
            
            if (state == STATE_IN_LINE_CMT || state == STATE_IN_BLOCK_CMT) {
                if (commentSt != -1) {
                    toRemoveIntervals.push_back({commentSt, line.length()});
                }
            }
            
            if (debug) {
                printf("\ttoRemoveIntervals has\n");
                for (auto &p : toRemoveIntervals) {
                    printf("\t[%d, %d)\n", p.first, p.second);
                }
            }

            bool shouldAdhereToLastRow = (true == startedWithMultilineBlockCmt && false == toRemoveIntervals.empty());
            string newRow = "";
            string *pRow = shouldAdhereToLastRow ? &(ans.back()) : &newRow;
            
            auto it = toRemoveIntervals.begin(); 
            for (int j = 0; j < line.length(); ) {
                if (it != toRemoveIntervals.end()) {
                    if (j >= it->first && j < it->second) {
                        ++j;
                        continue;
                    } else {
                        if (j >= it->second) {
                            ++it;
                            continue;
                        }
                        pRow->push_back(line[j++]);
                        
                    } 
                } else {
                    pRow->push_back(line[j++]);
                }
            }
            
            if (false == shouldAdhereToLastRow && !pRow->empty()) ans.push_back(*pRow);
        }
        
        return ans;
    }
};
