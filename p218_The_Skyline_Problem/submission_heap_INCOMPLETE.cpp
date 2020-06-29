#define INVALID -1

class Building {
public:
    int l, r, h;
    Building():l(INVALID), r(INVALID), h(INVALID) {
        
    }
    
    Building(int aL, int aR, int aH):l(INVALID), r(INVALID), h(INVALID) {
        l = aL;
        r = aR;
        h = aH;
    }
    
    void reset() {
        l = INVALID;
        r = INVALID;
        h = INVALID;
    }
};

class BuildingCompareVec {
public:
    bool operator() (Building const &lhs, Building const &rhs) const {
        if (lhs.l != rhs.l) {
            return (lhs.l < rhs.l);
        } else if (lhs.h != rhs.h) {
            return (lhs.h > rhs.h); 
        } else {
            return (lhs.r < rhs.r);
        }
    }
};

// Note that priority_queue by default is a max-heap
class BuildingCompareHeap {
public:
    bool operator() (Building const &lhs, Building const &rhs) const {
        if (lhs.h != rhs.h) {
            return (lhs.h < rhs.h);
        } else if (lhs.r != rhs.r) {
            return (lhs.r > rhs.r); 
        } else {
            return (lhs.l < rhs.l);
        }
    }
};

class Solution {
public:
    bool extendOrClose(Building* pNextBuilding, Building &currentOpenClosure, vector<Building> &ansBuffer, priority_queue<Building, vector<Building>, BuildingCompareHeap> &pq) {
        
        // Returns false if the same "pNextBuilding"(including NULL) should be re-evaluated for the updated "justClosedClosure" & "currentOpenClosure".
               
        if (INVALID == currentOpenClosure.l) {
            if (NULL != pNextBuilding) {
                // lazy init
                currentOpenClosure.l = pNextBuilding->l;
                currentOpenClosure.r = pNextBuilding->r;
                currentOpenClosure.h = pNextBuilding->h;
            } else {
                // an overall end of iteration
            }
            return true;
        } 
            
        bool shouldExtendTheCurrentOpenClosure = false;
        bool shouldCloseTheCurrentOpenClosure = false;
                
        if (NULL == pNextBuilding) {
            shouldCloseTheCurrentOpenClosure = true;
        } else {
            Building nextBuilding = *pNextBuilding;
            if (nextBuilding.l > currentOpenClosure.r) {
                shouldCloseTheCurrentOpenClosure = true;
            } else {
                // nextBuilding.l <= currentOpenClosure.r
                if (nextBuilding.h > currentOpenClosure.h) {
                    // here comes a trick, after using "BuildingCompareVec", we've guaranteed that "nextBuilding.h > currentOpenClosure.h" during iteration, it must be true that "nextBuilding.l != currentOpenClosure.l", otherwise "nextBuilding" should've come earlier 
                    shouldCloseTheCurrentOpenClosure = true;
                } else {
                    // nextBuilding.l <= currentOpenClosure.r && nextBuilding.h <= currentOpenClosure.h
                    if (nextBuilding.h == currentOpenClosure.h) {
                        shouldExtendTheCurrentOpenClosure = true;
                    } else {
                        // nextBuilding.l <= currentOpenClosure.r && nextBuilding.h < currentOpenClosure.h
                        pq.push(nextBuilding);                            
                        return true;
                    }
                }
            }
        }
        if (shouldExtendTheCurrentOpenClosure) {
            currentOpenClosure.r = (currentOpenClosure.r > pNextBuilding->r ? currentOpenClosure.r : pNextBuilding->r);
            return true;
        } else {
            if (shouldCloseTheCurrentOpenClosure) {
                Building snappedRightHalf; // the snapped right-half by "pNextBuilding"
                if (NULL != pNextBuilding && pNextBuilding->r < currentOpenClosure.r) {
                    snappedRightHalf.l = pNextBuilding->r;
                    snappedRightHalf.r = currentOpenClosure.r;
                    snappedRightHalf.h = currentOpenClosure.h; 
                }
                
                currentOpenClosure.r = (
                    NULL == pNextBuilding 
                    ? 
                    currentOpenClosure.r 
                    : 
                    (currentOpenClosure.r < pNextBuilding->l ? currentOpenClosure.r : pNextBuilding->l) // snapped by higher building
                );     
                Building toPushClosure(currentOpenClosure.l, currentOpenClosure.r, currentOpenClosure.h);
                //printf("Closed toPushClosure(l:%d, r:%d, h:%d)\n", toPushClosure.l, toPushClosure.r, toPushClosure.h);
                ansBuffer.push_back(toPushClosure);
                
                currentOpenClosure.reset(); // would be init again by "rotation from pq item" or "next iteration"
                
                while (false == pq.empty()) {
                    Building nextOpenClosureCandidate = pq.top();
                    if (NULL != pNextBuilding && (nextOpenClosureCandidate.h < pNextBuilding->h && nextOpenClosureCandidate.l >= pNextBuilding->l)) {
                        // NOT a valid candidate now, but could be in later iterations, consider "test case #9"
                        break;
                    }
                    pq.pop();
                    // recall that "item" in pq must suffice "item.l <= toPushClosure.r && item.h < toPushClosure.h"
                    
                    //printf("\tevaluating nextOpenClosureCandidate(l:%d, r:%d, h:%d)\n", nextOpenClosureCandidate.l, nextOpenClosureCandidate.r, nextOpenClosureCandidate.h);
                    if (nextOpenClosureCandidate.r <= toPushClosure.r) {
                        // NOT a valid candidate now, and won't be in later iterations
                        continue;
                    }
                    // rotate "currentOpenClosure"
                    nextOpenClosureCandidate.l = toPushClosure.r;
                    //printf("\twill rotate currentOpenClosure by nextOpenClosureCandidate(l:%d, r:%d, h:%d)\n", nextOpenClosureCandidate.l, nextOpenClosureCandidate.r, nextOpenClosureCandidate.h);

                    currentOpenClosure.l = nextOpenClosureCandidate.l;
                    currentOpenClosure.r = nextOpenClosureCandidate.r;
                    currentOpenClosure.h = nextOpenClosureCandidate.h;
                    break;
                }
                
                if (INVALID != snappedRightHalf.l) {
                    pq.push(snappedRightHalf); // [WARNING] "snappedRightHalf" must be push into heap AFTER THE LOTTERY, consider "test case #7"                         
                }
                return false;
            }
        }
        return true;
    }
    
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        /*
        test case #0
        [[2,9,10],[3,7,15],[5,12,12],[15,20,10],[19,24,8]]
        
        test case #1
        []
        
        test case #2
        [[2,9,10]]
        
        test case #3
        [[2,9,10],[3,7,15],[5,8,11],[5,12,12],[15,20,10],[19,24,8]]
        
        test case #4
        [[2,9,10],[3,7,15],[5,8,20],[5,12,12],[15,20,10],[19,24,8]]
        
        test case #5
        [[1,2,1],[1,2,2],[1,2,3]]
        
        test case #6
        [[2,9,10],[3,7,15],[3,8,16],[5,8,11],[5,12,12],[15,20,10],[19,24,8]]
        
        test case #7
        [[6,100,41],[7,15,70]]

        test case #8
        [[2,4,70],[3,8,30],[6,100,41],[7,15,70],[10,30,102],[15,25,76],[60,80,91],[70,90,72],[85,120,59]]
        
        test case #9
        [[2,90,10],[3,7,15],[30,70,15],[50,80,20]]
        */
        vector<vector<int>> ans;
        if (0 == buildings.size()) return ans;
        vector<Building> buildingList;
        priority_queue<Building, vector<Building>, BuildingCompareHeap> pq;
        for (auto single : buildings) {
            Building item(single[0], single[1], single[2]);
            buildingList.push_back(item);
        }
        sort(buildingList.begin(), buildingList.end(), BuildingCompareVec());
        /*
        for (auto &b : buildingList) {
            printf("\tb(l:%d, r:%d, h:%d)\n", b.l, b.r, b.h);
        }
        */
            
        Building currentOpenClosure, justClosedClosure;
        vector<Building> ansBuffer;

        auto it = buildingList.begin();
        while (it != buildingList.end()) {
            Building single = (*it);
            bool shouldMoveIterator = extendOrClose(&single, currentOpenClosure, ansBuffer, pq);
            if (shouldMoveIterator) {
                ++it;
            }
        }
        while(false == extendOrClose(NULL, currentOpenClosure, ansBuffer, pq));
        
        int prevSegR = INT_MAX;
        for (auto single : ansBuffer) {
            if (single.l > prevSegR) {
                ans.push_back({prevSegR, 0});
            }
            ans.push_back({single.l, single.h});
            prevSegR = single.r;
        }
        ans.push_back({prevSegR, 0});
        return ans;
    }
};
