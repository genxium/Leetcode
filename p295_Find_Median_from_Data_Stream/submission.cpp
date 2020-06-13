class MedianFinder {
public:
    map<int, int> groupCounter;
    int medianVal;
    int medianOffsetInGroup;
    int totalCount;
    
    /** initialize your data structure here. */
    MedianFinder() {
        totalCount = 0;
    }
    
    void addNum(int num) {
        map<int, int>::iterator it = groupCounter.find(num);
        if (it == groupCounter.end()) {
            groupCounter[num] = 1;
        } else {
            groupCounter[num] += 1;
        }
        if (1 == groupCounter.size()) {
            medianVal = num;
            medianOffsetInGroup = 0;
        } else {
            if (num == medianVal) {
                if (totalCount%2 == 1) {
                    /*
                    * Consider initially 
                    * 2  6  10
                    *    ^ 
                    * , then adding "6" should result in 
                    *
                    * 2  6  6  10
                    *       ^
                    * which is made of in "findMedian" to calculate the outcome when (0 == totalCount%2).
                    * Another example for this branch case should be 
                    * 2  6  6  10  11
                    *       ^ 
                    * , then adding "6" should result in 
                    *
                    * 2  6  6  6  10  11
                    *          ^
                    * which also works in "findMedian" to calculate the outcome when (0 == totalCount%2).
                    */
                    ++medianOffsetInGroup;
                } else {
                    /*
                    * Consider initially 
                    * 2  6  6  10
                    *       ^ 
                    * , then adding "6" should result in 
                    *
                    * 2  6  6  6  10
                    *       ^
                    * where "medianOffsetInGroup" doesn't change.
                    */
                }
            } else if (num > medianVal) {
                if (totalCount%2 == 1) {
                    ++medianOffsetInGroup;
                    if (medianOffsetInGroup >= groupCounter[medianVal]) {
                        map<int, int>::iterator nextGroup = groupCounter.upper_bound(medianVal); 
                        medianVal = nextGroup->first;
                        medianOffsetInGroup = 0;
                    }
                } else {
                    /*
                    * No need to move "medianOffsetInGroup", consider the following case
                    * 2  6  6  10
                    *       ^
                    * , adding "11" should result in
                    * 2  6  6  10  11.
                    *       ^
                    */
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
                } else {
                    /*
                    * No need to move "medianOffsetInGroup", consider the following case
                    * 2  6  6  6  10
                    *       ^
                    * , adding "3" should result in
                    * 2  3  6  6  6  10
                    *          ^
                    * which is made of in "findMedian" to calculate the outcome when (0 == totalCount%2).
                    */
                }
            }
        }
        ++totalCount;
    }
    
    double findMedian() {
        //printf("findMedian called when totalCount == %d, medianVal == %d, medianOffsetInGroup == %d\n", totalCount, medianVal, medianOffsetInGroup);
        if (totalCount%2 == 1) {
            return (double)this->medianVal;
        } else {
            // Always look backward for "medianOffsetInGroup".
            //printf("\tlooking backward...");

            if (medianOffsetInGroup - 1 >= 0) {
                //printf("found in same group\n");
                return (double)this->medianVal;
            } else {
                map<int, int>::iterator prevGroup = groupCounter.lower_bound(medianVal);
                --prevGroup;
                //printf("found in prev group with key == %d\n", prevGroup->first);
                return ((double)this->medianVal + (double)prevGroup->first)/2;
            }
        }
    }
};

/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */
