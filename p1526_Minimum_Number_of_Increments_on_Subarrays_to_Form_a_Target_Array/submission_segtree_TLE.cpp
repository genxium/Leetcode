#define MOD 1000000007u
#define ULL unsigned long
bool TOP = true, BOTTOM = false;

class Rectangle {
public:
  int l, r, t, b;
  
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
  
  // Not full cover
  int mid = ((currentRoot->leftIndexClosed + currentRoot->rightIndexOpen) >> 1);
  if (NULL == currentRoot->lChild && newSegLeftIndexClosed < mid) {
    // create left-child on the fly
    //printf("creating lChild[%d, %d)\n", currentRoot->leftIndexClosed, mid);
    currentRoot->lChild = new SegmentTreeNode();
    currentRoot->lChild->leftIndexClosed = currentRoot->leftIndexClosed;
    currentRoot->lChild->rightIndexOpen = mid;
  }
  if (NULL != currentRoot->lChild) {
    RangeAdd(currentRoot->lChild, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge, level+1);
  }
  
  if (NULL == currentRoot->rChild && newSegRightIndexOpen > mid) {
    // create right-child on the fly
    //printf("creating rChild[%d, %d)\n", mid, currentRoot->rightIndexOpen);
    currentRoot->rChild = new SegmentTreeNode();
    currentRoot->rChild->leftIndexClosed = mid;
    currentRoot->rChild->rightIndexOpen = currentRoot->rightIndexOpen;
  }
  if (NULL != currentRoot->rChild) {
    RangeAdd(currentRoot->rChild, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedYEdge, level+1);
  }
}

void RangeIntervalCount(SegmentTreeNode* root, int targetLeftIndexClosed, int targetRightIndexOpen, vector<pair<int, int>> &result) {
  // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
  if (targetLeftIndexClosed >= targetRightIndexOpen) return;
  if (targetLeftIndexClosed >= root->rightIndexOpen) return;
  if (targetRightIndexOpen <= root->leftIndexClosed) return;
  
  // Snap valid "[targetLeftIndexClosed, targetRightIndexOpen)"s.
  targetLeftIndexClosed = max(targetLeftIndexClosed, root->leftIndexClosed);
  targetRightIndexOpen = min(targetRightIndexOpen, root->rightIndexOpen);
    
  if (0 < root->fullCoverAccDiff) {
    if (0 < result.size() && result.back().second == root->leftIndexClosed) {
      result.back().second = root->rightIndexOpen;
    } else {
      result.push_back({root->leftIndexClosed, root->rightIndexOpen});
    }
    return;
  }

  if (NULL != root->lChild) {
    RangeIntervalCount(root->lChild, targetLeftIndexClosed, targetRightIndexOpen, result);
  }
  if (NULL != root->rChild) {
    RangeIntervalCount(root->rChild, targetLeftIndexClosed, targetRightIndexOpen, result);
  }
}

class Solution {
public:
    int minNumberOperations(vector<int>& target) {
      int n = target.size();
      vector<Rectangle> recs;
      
      int currentH = target[0], currentL = 0;
      for (int i = 1; i < n; ++i) {
        if (currentH != target[i]) {
          Rectangle rec(currentL, i, 0, currentH);
          recs.push_back(rec);
          
          currentH = target[i];
          currentL = i;
        }
      }
      // wrap up
      Rectangle rec(currentL, n, 0, currentH);
      recs.push_back(rec);
          
      SegmentTreeNode* root = new SegmentTreeNode();
      root->leftIndexClosed = 0; root->rightIndexOpen = n;
      
      vector<vector<int>> sortedYEdgeList;
      for (auto &rec : recs) {
        sortedYEdgeList.push_back({rec.b, BOTTOM, rec.l, rec.r});
        sortedYEdgeList.push_back({rec.t, TOP, rec.l, rec.r});
      }
      sort(sortedYEdgeList.begin(), sortedYEdgeList.end(), less<vector<int>>());
      
      int lastY = INT_MAX;
      int ans = 0;
      for (auto &yEdge : sortedYEdgeList) {
        int currentY = yEdge[0];
        //printf("currentY:%d, [l:%d, r:%d]\n", currentY, yEdge[2], yEdge[3]);

        if (INT_MAX != lastY && currentY - lastY > 0) {
          vector<pair<int, int>> intervals;
          RangeIntervalCount(root, 0, n, intervals);
          //printf("\tintervals.size():%d\n", intervals.size());
          ans += (ULL)(currentY-lastY)*(ULL)intervals.size();
        }
        RangeAdd(root, yEdge[2], yEdge[3], {yEdge[0], yEdge[1]}, 0);
        lastY = currentY;
      }
      return ans;
    }
};
