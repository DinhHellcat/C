#include <vector>
#include <algorithm>

class Solution {
public:
    int findPoisonedDuration(vector<int>& timeSeries, int duration) {
        if(timeSeries.empty())
            return 0;
        int total=duration;
        for(int i=0;i<timeSeries.size()-1;i++)
            total+=min(duration,timeSeries[i+1]-timeSeries[i]);
        return total;
    }
};
