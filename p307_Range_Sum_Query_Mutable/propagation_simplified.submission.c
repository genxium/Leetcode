#define min(x,y) (x <= y ? x : y) 
#define max(x,y) (x >= y ? x : y) 
#define INVALID INT_MAX

typedef struct NumArray {
  int unifiedValue;
  int leftIndexClosed; // To be initialized upon construction.
  int rightIndexOpen; // To be initialized upon construction.
  struct NumArray* lChild; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
  struct NumArray* rChild; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
} NumArray;

NumArray* createBlankSegmentTree(int newSegLeftIndexClosed, int newSegRightIndexOpen) {
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) {
    return NULL;
  } 

  // printf("createBlankSegmentTree, newSegLeftIndexClosed = %d, newSegRightIndexOpen = %d.\n", newSegLeftIndexClosed, newSegRightIndexOpen);

  NumArray* root = (NumArray*)malloc(sizeof(NumArray)); 
  root->leftIndexClosed = newSegLeftIndexClosed;
  root->rightIndexOpen = newSegRightIndexOpen;
  root->unifiedValue = INVALID;

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

/**
 * Your NumArray struct will be instantiated and called as such:
 * NumArray* obj = numArrayCreate(nums, numsSize);
 * numArrayUpdate(obj, i, val);
 
 * int param_2 = numArraySumRange(obj, i, j);
 
 * numArrayFree(obj);
*/

void RangeWrite(NumArray* currentRoot, int newSegLeftIndexClosed, int newSegRightIndexOpen, int unifiedValue, bool allowResiduePropagation, int level) {
  // Reject invalid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s. 
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
  if (newSegLeftIndexClosed >= currentRoot->rightIndexOpen) return;
  if (newSegRightIndexOpen <= currentRoot->leftIndexClosed) return;
  
  // Snap valid "[newSegLeftIndexClosed, newSegRightIndexOpen)"s.
  newSegLeftIndexClosed = max(newSegLeftIndexClosed, currentRoot->leftIndexClosed);
  newSegRightIndexOpen = min(newSegRightIndexOpen, currentRoot->rightIndexOpen);
      
  int indentSpaceCount = (level << 1);
  // printf("%*sRangeWrite, assigning [%d, %d) <- %d into [%d, %d):%d, allowResiduePropagation = %d.\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, unifiedValue, currentRoot->leftIndexClosed, currentRoot->rightIndexOpen, currentRoot->unifiedValue, allowResiduePropagation);

  if (newSegLeftIndexClosed <= currentRoot->leftIndexClosed && newSegRightIndexOpen >=  currentRoot->rightIndexOpen) {
    // Proactively stops at "full cover" update.
    currentRoot->unifiedValue = unifiedValue;
    // printf("%*sRangeWrite, [%d, %d) updated to %d.\n", indentSpaceCount, "", currentRoot->leftIndexClosed, currentRoot->rightIndexOpen);
    return;
  }
  if (unifiedValue == currentRoot->unifiedValue) {
    return;
  }
  int residueUnifiedValue = currentRoot->unifiedValue;
  currentRoot->unifiedValue = INVALID; // By now it's certain that "currentRoot->unifiedValue" is no longer unified.

  if (true == allowResiduePropagation && INVALID != residueUnifiedValue) {
    /*
    * Having "INVALID != residueUnifiedValue" implies that it's "more recent" than "any residueUnifiedValue from the subtree".
    */
    int leftResidueLeftClosed = currentRoot->leftIndexClosed;
    int leftResidueRightOpen = newSegLeftIndexClosed;
      
    int rightResidueLeftClosed = newSegRightIndexOpen;
    int rightResidueRightOpen = currentRoot->rightIndexOpen;
    if (NULL != currentRoot->lChild) {
      RangeWrite(currentRoot->lChild, leftResidueLeftClosed, leftResidueRightOpen, residueUnifiedValue, false, level+1);
      RangeWrite(currentRoot->lChild, rightResidueLeftClosed, rightResidueRightOpen, residueUnifiedValue, false, level+1);
    } 

    if (NULL != currentRoot->rChild) {
      RangeWrite(currentRoot->rChild, leftResidueLeftClosed, leftResidueRightOpen, residueUnifiedValue, false, level+1);
      RangeWrite(currentRoot->rChild, rightResidueLeftClosed, rightResidueRightOpen, residueUnifiedValue, false, level+1);
    }
  }

  if (NULL != currentRoot->lChild) {
    RangeWrite(currentRoot->lChild, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedValue, allowResiduePropagation, level+1);
  }
  if (NULL != currentRoot->rChild) {
    RangeWrite(currentRoot->rChild, newSegLeftIndexClosed, newSegRightIndexOpen, unifiedValue, allowResiduePropagation, level+1);
  }
}

void numArrayUpdate(NumArray* obj, int i, int val) {
  RangeWrite(obj, i, i+1, val, true, 0); 
}

NumArray* numArrayCreate(int* nums, int numsSize) {
  NumArray* root = createBlankSegmentTree(0, numsSize); 
  for (int i = 0; i < numsSize; ++i) {
    numArrayUpdate(root, i, nums[i]);
  }
  return root;
}

void RangeSum(NumArray* root, int targetLeftIndexClosed, int targetRightIndexOpen, int* pResult) {
  // Reject invalid "[targetLeftIndexClosed, targetRightIndexOpen)"s. 
  if (targetLeftIndexClosed >= targetRightIndexOpen) return;
  if (targetLeftIndexClosed >= root->rightIndexOpen) return;
  if (targetRightIndexOpen <= root->leftIndexClosed) return;
  
  // Snap valid "[targetLeftIndexClosed, targetRightIndexOpen)"s.
  targetLeftIndexClosed = max(targetLeftIndexClosed, root->leftIndexClosed);
  targetRightIndexOpen = min(targetRightIndexOpen, root->rightIndexOpen);
    
  if (INVALID != root->unifiedValue) {
    (*pResult) = (*pResult) + (targetRightIndexOpen - targetLeftIndexClosed)*(root->unifiedValue);
    return;
  } 

  if (NULL != root->lChild) {
    RangeSum(root->lChild, targetLeftIndexClosed, targetRightIndexOpen, pResult);
  }
  if (NULL != root->rChild) {
    RangeSum(root->rChild, targetLeftIndexClosed, targetRightIndexOpen, pResult);
  }
}

int numArraySumRange(NumArray* obj, int i, int j) {
  int result = 0; 
  RangeSum(obj, i, j+1, &result);
  return result;
}

void numArrayFree(NumArray* obj) {
  if (NULL == obj) return;
  if (NULL != obj->lChild) {
    numArrayFree(obj->lChild);
  }
  if (NULL != obj->rChild) {
    numArrayFree(obj->rChild);
  }
  free(obj); 
}
