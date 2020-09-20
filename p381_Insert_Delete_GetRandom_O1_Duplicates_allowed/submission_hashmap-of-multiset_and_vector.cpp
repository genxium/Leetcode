class RandomizedCollection {
private:
    unordered_map<int, unordered_multiset<int>> dictKeeper;
    vector<int> tailKeeper;
    
public:
    /** Initialize your data structure here. */
    RandomizedCollection() {
        
    }
    
    /** Inserts a value to the collection. Returns true if the collection did not already contain the specified element. */
    bool insert(int val) {
        bool alreadyContained = (
            dictKeeper.find(val) != dictKeeper.end()
            &&
            0 < dictKeeper[val].size()
        );
        dictKeeper[val].insert(tailKeeper.size());
        tailKeeper.push_back(val);
        return !alreadyContained; 
    }
    
    /** Removes a value from the collection. Returns true if the collection contained the specified element. */
    bool remove(int val) {
        if (dictKeeper.find(val) == dictKeeper.end()) return false;
        if (0 == dictKeeper[val].size()) return false;
        
        int origIndex = *(dictKeeper[val].begin());
        int lastIndex = tailKeeper.size()-1;
        int swappedVal = tailKeeper.back();
        
        if (origIndex != lastIndex) {
            swap(tailKeeper[origIndex], tailKeeper[lastIndex]);
            dictKeeper[val].erase(origIndex);
            dictKeeper[swappedVal].erase(lastIndex);
            tailKeeper.pop_back();
            dictKeeper[swappedVal].insert(origIndex);
        } else {
            tailKeeper.pop_back();
            dictKeeper[val].erase(origIndex);
        }
            
        return true;
    }
    
    /** Get a random element from the collection. */
    int getRandom() {
        int rIdx = rand() % tailKeeper.size();
        return tailKeeper[rIdx];
    }
};

/**
 * Your RandomizedCollection object will be instantiated and called as such:
 * RandomizedCollection* obj = new RandomizedCollection();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */
