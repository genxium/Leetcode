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

// Note that priority_queue by default is a max-heap
class BuildingCompareHeap {
public:
    int operator() (Building const &lhs, Building const &rhs) const {
        if (lhs.h != rhs.h) {
            return (lhs.h - rhs.h);
        } else if (lhs.r != rhs.r) {
            return (rhs.r - lhs.r); 
        } else {
            return (lhs.l - rhs.l);
        }
    }
};

class Solution {
public:
    bool extendOrClose(Building* pNextBuilding, Building &currentOpenClosure, vector<Building> ansBuffer, priority_queue<Building, vector<Building>, BuildingCompareHeap> &pq) {
        
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
                currentOpenClosure.r = (
                    NULL == pNextBuilding 
                    ? 
                    currentOpenClosure.r 
                    : 
                    (currentOpenClosure.r < pNextBuilding->l ? currentOpenClosure.r : pNextBuilding->l) // snapped by higher building
                );     
                Building toPushClosure(currentOpenClosure.l, currentOpenClosure.r, currentOpenClosure.h);
                printf("Closed toPushClosure(l:%d, r:%d, h:%d)\n", toPushClosure.l, toPushClosure.r, toPushClosure.h);
                ansBuffer.push_back(toPushClosure);
                
                currentOpenClosure.reset(); // would be init again by "rotation from pq item" or "next iteration"
                
                while (false == pq.empty()) {
                    Building nextOpenClosureCandidate = pq.top();
                    pq.pop();
                    // recall that "item" in pq must suffice "item.l <= currentOpenClosure.r && item.h < currentOpenClosure.h"
                    if (nextOpenClosureCandidate.r <= toPushClosure.r) {
                        // NOT a valid candidate now, and won't be in later iterations
                        continue;
                    }
                    // rotate "currentOpenClosure"
                    nextOpenClosureCandidate.l = (nextOpenClosureCandidate.l > toPushClosure.r ? nextOpenClosureCandidate.l : toPushClosure.r);
                    
                    currentOpenClosure.l = nextOpenClosureCandidate.l;
                    currentOpenClosure.r = nextOpenClosureCandidate.r;
                    currentOpenClosure.h = nextOpenClosureCandidate.h;
                }
                return false;
            }
        }
        return true;
    }
    
    vector<vector<int>> getSkyline(vector<vector<int>>& buildings) {
        vector<vector<int>> ans;
        vector<Building> buildingList;
        priority_queue<Building, vector<Building>, BuildingCompareHeap> pq;
        for (auto single : buildings) {
            Building item(single[0], single[1], single[2]);
            buildingList.push_back(item);
        }
            
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
        
        for (auto single : ansBuffer) {
            vector<int> seg{single.l, single.h};
            ans.push_back(seg);
        }
        return ans;
    }
};
