#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define SINGLE_INTERVAL_ARR_SIZE 2

typedef struct Pair {
  int st;
  int ed;
  struct Pair* next;
} Pair; 

Pair* allocNewPair(int st, int ed) {
  Pair* toRet = (Pair*)malloc(sizeof(Pair));
  toRet->st = st;
  toRet->ed = ed;
  toRet->next = NULL;
  return toRet;
}

void appendToRear(Pair** head, int* pPosition, Pair* newPair) {
  int position = (*pPosition);
  if (0 < position) { 
    Pair* lastPair = *(head + position - 1);
    lastPair->next = newPair;
    // printf("Appending (%d, %d) to (%d, %d)\n", newPair->st, newPair->ed, lastPair->st, lastPair->ed);
  }   
  *(head + position) = newPair;
  (*pPosition) = (1 + position);
  // printf("Closed newPair by value (%d, %d)\n\n", newPair->st, newPair->ed);
}

void closeOrExtendCurrentEnclosure(Pair** head, int* pPosition, Pair** pCurrentEnclosure, Pair *pCandidate) {
  /*
   * Deliberately NOT invoking "(*pCurrentEnclosure) = allocNewPair(...)" within this function.
   */
  Pair* currentEnclosure = (*pCurrentEnclosure);
  if (NULL == pCandidate) {
    // printf("closeOrExtendCurrentEnclosure #1, closing \n");
    // printf("  currentEnclosure (%d, %d) by pCandidate == NULL\n", currentEnclosure->st, currentEnclosure->ed);
    appendToRear(head, pPosition, currentEnclosure);
    // Close the (*pCurrentEnclosure).
    (*pCurrentEnclosure) = NULL;
  } else {
    // printf("closeOrExtendCurrentEnclosure #2, \n");
    // printf("  by candidate (%d, %d)\n", pCandidate->st, pCandidate->ed);
    if (pCandidate->st > currentEnclosure->ed) {
      // printf("  closing currentEnclosure (%d, %d)\n", currentEnclosure->st, currentEnclosure->ed);
      appendToRear(head, pPosition, currentEnclosure);
      // Close the (*pCurrentEnclosure).
      (*pCurrentEnclosure) = NULL;
    } else {
      if (pCandidate->ed > currentEnclosure->ed) {
        // Extends the right edge of (*pCurrentEnclosure).
        // printf("  extending currentEnclosure (%d, %d)\n", currentEnclosure->st, currentEnclosure->ed);
        (*pCurrentEnclosure)->ed = pCandidate->ed; 
      }
    }
  }
}

int cmpfunc (void const * a, void const * b) {
  Pair* convertedA = (Pair*)a;
  Pair* convertedB = (Pair*)b;
  if (convertedA->st != convertedB->st) {
    return ( convertedA->st - convertedB->st );
  } else {
    return ( convertedA->ed - convertedB->ed );
  }
}

int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes) {
  /**
   * Return an array of arrays of size *returnSize.
   * The sizes of the arrays are returned as *returnColumnSizes array.
   * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
   */
  if (0 >= intervalsSize) {
    /*
     * The caller is responsible for freeing the "toRet" and "returnColumnSizes" w.r.t. "returnSize". 
     */
    int varReturnSize = 0;
    int* varReturnColumnSizes = (int*)malloc(varReturnSize*sizeof(int));
    *returnSize = varReturnSize;
    *returnColumnSizes = varReturnColumnSizes;
    int** toRet = (int**)malloc(varReturnSize*sizeof(int*));
    return toRet;
  }
  
  Pair* arrOfPairs = (Pair*)malloc(intervalsSize*sizeof(Pair));
  for (int i = 0; i < intervalsSize; ++i) {
    int* singleInterval = intervals[i];
    Pair p;
    p.st = singleInterval[0];
    p.ed = singleInterval[1]; 
    p.next = NULL;
    arrOfPairs[i] = p; 
  }
  qsort(arrOfPairs, intervalsSize, sizeof(Pair), cmpfunc);

  int varReturnSize = 0;
  Pair** toRetIntermediate = (Pair**)malloc(intervalsSize*sizeof(Pair*));
  Pair* currentEnclosure = NULL;

  for (int i = 0; i < intervalsSize; ++i) {
    Pair singlePair = arrOfPairs[i];
    if (NULL != currentEnclosure) {
      closeOrExtendCurrentEnclosure(toRetIntermediate, &varReturnSize, (&currentEnclosure), &singlePair);
    }

    if (NULL == currentEnclosure) {
      // Allocate new HeapMem for "toRetIntermediate".
      currentEnclosure = allocNewPair(singlePair.st, singlePair.ed);
    }

    if (i == intervalsSize - 1) {
      if (NULL != currentEnclosure) {
        closeOrExtendCurrentEnclosure(toRetIntermediate, &varReturnSize, (&currentEnclosure), NULL);
      }
    }
  }
  free(arrOfPairs);

  int* varReturnColumnSizes = (int*)malloc(varReturnSize*sizeof(int));
  int** toRet = (int**)malloc(varReturnSize*sizeof(int*));
  int counter = 0;
  currentEnclosure = *toRetIntermediate; 
  Pair* preEnclosure = NULL;
  while (NULL != currentEnclosure) {
    int* singleToRetInterval = (int*)malloc(SINGLE_INTERVAL_ARR_SIZE*sizeof(int));
    singleToRetInterval[0] = currentEnclosure->st;
    singleToRetInterval[1] = currentEnclosure->ed;
    *(toRet + counter) = singleToRetInterval;
    *(varReturnColumnSizes + counter) = SINGLE_INTERVAL_ARR_SIZE;
    ++counter;
    preEnclosure = currentEnclosure;
    currentEnclosure = currentEnclosure->next;
    free(preEnclosure);
  }
  free(toRetIntermediate);
  /*
   * The caller is responsible for freeing the "toRet" and "returnColumnSizes" w.r.t. "returnSize". 
   */
  *returnSize = varReturnSize;
  *returnColumnSizes = varReturnColumnSizes;
  return toRet;
}

int main() {
  // int countOfIntervals = 0;
  // int** inputArrs = (int**)malloc(countOfIntervals*sizeof(int*));
  
  // int countOfIntervals = 2;
  // int** inputArrs = (int**)malloc(countOfIntervals*sizeof(int*));
  // for (int i = 0; i < countOfIntervals; ++i) {
  //   if (0 == i) {
  //     int values[SINGLE_INTERVAL_ARR_SIZE] = {1, 4};
  //     inputArrs[i] = values;
  //   } else if (1 == i) {
  //     int values[SINGLE_INTERVAL_ARR_SIZE] = {4, 6};
  //     inputArrs[i] = values;
  //   } else;
  // } 

  int countOfIntervals = 4;
  int** inputArrs = (int**)malloc(countOfIntervals*sizeof(int*));
  for (int i = 0; i < countOfIntervals; ++i) {
    if (0 == i) {
      int values[SINGLE_INTERVAL_ARR_SIZE] = {1, 3};
      inputArrs[i] = values;
    } else if (1 == i) {
      int values[SINGLE_INTERVAL_ARR_SIZE] = {2, 6};
      inputArrs[i] = values;
    } else if (2 == i) {
      int values[SINGLE_INTERVAL_ARR_SIZE] = {8, 10};
      inputArrs[i] = values;
    } else if (3 == i) {
      int values[SINGLE_INTERVAL_ARR_SIZE] = {15, 18};
      inputArrs[i] = values;
    } else;
  } 

  int initIntervalsColSize = SINGLE_INTERVAL_ARR_SIZE;
  int initReturnSize = 0;
  int* initReturnColumnSizes = NULL; 

  int* returnSize = &initReturnSize; 
  int* intervalsColSize = &initIntervalsColSize;
  int** returnColumnSizes = &initReturnColumnSizes;

  int** result = merge(inputArrs, countOfIntervals, intervalsColSize, returnSize, returnColumnSizes);

  for (int i = 0; i < *returnSize; ++i) {
    int* singlePair = result[i];
    printf("(%d, %d) ", singlePair[0], singlePair[1]);
    free(result[i]);
  }
  free(result);
  printf("\n");

  free((*returnColumnSizes));
  free(inputArrs);
  return 0;
}
