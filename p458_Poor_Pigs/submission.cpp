/*
test case#1
10
15
60

test case#2
11
15
60

test case#3
10
16
60

test case#4
11
16
60

test case#5
100
15
60
*/
class Solution {
public:
    /*
    In the most general case, if there're p pigs to drink till the end of r-th round, we can test up to (r+1)^p buckets. The strategy is that at every round each pig drinks (r+1)^(p-1) buckets, while each k pigs having (r+1)^(p-k) buckets shared and k > 1. It can be proved that if there were no poisonous bucket, over all rounds all pigs together drink exactly (r+1)^(p) buckets by the Inclusion-Exclusion Principle.
    
    - It's obviously true for "p == 1", because 1 pig can drink r buckets at the end of the r-th round.
    - For "p == 2", imagine that (r+1)^2 buckets are arranged in a square and at every round the 2 pigs drink 1 row and 1 col respectively, with 1 bucket overlapping. For every round if both pigs die, we immediately that the overlapping bucket is poisonous, else if at the end of the k-th round 1 pig dies, we use the remaining pig to drink the non-overlapping (r+1-k) buckets thus will find the answer in (k+(r-k) == r) rounds.
    - For "p == 3", imagine that (r+1)^3 buckets are arranged in a cube and at every round the 3 pigs drink 1 xy-face, 1 xz-face and 1-yz face respectively, with 1 bucket overlapping for all pigs and 1 (r+1)-segment buckets overlapping for each pair of pigs.
    - For "p == 4", imagine that (r+1)^4 buckets are arranged in an array of (r+1) (r+1)^3 cubes and at the k-th round 
      - pig#1 drinks 1 whole cube,
      - pig#2 drinks the k-th xy-face of every cube,
      - pig#3 drinks the k-th xz-face of every cube, 
      - pig#4 drinks the k-th yz-face of every cube.
      Therefore at every round 1 bucket overlapping for all pigs, (r+1) overlapping buckets for each 3-tuple of pigs, (r+1)^2 overlapping buckets for each pair of pigs.
    */
    int poorPigs(int buckets, int minutesToDie, int minutesToTest) {
        // we can test "pow(r+1, p)" buckets by using "p" pigs at the end of the "r-th" round.
        return (int)ceil(log(buckets) / (log(minutesToTest / minutesToDie + 1)));
    }
};
