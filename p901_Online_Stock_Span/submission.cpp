int const MAXN = 10000, INVALID = -1;
int lmost[MAXN]; // "lmost[j] == i" means that for "prices[j]", the max span is "prices[i, ..., j]" 

class StockSpanner {
private:
    vector<int> prices;
    
public:
    StockSpanner() {
        memset(lmost, INVALID, sizeof lmost);
    }
    
    int next(int price) {
        prices.push_back(price);
        int j = prices.size()-1;
        
        // init
        lmost[j] = j;
        int i = lmost[j];
        
        // loop 
        while (i-1 >= 0 && prices[i-1] <= prices[j]) {
            lmost[j] = lmost[i-1];
            i = lmost[j];
        }
        
        return j-i+1;
    }
};

/**
 * Your StockSpanner object will be instantiated and called as such:
 * StockSpanner* obj = new StockSpanner();
 * int param_1 = obj->next(price);
 */
