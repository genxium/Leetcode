bool const debug = false;

/*
test cases
3
4
10
15
*/
class Solution {
private:
    int singleLayerLocalSum(int layer) {
        return (layer & 1) ? (layer+1)/2*layer : layer/2*(layer+1);
    }
    
public:
    int minimumBoxes(int n) {
        /*
        Assume that we start building at (x: 0, y: 0, z: 0), then define "layer = x+y+z", at each layer, we have totally "1+2+...+layer = (1+layer)*layer/2" cubes when fully filled.
        */
        
        vector<int> layerLocalSums ={0};
        vector<int> layerLocalPs = {0}; // prefixSum of "layerLocalSums" 
        int sum = 0;
        for (int layer = 1; sum <= n; ++layer) {
            int localLayerSum = singleLayerLocalSum(layer);
            sum += localLayerSum;
            layerLocalSums.push_back(localLayerSum);
            layerLocalPs.push_back(layerLocalPs.back()+localLayerSum);
            if (debug) {
                printf("layerLocalSums[layer:%d] == %d, layerLocalPs[layer:%d] == %d\n", layer, layerLocalSums[layer], layer, layerLocalPs[layer]);
            }
        }

        auto psLowerIt = lower_bound(layerLocalPs.begin(), layerLocalPs.end(), n);
        if (*psLowerIt == n) {
            int fullLayerIdx = (int)(psLowerIt - layerLocalPs.begin()); 
            return singleLayerLocalSum(fullLayerIdx);
        }
        
        /*
        Assume that the input is "n == 15", we first build "prevFullLayerIdx == 3" full layers of 10 cubes, then find a valid path to fit "residue == 5" cubes. 
        ```
        3 2 1
        2 1 
        1
        ```
        The residue can be filled by using only 3 onfloor cubes, and 2 piled up cubes.
        ```
        3 2 1
        2 2 1 
        2 1
        1
        ```
        The key idea is to notice that to fill "residue == 5" between "prevFullLayerIdx == 3 & currentfullLayerIdx == 4", adding each onfloor cube extends the "capacity for residue@currentfullLayerIdx" to "singleLayerLocalSum(# of onfloor cubes @currentfullLayerIdx)".
        */
        --psLowerIt;
        int prevFullLayerIdx = (int)(psLowerIt - layerLocalPs.begin());
        int residue = n - *psLowerIt;
        if (debug) {
            printf("*psLowerIt is %d, prevFullLayerIdx is %d, residue is %d\n", *psLowerIt, prevFullLayerIdx, residue);
        }
        auto interFullLayerOnFloorIt = lower_bound(layerLocalSums.begin(), layerLocalSums.end(), residue);
        int interFullLayerOnFloorCount = (int)(interFullLayerOnFloorIt-layerLocalSums.begin()); 

        int onfloorCount = interFullLayerOnFloorCount + singleLayerLocalSum(prevFullLayerIdx);
        
        return onfloorCount;
    }
};
