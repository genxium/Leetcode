#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define INVALID INT_MAX

typedef struct NumArray {
  int unifiedValue;
  int leftIndexClosed; // To be initialized upon construction.
  int rightIndexOpen; // To be initialized upon construction.
  struct NumArray* lChild; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
  struct NumArray* rChild; // To be initialized upon construction, and would be NULL for a leaf, i.e. (rightIndexOpen == 1 + leftIndexClosed).
} NumArray;

void printSegmentTree(NumArray* root, int level) {
  if (NULL == root) return; 
  int indentSpaceCount = (level << 1);

  printf("%*s[%d, %d): %d\n", indentSpaceCount, "", root->leftIndexClosed, root->rightIndexOpen, root->unifiedValue);

  if (NULL != root->lChild) {
    printSegmentTree(root->lChild, level+1);
  } 
  if (NULL != root->rChild) {
    printSegmentTree(root->rChild, level+1);
  } 
}

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
  if (newSegLeftIndexClosed >= newSegRightIndexOpen) return;
  int indentSpaceCount = (level << 1);
  printf("%*sRangeWrite, assigning [%d, %d) <- %d into [%d, %d):%d, allowResiduePropagation = %d.\n", indentSpaceCount, "", newSegLeftIndexClosed, newSegRightIndexOpen, unifiedValue, currentRoot->leftIndexClosed, currentRoot->rightIndexOpen, currentRoot->unifiedValue, allowResiduePropagation);

  if (newSegLeftIndexClosed <= currentRoot->leftIndexClosed && newSegRightIndexOpen >=  currentRoot->rightIndexOpen) {
    // Proactively stops at "full cover" update.
    currentRoot->unifiedValue = unifiedValue;
    printf("%*sRangeWrite, [%d, %d) updated to %d.\n", indentSpaceCount, "", currentRoot->leftIndexClosed, currentRoot->rightIndexOpen);
    return;
  }
  if (unifiedValue == currentRoot->unifiedValue) {
    return;
  }
  int residueUnifiedValue = currentRoot->unifiedValue;
  currentRoot->unifiedValue = INVALID; // By now it's certain that "currentRoot->unifiedValue" is no longer unified.

  int currentRootMidIndex = ((currentRoot->leftIndexClosed + currentRoot->rightIndexOpen) >> 1);

  if (true == allowResiduePropagation && INVALID != residueUnifiedValue) {
    /*
    * Having "INVALID != residueUnifiedValue" implies that it's "more recent" than "any residueUnifiedValue from the subtree".
    */
    if (newSegLeftIndexClosed < currentRootMidIndex && currentRootMidIndex <= newSegRightIndexOpen) {
      // Case#1, |    ..|..    |
      if (NULL != currentRoot->lChild) {
        // The left residue into lChild.
        {
          int residueSegLeftClosed = currentRoot->leftIndexClosed;
          int residueSegRightOpen = newSegLeftIndexClosed;

          RangeWrite(currentRoot->lChild, residueSegLeftClosed, residueSegRightOpen, residueUnifiedValue, false, level+1);
        }
      } 

      if (NULL != currentRoot->rChild) {
        // The right residue into rChild.
        {
          int residueSegLeftClosed = newSegRightIndexOpen;
          int residueSegRightOpen = currentRoot->rightIndexOpen;

          RangeWrite(currentRoot->lChild, residueSegLeftClosed, residueSegRightOpen, residueUnifiedValue, false, level+1);
        }
      }
    } else if (newSegLeftIndexClosed < currentRootMidIndex && newSegRightIndexOpen <= currentRootMidIndex) {
      // Case#2, | .... |      |
      if (NULL != currentRoot->lChild) {
        // The left residue into lChild.
        {
          int residueSegLeftClosed = currentRoot->leftIndexClosed;
          int residueSegRightOpen = newSegLeftIndexClosed;

          RangeWrite(currentRoot->lChild, residueSegLeftClosed, residueSegRightOpen, residueUnifiedValue, false, level+1);
        }

        // The right residue into lChild.
        {
          int residueSegLeftClosed = newSegRightIndexOpen;
          int residueSegRightOpen = currentRootMidIndex;

          RangeWrite(currentRoot->lChild, residueSegLeftClosed, residueSegRightOpen, residueUnifiedValue, false, level+1);
        }
      } 

      if (NULL != currentRoot->rChild) {
        // The right residue into rChild.
        {
          int residueSegLeftClosed = currentRootMidIndex;
          int residueSegRightOpen = currentRoot->rightIndexOpen;

          RangeWrite(currentRoot->lChild, residueSegLeftClosed, residueSegRightOpen, residueUnifiedValue, false, level+1);
        }
      }
    } else if (currentRootMidIndex < newSegLeftIndexClosed && currentRootMidIndex < newSegRightIndexOpen) {
      // Case#3, |      | .... |
      if (NULL != currentRoot->lChild) {
        // The left residue into lChild.
        {
          int residueSegLeftClosed = currentRoot->leftIndexClosed;
          int residueSegRightOpen = currentRootMidIndex;

          RangeWrite(currentRoot->lChild, residueSegLeftClosed, residueSegRightOpen, residueUnifiedValue, false, level+1);
        }
      } 

      if (NULL != currentRoot->rChild) {
        // The left residue into rChild.
        {
          int residueSegLeftClosed = currentRootMidIndex;
          int residueSegRightOpen = newSegLeftIndexClosed;

          RangeWrite(currentRoot->lChild, residueSegLeftClosed, residueSegRightOpen, residueUnifiedValue, false, level+1);
        }

        // The right residue into rChild.
        {
          int residueSegLeftClosed = newSegRightIndexOpen;
          int residueSegRightOpen = currentRoot->rightIndexOpen;

          RangeWrite(currentRoot->lChild, residueSegLeftClosed, residueSegRightOpen, residueUnifiedValue, false, level+1);
        }
      }
    } else;
  }

  if (NULL != currentRoot->lChild && newSegLeftIndexClosed < currentRootMidIndex) {
    int theRightIndexOpen = (newSegRightIndexOpen < currentRootMidIndex ? newSegRightIndexOpen : currentRootMidIndex);
    RangeWrite(currentRoot->lChild, newSegLeftIndexClosed, theRightIndexOpen, unifiedValue, allowResiduePropagation, level+1);
  }
  if (NULL != currentRoot->rChild && newSegRightIndexOpen >= currentRootMidIndex) {
    int theLeftIndexClosed = (newSegLeftIndexClosed >= currentRootMidIndex ? newSegLeftIndexClosed : currentRootMidIndex);
    RangeWrite(currentRoot->rChild, theLeftIndexClosed, newSegRightIndexOpen, unifiedValue, allowResiduePropagation, level+1);
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
  if (targetLeftIndexClosed >= targetRightIndexOpen) {
    return;
  }
  if (INVALID != root->unifiedValue) {
    int theLeftIndexClosed = (targetLeftIndexClosed >= root->leftIndexClosed ? targetLeftIndexClosed : root->leftIndexClosed);
    int theRightIndexOpen = (targetRightIndexOpen <= root->rightIndexOpen ? targetRightIndexOpen : root->rightIndexOpen);
    (*pResult) = (*pResult) + (theRightIndexOpen - theLeftIndexClosed)*(root->unifiedValue);
    return;
  } 

  int currentRootMidIndex = ((root->leftIndexClosed + root->rightIndexOpen) >> 1);

  if (NULL != root->lChild && targetLeftIndexClosed < currentRootMidIndex) {
    int theRightIndexOpen = (targetRightIndexOpen < currentRootMidIndex ? targetRightIndexOpen : currentRootMidIndex);
    RangeSum(root->lChild, targetLeftIndexClosed, theRightIndexOpen, pResult);
  }
  if (NULL != root->rChild && targetRightIndexOpen >= currentRootMidIndex) {
    int theLeftIndexClosed = (targetLeftIndexClosed >= currentRootMidIndex ? targetLeftIndexClosed : currentRootMidIndex);
    RangeSum(root->rChild, theLeftIndexClosed, targetRightIndexOpen, pResult);
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


int main() {
  int arr[] = {1, 3, 5};
  int* pArr = arr;
  NumArray* root = numArrayCreate(pArr, 3);
  printSegmentTree(root, 0);

  int leftIndexClosed = 0;
  int rightIndexOpen = 3;

  int rangeSum = numArraySumRange(root, leftIndexClosed, rightIndexOpen);
  printf("rangeSum of [%d, %d) is %d.\n", leftIndexClosed, rightIndexOpen, rangeSum);

  numArrayFree(root);
  return 0;
}
