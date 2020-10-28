int const INVALID = -1;

class DcLinkNode {
public:
  char ball; 
  DcLinkNode *l, *r;
  DcLinkNode(): ball(INVALID), l(NULL), r(NULL) {
  }
  DcLinkNode(char aBall): l(NULL), r(NULL) {
    this->ball = aBall;
  }
};

/*
test case#1
"WRRBBW"
"RB"

test case#2
"WWRRBBWW"
"WRBRW"

test case#3
"RRWWRRBBRR"
"WB"
// the answer should be 2, with steps
// RRWWRRBBRR -> RRWWRRBBR[W]R -> RRWWRRBB[B]RWR -> RRWWRRRWR -> RRWWWR -> RRR -> empty
*/
 
class Solution {
public:
    string toString(DcLinkNode* grandHead) {
        if (!grandHead) return "";
        string ans = "";
        DcLinkNode* cur = grandHead->r;
        while (cur) {
            ans.push_back(cur->ball);
            cur = cur->r;
        }
        return ans;
    }
    
    int countConsecutiveSameColor(DcLinkNode* right) {
        if (!right) return 0;
        int toRet = 0;
        DcLinkNode *cur = right;
        while (cur && cur->ball == right->ball) {
            ++toRet;
            cur = cur->l;
        }
        return toRet;
    }
    
    void cover(DcLinkNode* c) {
        if (c->r) {
            c->r->l = c->l;            
        }
        if (c->l) {
            c->l->r = c->r;            
        }
    } 

    void uncover(DcLinkNode* c) {
        if (c->r) {
            c->r->l = c;            
        }
        if (c->l) {
            c->l->r = c;            
        }
    }
    
    DcLinkNode* coverConsecutiveSameColor(DcLinkNode* right, int count, int indent) {
        // printf("%*scoverConsecutiveSameColor right:%p {ball:%c}\n", indent, "", right, right->ball);
        DcLinkNode* cur = right, *toRet = right;
        for (int i = 0; i < count; ++i) {
            // printf("\t%*sprocessing cur:%p {ball:%c}\n", indent, "", cur, cur->ball);
            DcLinkNode* cachedLeft = cur->l;
            cover(cur);
            cur = cachedLeft;                
        }
        return toRet;
    }
    
    void uncoverConsecutiveSameColor(DcLinkNode* handle, int count, int indent) {
        /*
        [WARNING]
        
        The direction of uncovering a 3-consecutive-same-color-tuple must be the same as that of the covering, otherwise we'll get incorrect pointer behaviours.
        */
        // printf("%*suncoverConsecutiveSameColor handle:%p {ball:%c}\n", indent, "handle, handle->ball);
        DcLinkNode* cur = handle;
        for (int i = 0; i < count; ++i) {
            // printf("\t%*sprocessing cur:%p {ball:%c}\n", indent, "", cur, cur->ball);
            DcLinkNode* cachedNeighbour = cur->l;
            uncover(cur);
            cur = cachedNeighbour;
        }
    }
    
    int solve(DcLinkNode* grandHeadTable, DcLinkNode* grandHeadAtHand, int stepCount, unordered_map<string, unordered_map<string, int>> &cache) {
        string tableStr = toString(grandHeadTable);
        string atHandStr = toString(grandHeadAtHand);
        if (cache.find(tableStr) != cache.end() && cache[tableStr].find(atHandStr) != cache[tableStr].end()) {
            return cache[tableStr][atHandStr];
        }
        
        if (!grandHeadTable->r) {
            return (cache[tableStr][atHandStr] = 0);
        }
        
        if (!grandHeadAtHand->r) {
            return INVALID;
        }
        
        int indent = (stepCount << 1);
        int candidateCacheVal = INVALID;
        DcLinkNode* ballToUse = grandHeadAtHand->r;
        while (ballToUse) {
            // printf("%*sTrying ballToUse {ball:%c} at stepCount:%d\n", indent, "", ballToUse->ball, stepCount);
            DcLinkNode *posToAppend = grandHeadTable;
            while (posToAppend) {
                
                // Always insert into the rightmost of a consecutive of same color balls.
                DcLinkNode* cachedRightInTable = posToAppend->r;
                DcLinkNode* copiedBallToUse = new DcLinkNode(ballToUse->ball);
                if (cachedRightInTable) {
                    posToAppend->r = copiedBallToUse;
                    copiedBallToUse->l = posToAppend;
                    copiedBallToUse->r = cachedRightInTable;
                    cachedRightInTable->l = copiedBallToUse;
                } else {
                    posToAppend->r = copiedBallToUse;
                    copiedBallToUse->l = posToAppend;
                }

                stack<pair<DcLinkNode*, int>> toUncoverHandles;
                DcLinkNode* candidate = copiedBallToUse;
                // covering
                while (true) {
                    int countToLeft = countConsecutiveSameColor(candidate);
                    if (countToLeft >= 3) {
                        int anotherCountToLeft = countConsecutiveSameColor(candidate->r);
                        if (anotherCountToLeft >= 4) {
                            candidate = candidate->r;
                            countToLeft = anotherCountToLeft;
                        }
                        DcLinkNode* toUncoverHandle = coverConsecutiveSameColor(candidate, countToLeft, indent); 
                        // printf("%*sCaching toUncoverHandle:%p {ball:%c} with countToLeft:%d\n", indent, "", toUncoverHandle, toUncoverHandle->ball, countToLeft);
                        toUncoverHandles.push({toUncoverHandle, countToLeft});
                        candidate = candidate->r;
                        if (!candidate) break;
                        countToLeft = countConsecutiveSameColor(candidate);
                        if (countToLeft < 3) {
                            /*
                            e.g.                                
                            RGGGRR
                               ^
                              copiedBallToUse

                            RRR
                             ^
                             candidate
                            */
                            candidate = candidate->r;
                        }
                    } else {
                        break;
                    }
                }
                cover(ballToUse);

                // recursive search
                int minRemainingSteps = solve(grandHeadTable, grandHeadAtHand, stepCount+1, cache);
                if (INVALID != minRemainingSteps) {
                    if (INVALID == candidateCacheVal) {
                        candidateCacheVal = 1+minRemainingSteps;
                    } else {
                        candidateCacheVal = min(candidateCacheVal, 1+minRemainingSteps);
                    }
                }
                
                // uncovering (backtracking)
                uncover(ballToUse);
                while (!toUncoverHandles.empty()) {
                    auto top = toUncoverHandles.top(); toUncoverHandles.pop();
                    auto toUncoverHandle = top.first;
                    auto count = top.second;
                    uncoverConsecutiveSameColor(toUncoverHandle, count, indent);
                }
                // dealloc copied node
                cover(copiedBallToUse); // shall cover it before the actuall dealloc
                delete copiedBallToUse;
            
                posToAppend = posToAppend->r;
            }
            ballToUse = ballToUse->r;
        }
        
        return (cache[tableStr][atHandStr] = candidateCacheVal); // Including "INVALID"
    }
    
    int findMinStep(string table, string hand) {
        DcLinkNode* grandHeadTable = new DcLinkNode(INVALID);
        DcLinkNode* curTable = grandHeadTable;
        for (auto ch : table) {
            DcLinkNode* newNode = new DcLinkNode(ch);
            curTable->r = newNode;
            newNode->l = curTable;
            curTable = newNode;
        }
        
        DcLinkNode* grandHeadAtHand = new DcLinkNode(INVALID);
        DcLinkNode* curAtHand = grandHeadAtHand;
        for (auto ch : hand) {
            DcLinkNode* newNode = new DcLinkNode(ch);
            curAtHand->r = newNode;
            newNode->l = curAtHand;
            curAtHand = newNode;
        }
        
        unordered_map<string, unordered_map<string, int>> cache;
        solve(grandHeadTable, grandHeadAtHand, 0, cache);
        return cache[table][hand];
    }
};
