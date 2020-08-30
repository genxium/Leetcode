#define MAXN 2001

class SegmentTreeNode {
public:
  bool fullCover = false;
  int leftIndexClosed; // To be initialized upon construction.
  int rightIndexOpen; // To be initialized upon construction.
  
  SegmentTreeNode* lChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
  SegmentTreeNode* rChild = NULL; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
};

void RangeAdd(SegmentTreeNode* currentRoot, int newSegLeftIndexClosed, int newSegRightIndexOpen, int level) {
  // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
  if (newSegLeftIndexClosed >= currentRoot->rightIndexOpen) return;
  if (newSegRightIndexOpen <= currentRoot->leftIndexClosed) return;
  
  // Snap valid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s.
  newSegLeftIndexClosed = max(newSegLeftIndexClosed, currentRoot->leftIndexClosed);
  newSegRightIndexOpen = min(newSegRightIndexOpen, currentRoot->rightIndexOpen);
  
  int indentSpaceCount = (level << 1);
  //printf("%*sRangeAdd, [newSegLeftIndexClosed:%d, newSegRightIndexOpen:%d)\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen);
  
  if (newSegLeftIndexClosed <= currentRoot->leftIndexClosed && newSegRightIndexOpen >=  currentRoot->rightIndexOpen) {
    // Proactively stops at "full cover" update.
    currentRoot->fullCover = true;
    return;
  }
  if (NULL != currentRoot->lChild) {
    RangeAdd(currentRoot->lChild, newSegLeftIndexClosed, newSegRightIndexOpen, level+1);
  }
  if (NULL != currentRoot->rChild) {
    RangeAdd(currentRoot->rChild, newSegLeftIndexClosed, newSegRightIndexOpen, level+1);
  }
  currentRoot->fullCover = (
    NULL != currentRoot->lChild && currentRoot->lChild->fullCover 
    && 
    NULL != currentRoot->rChild && currentRoot->rChild->fullCover
  );
}

void RangeCount(SegmentTreeNode* root, int targetLeftIndexClosed, int targetRightIndexOpen, vector<vector<int>>& ans) {
  // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
  if (targetLeftIndexClosed >= targetRightIndexOpen) return;
  if (targetLeftIndexClosed >= root->rightIndexOpen) return;
  if (targetRightIndexOpen <= root->leftIndexClosed) return;
  
  // Snap valid "[targetLeftIndexClosed, targetRightIndexOpen)"s.
  targetLeftIndexClosed = max(targetLeftIndexClosed, root->leftIndexClosed);
  targetRightIndexOpen = min(targetRightIndexOpen, root->rightIndexOpen);
  
  if (root->fullCover) {
    // Proactively stops at "full cover" update.
    if (!ans.empty() && ans.back().back() == root->leftIndexClosed-1) {
      /*
      We always recur at the "lChild" first, thus extending ALWAYS applies to the last interval.
      */
      ans.back().back() = root->rightIndexOpen-1;
    } else {
      ans.push_back({root->leftIndexClosed, root->rightIndexOpen-1});      
    }

    return;
  }

  if (NULL != root->lChild) {
    RangeCount(root->lChild, targetLeftIndexClosed, targetRightIndexOpen, ans);
  }
  if (NULL != root->rChild) {
    RangeCount(root->rChild, targetLeftIndexClosed, targetRightIndexOpen, ans);
  }
}

SegmentTreeNode* createBlankSegmentTree(int newSegLeftIndexClosed, int newSegRightIndexOpen) {
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) {
    return NULL;
  } 

  SegmentTreeNode* root = new SegmentTreeNode(); 
  root->leftIndexClosed = newSegLeftIndexClosed;
  root->rightIndexOpen = newSegRightIndexOpen;
  
  //printf("createBlankSegmentTree, newSegLeftIndexClosed:%d, newSegRightIndexOpen:%d, root->leftXClosed:%d, root->rightXClosed:%d.\n", newSegLeftIndexClosed, newSegRightIndexOpen, root->leftXClosed, root->rightXClosed);

  int mid = ((newSegLeftIndexClosed + newSegRightIndexOpen) >> 1);
  if (mid > newSegLeftIndexClosed) {
    root->lChild = createBlankSegmentTree(newSegLeftIndexClosed, mid);
    if (mid < newSegRightIndexOpen) {
      root->rChild = createBlankSegmentTree(mid, newSegRightIndexOpen);
    } else {
      root->rChild = NULL;
    }
  } else {
    root->lChild = NULL;
    root->rChild = NULL;
  }
  return root;
}

class SummaryRanges {
private:
  SegmentTreeNode* root = NULL;
public:
  /** Initialize your data structure here. */
  SummaryRanges() {
    root = createBlankSegmentTree(0, MAXN);
  }
    
  void addNum(int val) {
    RangeAdd(root, val, val+1, 0);
  }
    
  vector<vector<int>> getIntervals() {
    vector<vector<int>> ans;
    RangeCount(root, 0, MAXN, ans);
    return ans;
  }
};

/**
 * Your SummaryRanges object will be instantiated and called as such:
 * SummaryRanges* obj = new SummaryRanges();
 * obj->addNum(val);
 * vector<vector<int>> param_2 = obj->getIntervals();
 */
