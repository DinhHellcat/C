class Solution {
public:
    string longestPalindrome(string s) {
        vector<int> a(255, -1);
        int len = s.length();
        int maxL = 0, first = 0;
        string temp;
        for(int i=0;i<len;i++)
        {
            char c = s[i];
            if(a[c]>=first) first=a[c]+1;
            a[c]=i;
            if(max(maxL,i-first+1)==i-first+1)
                temp = s.substr(first,i-first+1);
            maxL = max(maxL,i-first+1);
        }
        return temp;
    }
};