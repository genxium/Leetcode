typedef vector<int> VI;
typedef deque<int> DI;
bool const debug = false;
/*
test cases
[17,13,11,2,3,5,7]
[1,1000]
[1,2,3,4,5,6,7,8,9]
*/
class Solution {
public:
    vector<int> dfs(VI& sorted) {
        // Returns a shuffled list of cards that would be revealed in increasing order.
        int n = sorted.size();
        if (2 >= n) return VI(sorted.begin(), sorted.end());
        bool odd = (n & 1 > 0);
        int firstPartEdInclusive = odd ? ((n+1)/2) : (n/2);
        DI firstPart(sorted.begin(), sorted.begin()+firstPartEdInclusive);
        VI secondPartDraft(sorted.begin()+firstPartEdInclusive, sorted.end());
        VI secondPartVI = dfs(secondPartDraft);
        DI secondPart(secondPartVI.begin(), secondPartVI.end());
        if (odd) {
            // [WARNING] Now that "secondPart" is shuffled such that if played alone would be revealed in increasing order. However, after the original last card, the "1st put-back card" will be put back again, therefore we "put forward" a "shield" here.
            int shield = secondPart.back(); secondPart.pop_back();
            secondPart.push_front(shield);
        }
        VI result;
        result.reserve(n);
        bool first = true;
        while (!firstPart.empty() || !secondPart.empty()) {
            if (first) {
                auto t = firstPart.front(); firstPart.pop_front();
                result.push_back(t);
            } else {
                auto t = secondPart.front(); secondPart.pop_front();
                result.push_back(t);
            }
            first = !first;
        }
        return result;
    }
    
    vector<int> deckRevealedIncreasing(vector<int>& deck) {
        sort(deck.begin(), deck.end());
        return dfs(deck);
    }
};
