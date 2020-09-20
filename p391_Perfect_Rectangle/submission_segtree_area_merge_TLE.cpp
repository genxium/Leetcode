#define ULL unsigned long
bool TOP = true, BOTTOM = false;
unordered_map<int, int> discretizedXDict; // sortedDedupedIndex -> value

class Rectangle {
public:
  int l, r, t, b;
  int xIndexL, xIndexR;
  
  Rectangle(int aL, int aR, int aB, int aT) {
    l = aL;
    r = aR;
    b = aB;
    t = aT;
  }
};

class SegmentTreeNode {
public:
  int fullCoverAccDiff = 0; // Accumulated diff of this node by "full cover" updates.  
  int leftIndexClosed; // To be initialized upon construction.
  int rightIndexOpen; // To be initialized upon construction.
  int leftXClosed;
  int rightXClosed; // It's BY DESIGN that "rightXClosed = discretizedXDict[rightIndexOpen]" in this problem/solution.
  
  SegmentTreeNode* lChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
  SegmentTreeNode* rChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
  
  SegmentTreeNode(int newSegLeftIndexClosed, int newSegRightIndexOpen) {
    this->leftIndexClosed = newSegLeftIndexClosed;
    this->rightIndexOpen = newSegRightIndexOpen;
    this->leftXClosed = discretizedXDict[newSegLeftIndexClosed];
    this->rightXClosed = (
                        newSegRightIndexOpen >= discretizedXDict.size() 
                        ?
                        INT_MAX 
                        : 
                        discretizedXDict[newSegRightIndexOpen]
                        );
  }
  
  void RangeAdd(int newSegLeftIndexClosed, int newSegRightIndexOpen, pair<int, bool> unifiedYEdge, int level) {
    auto currentRoot = this;
    // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
    if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
    if (newSegLeftIndexClosed >= currentRoot->rightIndexOpen) return;
    if (newSegRightIndexOpen <= currentRoot->leftIndexClosed) return;

    // Snap valid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s.
    newSegLeftIndexClosed = max(newSegLeftIndexClosed, currentRoot->leftIndexClosed);
    newSegRightIndexOpen = min(newSegRightIndexOpen, currentRoot->rightIndexOpen);

    int indentSpaceCount = (level << 1);
    //printf("%*sRangeAdd, [newSegLeftIndexClosed:%d, newSegRightIndexOpen:%d), (y: %d, flag:%s)\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge.first, unifiedYEdge.second == TOP ? "TOP" : "BOTTOM");

    if (newSegLeftIndexClosed <= currentRoot->leftIndexClosed && newSegRightIndexOpen >=  currentRoot->rightIndexOpen) {
      // Proactively stops at "full cover" update.
      if (unifiedYEdge.second == TOP) {
        currentRoot->fullCoverAccDiff += -1;
      } else {
        currentRoot->fullCoverAccDiff += +1;
      }
      return;
    }
    int mid = ((currentRoot->leftIndexClosed + currentRoot->rightIndexOpen) >> 1);
    if (newSegLeftIndexClosed < mid) {
      if (NULL == currentRoot->lChild) {
        currentRoot->lChild = new SegmentTreeNode(currentRoot->leftIndexClosed, mid);
      }
      currentRoot->lChild->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge, level+1);
    }
    
    if (newSegRightIndexOpen > mid) {
      if (NULL == currentRoot->rChild) {
        currentRoot->rChild = new SegmentTreeNode(mid, currentRoot->rightIndexOpen);
      }
      currentRoot->rChild->RangeAdd(newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge, level+1);
    }
  }

  void RangeSum(int targetLeftIndexClosed, int targetRightIndexOpen, int* pResult) {
    auto root = this;
    // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
    if (targetLeftIndexClosed >= targetRightIndexOpen) return;
    if (targetLeftIndexClosed >= root->rightIndexOpen) return;
    if (targetRightIndexOpen <= root->leftIndexClosed) return;

    // Snap valid "[targetLeftIndexClosed, targetRightIndexOpen)"s.
    targetLeftIndexClosed = max(targetLeftIndexClosed, root->leftIndexClosed);
    targetRightIndexOpen = min(targetRightIndexOpen, root->rightIndexOpen);

    if (0 < root->fullCoverAccDiff) {
      (*pResult) = (*pResult) + (root->rightXClosed - root->leftXClosed);
      return;
    }

    if (NULL != root->lChild) {
      root->lChild->RangeSum(targetLeftIndexClosed, targetRightIndexOpen, pResult);
    }
    if (NULL != root->rChild) {
      root->rChild->RangeSum(targetLeftIndexClosed, targetRightIndexOpen, pResult);
    }
  }
};

class Solution {
public:
    bool isRectangleCover(vector<vector<int>>& rectangles) {
      discretizedXDict.clear();
      ULL mergedArea = 0u;
      ULL rawAreaSum = 0u;
      vector<Rectangle> recs;
      int minL = INT_MAX, maxR = INT_MIN, minB = INT_MAX, maxT = INT_MIN;
      for (auto &rectangle : rectangles) {
        Rectangle rec(rectangle[0], rectangle[2], rectangle[1], rectangle[3]);
        recs.push_back(rec);
        rawAreaSum += abs(rec.r - rec.l)*abs(rec.t - rec.b);
        minL = min(minL, rec.l);
        maxR = max(maxR, rec.r);
        minB = min(minB, rec.b);
        maxT = max(maxT, rec.t);
      }
      
      // Discretize and index each of "rec.l & rec.r".
      set<int> sortedDedupedXSet;
      for (auto &rec : recs) {
        sortedDedupedXSet.insert(rec.l);
        sortedDedupedXSet.insert(rec.r);
      }
      vector<int> sortedDedupedXList;
      for (auto it = sortedDedupedXSet.begin(); it != sortedDedupedXSet.end(); ++it) {
        sortedDedupedXList.push_back(*it);
      }
      
      for (auto &rec : recs) {
        auto itL = lower_bound(sortedDedupedXList.begin(), sortedDedupedXList.end(), rec.l);
        rec.xIndexL = (int)(itL - sortedDedupedXList.begin());
        discretizedXDict[rec.xIndexL] = rec.l;
                
        auto itR = lower_bound(sortedDedupedXList.begin(), sortedDedupedXList.end(), rec.r);
        rec.xIndexR = (int)(itR - sortedDedupedXList.begin());
        discretizedXDict[rec.xIndexR] = rec.r;
      }
          
      //printf("After the traversal, discretizedXDict.size() == %zu, sortedDedupedXList.size() == %lu, they should be the same.\n", discretizedXDict.size(), sortedDedupedXList.size());
      SegmentTreeNode* root = new SegmentTreeNode(0, sortedDedupedXList.size());
      
      vector<vector<int>> sortedYEdgeList;
      for (auto &rec : recs) {
        sortedYEdgeList.push_back({rec.b, BOTTOM, rec.xIndexL, rec.xIndexR});
        sortedYEdgeList.push_back({rec.t, TOP, rec.xIndexL, rec.xIndexR});
      }
      sort(sortedYEdgeList.begin(), sortedYEdgeList.end(), less<vector<int>>());
      
      // printf("sortedYEdgeList.size() == %d\n", sortedYEdgeList.size());
      
      int lastY = INT_MAX;
      mergedArea = 0; // reset the answer
      for (auto &yEdge : sortedYEdgeList) {
        int currentY = yEdge[0];
        if (INT_MAX != lastY) {
          int activeBottomLengthSum = 0;
          /*
          Note that We're computing "activeBottomLengthSum" regardless of "yEdge[2], yEdge[3]".
          */
          root->RangeSum(0, sortedDedupedXList.size(), &activeBottomLengthSum);
          mergedArea += (ULL)(currentY-lastY)*(ULL)activeBottomLengthSum;
        }
        root->RangeAdd(yEdge[2], yEdge[3], {yEdge[0], yEdge[1]}, 0);
        lastY = currentY;
      }
      int hullArea = abs(maxT - minB)*abs(maxR - minL);
      //printf("mergedArea: %d, hullArea: %d, rawAreaSum: %d\n", mergedArea, hullArea, rawAreaSum);
      if (mergedArea != rawAreaSum) return false;
      if (mergedArea != hullArea) return false;
      return true;
    }
};
