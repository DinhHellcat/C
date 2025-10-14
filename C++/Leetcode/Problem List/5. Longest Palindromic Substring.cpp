class Solution {
public:
    string longestPalindrome(string s) {
        if(s.length()<1)
            return "";
        int maxL = 1;
        int start = 0;
        for(int i=0;i<s.length();i++)
        {
            int len1 = expandFromCenter(s,i,i);
            int len2 = expandFromCenter(s,i,i+1);
            int maxLrn = max(len1,len2);
            if(maxLrn>maxL)
            {
                maxL = maxLrn;
                start = i - (maxL-1)/2;
            }
        }
        return s.substr(start,maxL);
    }
    int expandFromCenter(const std::string& s, int left, int right) {
        while(left>=0 && right<s.length() && s[left]==s[right])
        {
            left--;
            right++;
        }
        return right - left - 1;
    }
};
