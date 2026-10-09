struct SuffixArray {
    vector<int> suffix_indices, suffix_rank, lcp;
    vector<vector<int>> sparse_table;

    SuffixArray(const string &s) {
        build(s);
    }

    // suffix_indices[i] = start of ith smallest suffix
    void build(const string &s) {
        suffix_indices.clear();
        suffix_rank.clear();
        lcp.clear();
        sparse_table.clear();

        int n = s.size();
        if (n == 0) return;

        string t = s + char(0);
        int m = t.size();

        vector<int> suffix_class(m), new_class(m);
        vector<int> new_indices(m), cnt(max(256, m), 0);
        suffix_indices.resize(m);

        for (unsigned char ch : t) cnt[ch]++;
        for (int i = 1; i < 256; i++)
            cnt[i] += cnt[i - 1];

        for (int i = 0; i < m; i++)
            suffix_indices[--cnt[(unsigned char)t[i]]] = i;

        suffix_class[suffix_indices[0]] = 0;
        int classes = 1;

        for (int i = 1; i < m; i++) {
            if (t[suffix_indices[i]] != t[suffix_indices[i - 1]])
                classes++;
            suffix_class[suffix_indices[i]] = classes - 1;
        }

        for (int h = 0; (1LL << h) < m; h++) {
            int len = 1 << h;

            for (int i = 0; i < m; i++) {
                new_indices[i] = suffix_indices[i] - len;
                if (new_indices[i] < 0)
                    new_indices[i] += m;
            }

            fill(cnt.begin(), cnt.begin() + classes, 0);

            for (int i = 0; i < m; i++)
                cnt[suffix_class[new_indices[i]]]++;

            for (int i = 1; i < classes; i++)
                cnt[i] += cnt[i - 1];

            for (int i = m - 1; i >= 0; i--)
                suffix_indices[--cnt[suffix_class[new_indices[i]]]]
                    = new_indices[i];

            new_class[suffix_indices[0]] = 0;
            classes = 1;

            for (int i = 1; i < m; i++) {
                pair<int, int> cur = {
                    suffix_class[suffix_indices[i]],
                    suffix_class[(suffix_indices[i] + len) % m]
                };

                pair<int, int> prev = {
                    suffix_class[suffix_indices[i - 1]],
                    suffix_class[(suffix_indices[i - 1] + len) % m]
                };

                if (cur != prev) classes++;
                new_class[suffix_indices[i]] = classes - 1;
            }

            suffix_class.swap(new_class);
        }

        suffix_indices.erase(suffix_indices.begin());

        suffix_rank.resize(n);
        for (int i = 0; i < n; i++)
            suffix_rank[suffix_indices[i]] = i;

        build_lcp(s);
    }

    // lcp[i] = LCP of suffix_indices[i] and [i+1]
    void build_lcp(const string &s) {
        int n = s.size(), k = 0;
        lcp.assign(max(0, n - 1), 0);

        for (int i = 0; i < n; i++) {
            if (suffix_rank[i] == n - 1) {
                k = 0;
                continue;
            }

            int j = suffix_indices[suffix_rank[i] + 1];

            while (i + k < n && j + k < n &&
                   s[i + k] == s[j + k])
                k++;

            lcp[suffix_rank[i]] = k;
            if (k) k--;
        }
    }

    // Call before get_lcp() or compare()
    void build_sparse_table() {
        int n = lcp.size();
        sparse_table.clear();
        if (n == 0) return;

        int levels = __lg(n) + 1;
        sparse_table.assign(levels, vector<int>(n));
        sparse_table[0] = lcp;

        for (int k = 1; k < levels; k++) {
            for (int i = 0; i + (1 << k) <= n; i++) {
                sparse_table[k][i] = min(
                    sparse_table[k - 1][i],
                    sparse_table[k - 1][i + (1 << (k - 1))]
                );
            }
        }
    }

    // LCP of suffixes starting at indices i and j
    int get_lcp(int i, int j) {
        int n = suffix_indices.size();
        if (i == j) return n - i;

        int left = suffix_rank[i];
        int right = suffix_rank[j];
        if (left > right) swap(left, right);

        int k = __lg(right - left);
        return min(
            sparse_table[k][left],
            sparse_table[k][right - (1 << k)]
        );
    }

    // Compare s[l1..r1] and s[l2..r2]
    // Returns -1 (smaller), 0 (equal), 1 (greater)
    int compare(int l1, int r1, int l2, int r2) {
        int len1 = r1 - l1 + 1;
        int len2 = r2 - l2 + 1;

        int common = min({
            get_lcp(l1, l2), len1, len2
        });

        if (common == min(len1, len2)) {
            if (len1 == len2) return 0;
            return len1 < len2 ? -1 : 1;
        }

        return suffix_rank[l1] < suffix_rank[l2] ? -1 : 1;
    }
};