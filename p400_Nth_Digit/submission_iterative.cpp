#define ULL uint64_t
#define MAXINTERVAL 100
ULL pow10[MAXINTERVAL];
ULL pow10IntervalDigitLengths[MAXINTERVAL]; // pow10IntervalDigitLengths[k] includes the digits in [10^k, 10^(k+1)-1].

class Solution {
public:
  int elementDigitLength(int k) {
    return (k+1);
  }
  
  int subintervalDigitLength(int k) {
    /*
      * of interval [10^k, 10^(k+1)-1]
      * each subinterval is [x0...0 ~ x9...9] of (10^k) elements where each element "x_..._" has (k+1) digits
      */
    return elementDigitLength(k)*pow10[k];
  }
  
  int findNthDigit(int n) {
    int remainingN = n;    
    int targetK = 0;
    for (int k = 0; k < MAXINTERVAL; ++k) {
      pow10[k] = (k == 0 ? 1 : 10*pow10[k-1]);
      /*
      there're always 9 "subinterval"s in an interval
      */
      int subintervalLen = subintervalDigitLength(k);
      pow10IntervalDigitLengths[k] = 9*(ULL)subintervalLen;

      if (remainingN <= pow10IntervalDigitLengths[k]) {
        targetK = k;
        break;
      }

      remainingN -= pow10IntervalDigitLengths[k];
    }
    
    int subintervalLen = subintervalDigitLength(targetK); 
    int elementLen = elementDigitLength(targetK);
    //printf("targetK == %d, remainingN == %d, subintervalLen == %d, elementLen == %d\n", targetK, remainingN, subintervalLen, elementLen);

    int subintervalIdx = 1;
    while (remainingN > subintervalLen) {
      remainingN -= subintervalLen;
      ++subintervalIdx;
    }
      
    int elementIdx = 1;
    while (remainingN > elementLen) {
      remainingN -= elementLen;
      ++elementIdx;
    }
    //printf("subintervalIdx == %d, elementIdx == %d, remainingN == %d\n", subintervalIdx, elementIdx, remainingN);
    
    if (1 == remainingN) {
      return subintervalIdx; // just the 'x' of "x_..._"
    }
    remainingN -= 2; // makes "remainingN" the index in "digitsOfElementValue", for example, searching for "remainingN: 8" in element "x7613902" is equivalent to searching for "arr[6]" in "arr=vector<int>{7,6,1,3,9,0,2}"
    
    int elementValue = (elementIdx-1);
    vector<int> digitsOfElementValue;
    int elementValueDigitLen = elementLen-1;
    for (int i = 0; i < elementValueDigitLen; ++i) {
      digitsOfElementValue.push_back(elementValue%10); 
      elementValue /= 10;
    }
    reverse(digitsOfElementValue.begin(), digitsOfElementValue.end());
    return digitsOfElementValue[remainingN];
  }
};
