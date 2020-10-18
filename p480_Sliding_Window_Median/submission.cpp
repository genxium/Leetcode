#define INVALID -1
/*
test case#1
[1,3,-1,-3,5,3,6,7]
3

test case#2
[5,5,8,1]
4

test case#3
[5,5,8,1,4,7,1]
7

test case#4
[5,5,8,1,4,7,1,3]
8

test case#5
[5,5,8,1,4,7,1,3,8,4]
8

test case#6
[6,5,9,5,4,9]
4

test case#7
[6,5,9,5,4,9,1,7,5,5]
4
*/
class Solution {
private:
  map<int, int> groupCounter;
  int medianVal;
  int medianOffsetInGroup;
  int totalCount;
  
  void addNum(int num) {
    map<int, int>::iterator it = groupCounter.find(num);
    int prevGroupCounterSize = groupCounter.size();
    
    if (it == groupCounter.end()) {
      groupCounter[num] = 1;
    } else {
      groupCounter[num] += 1;
    }
    if (0 == prevGroupCounterSize && 1 == groupCounter.size()) {
      medianVal = num;
      medianOffsetInGroup = 0;
    } else {
      if (num == medianVal) {
        if (totalCount%2 == 1) {
          ++medianOffsetInGroup;
        }
      } else if (num > medianVal) {
        if (totalCount%2 == 1) {
          ++medianOffsetInGroup;
          if (medianOffsetInGroup >= groupCounter[medianVal]) {
            map<int, int>::iterator nextGroup = groupCounter.upper_bound(medianVal); 
            medianVal = nextGroup->first;
            medianOffsetInGroup = 0;
          }
        }
      } else {
        // num < medianVal
        if (totalCount%2 == 0) {
          --medianOffsetInGroup;
          if (medianOffsetInGroup < 0) {
            map<int, int>::iterator prevGroup = groupCounter.lower_bound(medianVal);
            --prevGroup;
            medianVal = prevGroup->first;
            medianOffsetInGroup = prevGroup->second - 1;
          }
        }
      }
    }
    ++totalCount;
  }
  
  void removeNum(int num) {
    --groupCounter[num];
    if (0 == groupCounter[num]) {
      groupCounter.erase(num);
    }
    
    if (0 == groupCounter.size()) {
      medianVal = 0;
      medianOffsetInGroup = 0;
    } else {
      if (num == medianVal) {
        if (totalCount%2 == 0) {
          --medianOffsetInGroup;
          if (medianOffsetInGroup < 0) {
            // printf("\tmedianOffsetInGroup for medianVal:%d becomes %d\n", medianVal, medianOffsetInGroup);
            map<int, int>::iterator prevGroup = groupCounter.lower_bound(medianVal);
            --prevGroup;
            medianVal = prevGroup->first;
            medianOffsetInGroup = prevGroup->second - 1;
          }
        } else {
          // totalCount%2 == 1
          if (groupCounter.find(num) == groupCounter.end() || medianOffsetInGroup >= groupCounter[num]) {
            map<int, int>::iterator nextGroup = groupCounter.upper_bound(medianVal); 
            medianVal = nextGroup->first;
            medianOffsetInGroup = 0;
          }
        }
      } else if (num > medianVal) {
        if (totalCount%2 == 0) {
          --medianOffsetInGroup;
          if (medianOffsetInGroup < 0) {
            // printf("\tmedianOffsetInGroup for medianVal:%d becomes %d\n", medianVal, medianOffsetInGroup);
            map<int, int>::iterator prevGroup = groupCounter.lower_bound(medianVal);
            --prevGroup;
            medianVal = prevGroup->first;
            medianOffsetInGroup = prevGroup->second - 1;
          } else {
            /*
            -3 -1 3 3 5
                  ^

            remove 5
            -3 -1 3 3
                  ^
            */
          }
        }
      } else {
        // num < medianVal
        if (totalCount%2 == 1) {
          /*
          ```
          -3 -1 3 3 5
                ^
          ```
          removing -1, becomes
          ```
          -3 3 3 5
               ^
          ```
          */
          ++medianOffsetInGroup;
          if (medianOffsetInGroup >= groupCounter[medianVal]) {
            map<int, int>::iterator nextGroup = groupCounter.upper_bound(medianVal); 
            medianVal = nextGroup->first;
            medianOffsetInGroup = 0;
          }
        } else {
          /*
          ```
          -3 -1 3 5
                ^
          ```
          removing -1, becomes
          ```
          -3 3 5
             ^
          ```
          */
        }
      }
    }
    --totalCount;
  }
  
  double findMedian() {
    if (totalCount%2 == 1) {
      return (double)this->medianVal;
    } else {
      // Always look backward for "medianOffsetInGroup".
      // printf("\tlooking backward...");

      if (medianOffsetInGroup - 1 >= 0) {
        // printf("found in same group\n");
        return (double)this->medianVal;
      } else {
        map<int, int>::iterator prevGroup = groupCounter.lower_bound(medianVal);
        --prevGroup;
        // printf("found in prev group with key == %d\n", prevGroup->first);
        return ((double)this->medianVal + (double)prevGroup->first)/2;
      }
    }
  }
  
public:
  Solution() {
    totalCount = 0;
  }
  
  vector<double> medianSlidingWindow(vector<int>& nums, int k) {
    // init
    int n = nums.size();
    vector<double> ans;
    for (int i = 0; i < k; ++i) {
      // printf("adding nums[i:%d] == %d\n", i, nums[i]);
      addNum(nums[i]);
      // printf("transitional median is: %.2lf, where totalCount == %d, medianVal == %d, medianOffsetInGroup == %d\n", findMedian(), totalCount, medianVal, medianOffsetInGroup);
    }

    // loop
    for (int i = 0; i < n-k+1; ++i) {
      if (i > 0) {
        // printf("adding nums[i+k-1:%d] == %d\n", i+k-1, nums[i+k-1]);
        addNum(nums[i+k-1]);
        // printf("transitional median is: %.2lf, where totalCount == %d, medianVal == %d, medianOffsetInGroup == %d\n", findMedian(), totalCount, medianVal, medianOffsetInGroup);
        // printf("removing nums[i-1:%d] == %d\n", i-1, nums[i-1]);
        removeNum(nums[i-1]);
        // printf("final median is: %.2lf, where totalCount == %d, medianVal == %d, medianOffsetInGroup == %d\n\n", findMedian(), totalCount, medianVal, medianOffsetInGroup);
      }
      ans.push_back(findMedian());
    }
    
    return ans;
  }
};
