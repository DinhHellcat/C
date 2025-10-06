class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int m=nums1.size(), n=nums2.size();
        if(m>n)
            return findMedianSortedArrays(nums2,nums1);

        int low = 0;
        int high = m;
        int half = (m+n+1)/2;

        while(low<=high)
        {
            int p1 = low + (high-low)/2;
            int p2 = half - p1;

            int max_left1 = (p1 == 0)?numeric_limits<int>::min():nums1[p1-1];
            int min_right1 = (p1 == m)?numeric_limits<int>::max():nums1[p1];

            int max_left2 = (p2 == 0)?numeric_limits<int>::min():nums2[p2-1];
            int min_right2 = (p2 == n)?numeric_limits<int>::max():nums2[p2];

            if(max_left1 <= min_right2 && max_left2 <= min_right1)
            {
                if((m+n)%2==0)
                    return (max(max_left1,max_left2)+min(min_right1,min_right2))/2.0;
                else
                    return max(max_left1,max_left2);
            }
            else if(max_left1 > min_right2)
                high = p1 - 1;
            else
                low = p1 + 1;
        }
        return -1;
    }
};
