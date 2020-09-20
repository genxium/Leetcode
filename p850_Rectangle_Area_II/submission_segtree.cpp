#define MOD 1000000007u
#define ULL unsigned long
bool TOP = true, BOTTOM = false;
ULL mergedArea = 0u;

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
};

void RangeAdd(SegmentTreeNode* currentRoot, int newSegLeftIndexClosed, int newSegRightIndexOpen, pair<int, bool> unifiedYEdge, int level) {
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
  if (NULL != currentRoot->lChild) {
    RangeAdd(currentRoot->lChild, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge, level+1);
  }
  if (NULL != currentRoot->rChild) {
    RangeAdd(currentRoot->rChild, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge, level+1);
  }
}

void RangeSum(SegmentTreeNode* root, int targetLeftIndexClosed, int targetRightIndexOpen, int* pResult) {
  // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
  if (targetLeftIndexClosed >= targetRightIndexOpen) return;
  if (targetLeftIndexClosed >= root->rightIndexOpen) return;
  if (targetRightIndexOpen <= root->leftIndexClosed) return;
  
  // Snap valid "[targetLeftIndexClosed, targetRightIndexOpen)"s.
  targetLeftIndexClosed = max(targetLeftIndexClosed, root->leftIndexClosed);
  targetRightIndexOpen = min(targetRightIndexOpen, root->rightIndexOpen);
    
  if (0 < root->fullCoverAccDiff) {
    (*pResult) = (*pResult) + (root->rightXClosed - root->leftXClosed);
    (*pResult) %= MOD;
    return;
  }

  if (NULL != root->lChild) {
    RangeSum(root->lChild, targetLeftIndexClosed, targetRightIndexOpen, pResult);
  }
  if (NULL != root->rChild) {
    RangeSum(root->rChild, targetLeftIndexClosed, targetRightIndexOpen, pResult);
  }
}

SegmentTreeNode* createBlankSegmentTree(int newSegLeftIndexClosed, int newSegRightIndexOpen, map<int, int> &discretizedXDict) {
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) {
    return NULL;
  } 

  SegmentTreeNode* root = new SegmentTreeNode(); 
  root->leftIndexClosed = newSegLeftIndexClosed;
  root->rightIndexOpen = newSegRightIndexOpen;
  root->leftXClosed = discretizedXDict[newSegLeftIndexClosed];
  root->rightXClosed = (
                        newSegRightIndexOpen >= discretizedXDict.size() 
                        ?
                        INT_MAX 
                        : 
                        discretizedXDict[newSegRightIndexOpen]
                        );
  
  //printf("createBlankSegmentTree, newSegLeftIndexClosed:%d, newSegRightIndexOpen:%d, root->leftXClosed:%d, root->rightXClosed:%d.\n", newSegLeftIndexClosed, newSegRightIndexOpen, root->leftXClosed, root->rightXClosed);

  int mid = ((newSegLeftIndexClosed + newSegRightIndexOpen) >> 1);
  if (mid > newSegLeftIndexClosed) {
    root->lChild = createBlankSegmentTree(newSegLeftIndexClosed, mid, discretizedXDict);
    if (mid < newSegRightIndexOpen) {
      root->rChild = createBlankSegmentTree(mid, newSegRightIndexOpen, discretizedXDict);
    } else {
      root->rChild = NULL;
    }
  } else {
    root->lChild = NULL;
    root->rChild = NULL;
  }
  return root;
}

class Solution {
public:
    int rectangleArea(vector<vector<int>>& rectangles) {
      vector<Rectangle> recs;
      for (auto &rectangle : rectangles) {
        Rectangle rec(rectangle[0], rectangle[2], rectangle[1], rectangle[3]);
        recs.push_back(rec);
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
      
      map<int, int> discretizedXDict; // sortedDedupedIndex -> value
      for (auto &rec : recs) {
        auto itL = lower_bound(sortedDedupedXList.begin(), sortedDedupedXList.end(), rec.l);
        rec.xIndexL = (int)(itL - sortedDedupedXList.begin());
        discretizedXDict[rec.xIndexL] = rec.l;
                
        auto itR = lower_bound(sortedDedupedXList.begin(), sortedDedupedXList.end(), rec.r);
        rec.xIndexR = (int)(itR - sortedDedupedXList.begin());
        discretizedXDict[rec.xIndexR] = rec.r;
      }
          
      //printf("After the traversal, discretizedXDict.size() == %zu, sortedDedupedXList.size() == %lu, they should be the same.\n", discretizedXDict.size(), sortedDedupedXList.size());
      SegmentTreeNode* root = createBlankSegmentTree(0, sortedDedupedXList.size(), discretizedXDict);
      
      vector<vector<int>> sortedYEdgeList;
      for (auto &rec : recs) {
        sortedYEdgeList.push_back({rec.b, BOTTOM, rec.xIndexL, rec.xIndexR});
        sortedYEdgeList.push_back({rec.t, TOP, rec.xIndexL, rec.xIndexR});
      }
      sort(sortedYEdgeList.begin(), sortedYEdgeList.end(), less<vector<int>>());
      
      int lastY = INT_MAX;
      mergedArea = 0; // reset the answer
      for (auto &yEdge : sortedYEdgeList) {
        int currentY = yEdge[0];
        if (INT_MAX != lastY) {
          int activeBottomLengthSum = 0;
          /*
          Note that we're computing "activeBottomLengthSum" regardless of "yEdge[2], yEdge[3]".
          */
          RangeSum(root, 0, sortedDedupedXList.size(), &activeBottomLengthSum);
          mergedArea += (ULL)(currentY-lastY)*(ULL)activeBottomLengthSum;
          mergedArea %= MOD;
        }
        RangeAdd(root, yEdge[2], yEdge[3], {yEdge[0], yEdge[1]}, 0);
        lastY = currentY;
      }
      return (int)mergedArea;
    }
};
