class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        vector<int> temp(256,-1);
        int dem=0;
        int first=0;
        int n=s.length();
        for(int i=0;i<n;i++)
        {
            char c=s[i];
            if(temp[c]>=first)
                first=temp[c]+1;
            temp[c]=i;
            dem=max(dem,i-first+1);
        }
        return dem;
    }
};
