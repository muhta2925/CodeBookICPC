void constructLps(str &pat, vi &lps)
{
 
    ll len = 0;
 
    lps[0] = 0;
 
    ll i = 1;
    while (i < pat.length())
    {
 
        if (pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
            {
 
                len = lps[len - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}
vi search(string &pat, string &txt, vi &lps)
{
    ll n = txt.length();
    ll m = pat.length();
 
    // vi lps(m);
    vi res;
 
    ///  constructLps(pat, lps);
 
    ll i = 0;
    ll j = 0;
 
    while (i < n)
    {
        if (txt[i] == pat[j])
        {
            i++;
            j++;
 
            if (j == m)
            {
                res.push_back(i - j);
                j = lps[j - 1];
            }
        }
 
        else
        {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }
    return res;
}