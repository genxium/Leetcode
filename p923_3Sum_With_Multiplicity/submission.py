MOD = 1000000007
debug = True

class Solution:
    def threeSumMulti(self, arr: List[int], target: int) -> int:
        ans = 0
        arr.sort()
        for i in range(len(arr)):
            j, k = i+1, len(arr)-1
            while j < k: 
                s = arr[i] + arr[j] + arr[k]
                if s > target:
                    k -= 1
                elif s < target:
                    j += 1
                else:
                    inc, anchorJ, anchorK = 0, arr[j], arr[k]
                    if arr[j] == arr[k]:
                        inc = int((k-j+1)*(k-j)/2)
                        j = k # termination
                    else:
                        incJ, incK = 0, 0
                        while arr[k] == anchorK: 
                            k -= 1
                            incK +=1 
                        while arr[j] == anchorJ:
                            j += 1
                            incJ += 1
                        inc = (incJ*incK)%MOD

                    ans = (ans + inc)%MOD

        return ans
