int const INVALID = -1;
int const EXTRY = -1;
int const EXIT = +1;

int const SUPPLY = +1;
int const DEMAND = -1;

bool debug = false;

class StockDiff {
public:
  int pos, amount; // "pos" means "position" 
  StockDiff(int aPos, int aAmount) {
    pos = aPos;
    amount = aAmount; 
  }
};

class Transmission {
public:
  int fromPos, toPos, amount; // "pos" means "position" 
  Transmission(int aFromPos, int aToPos, int aAmount) {
    fromPos = aFromPos;
    toPos = aToPos;  
    amount = aAmount;
  }
};

typedef vector<int> IntervalBoundary;
class IntervalBoundaryCompare {
public:
  bool operator() (IntervalBoundary const& lhs, IntervalBoundary const& rhs) {
    if (lhs[0] != rhs[0]) return lhs[0] < rhs[0];
    // now that "lhs[0] == rhs[0]"
    
    // In this solution, there WOULDN'T be any position, either a "fromPos" or a "toPos", that acts both as a "SUPPLY" and a "DEMAND", i.e. when "lhs[0] == rhs[0]" it's certain that "lhs[3] == rhs[3]".
    if (DEMAND == lhs[3]) {
      return lhs[1] > rhs[1]; // "receiving 2 dresses from different directions at the same step" is allowed, thus EXIT comes before ENTRY, see "test case#11"
    } else {
      return lhs[1] < rhs[1]; // ENTRY comes before EXIT
    }
  }
};

/*
test case#1
[2,1,1,1,1,0,1,1,1]

test case#2
[2,1,1,1,1,0,1,1,1,0,1,1,1,1,2]

test case#3
[6,0,4,0,0]

test case#4
[9,9,3,3]

test case#5
[15,0,9,0]

test case#6
[0,0,9,15]

test case#7
[0,0,9,17,14,0,8,0]

test case#8
[3,1,0,4]

test case#9
[0,0,10,0,0,0,10,0,0,0]

test case#10
[0,0,10,0,10,0,10,0,0,0]

test case#11
[9,1,8,8,9]
*/
class Solution {
private:
  void printTransmission(Transmission &transmission) {
    printf("%d -> %d: %d\n", transmission.fromPos, transmission.toPos, transmission.amount);
  }

  vector<Transmission> calcTransmissions(vector<int> &diffs) {
    vector<Transmission> toRet; 
    list<StockDiff> positives, negatives;
    int n = diffs.size();
    for (int pos = 0; pos < n; ++pos) {
      if (debug) printf("At pos:%d\n", pos);
      int diff = diffs[pos];
      if (0 == diff) continue;
      if (0 < diff) {
        while (0 < diff && !negatives.empty()) {
          auto &demand = negatives.back();  
          if (demand.amount <= diff) {
            Transmission trans(pos, demand.pos, demand.amount);
            toRet.push_back(trans);

            diff -= demand.amount; 
            negatives.pop_back();

          } else {
            // demand.amount > diff 
            Transmission trans(pos, demand.pos, diff);
            toRet.push_back(trans);

            demand.amount -= diff;
            diff = 0;
          }        
        }  
        if (0 < diff) {
          StockDiff positive(pos, abs(diff));
          positives.push_back(positive); 
        }
      } else {
        // 0 > diff 
        while (0 > diff && !positives.empty()) {
          auto &supply = positives.back();  
          if (supply.amount <= abs(diff)) {
            Transmission trans(supply.pos, pos, supply.amount);
            toRet.push_back(trans);

            diff += supply.amount; 
            positives.pop_back();
          } else {
            // supply.amount > abs(diff) 
            Transmission trans(supply.pos, pos, abs(diff));
            toRet.push_back(trans);

            supply.amount -= abs(diff);
            diff = 0;
          }        
        } 
        if (0 > diff) {
          StockDiff negative(pos, abs(diff));
          negatives.push_back(negative); 
        }
      }
      if (debug) printf("\tended, positives.size() == %d, negatives.size() == %d\n", positives.size(), negatives.size());
    }
    return toRet;
  }

  int calcParallelTransmissionCost(vector<Transmission> &transmissions) {
    /*
    * Now that a wash machine could be covered by multiple transmissions, in the fastest solution we should keep "every wash machine that will eventually make the move" actually moving EVERY EARLY ROUND.
    */
    if (debug) {
      for (auto &transmission : transmissions) {
        printTransmission(transmission);
      }
    }
    vector<IntervalBoundary> intervalBoundaries;
    for (auto &tr : transmissions) {
      int trLeft = min(tr.fromPos, tr.toPos), trRight = max(tr.fromPos, tr.toPos); 
      intervalBoundaries.push_back({trLeft, EXTRY, tr.amount, (trLeft == tr.fromPos ? SUPPLY : DEMAND)});
      intervalBoundaries.push_back({trRight, EXIT, tr.amount, (trRight == tr.fromPos ? SUPPLY : DEMAND)});
    }
    
    sort(intervalBoundaries.begin(), intervalBoundaries.end(), IntervalBoundaryCompare());
  
    int result = 0;
    int acc = 0; // Within "the densest intersection" of "activeInterval".
    for (auto &boundary : intervalBoundaries) {
      acc += (-boundary[1])*(boundary[2]);
      result = max(acc, result);
    }

    return result;
  }

public:
  int findMinMoves(vector<int>& machines) {
    /*
    Consider only 1 transmission object(i.e. "2 -> 0") from "test case#1", it takes only 1 step, because all the "1"s between "2" and "0" and help the transmission at the same step. Moreover, consider "test case#2" which has 2 transmission objects(i.e. "2 -> 0" on the left and "0 <- 2" on the right), yet also takes only 1 step to complete because the transmission objects have no overlap.
    
    In the case like "test case#2"
    
    2_________0____0___________________________________________________________2
       a        b                                              c

    , we should avoid any 2 transmissions with different directions overlapping.
    
    In fact, we should avoid overlapping regardless of the direction of transmission. In the case like "test case#3"

    6_________0____4____________________________0______________________________0
       a        b               c                              d                               

    , we should be greedy to make a transmission from/to the nearest possible supply/demand. To be quantitative, there're 2 different "transmission schemes" to fulfill demands
    - schm#1
      0 -> 1: 2 dresses
      0 -> 3: 2 dresses
      2 -> 4: 2 dresses
      
    - schm#2
      0 -> 1: 2 dresses, 2 -> 3: 2 dresses
      0 -> 4: 2 dresses
    
    and "schm#2" is better.
    */
    int sum = 0, n = machines.size(); 
    for (int i = 0; i < n; ++i) {
      sum += machines[i];
    }
    if (sum % n != 0) {
      return INVALID;
    } 
    int mean = sum/n;
    vector<int> diffs;
    for (int i = 0; i < n; ++i) {
      diffs.push_back(machines[i]-mean);
      if (debug) {
        printf("diffs[%d] == %d\n", i, diffs[i]);
      }
    }
    vector<Transmission> transmissions = calcTransmissions(diffs); 
    int ans = calcParallelTransmissionCost(transmissions);
    return ans;
  }
};

