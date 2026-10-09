struct AhoCorasick {
    struct Node {
        int next[26], fail = 0;
        int words = 0, out = 0;

        Node() {
            fill(next, next + 26, -1);
        }
    };

    vector<Node> trie;

    AhoCorasick() {
        trie.emplace_back();
    }

    // Insert pattern, return terminal node
    int insert(const string &s) {
        int node = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (trie[node].next[c] == -1) {
                trie[node].next[c] = trie.size();
                trie.emplace_back();
            }
            node = trie[node].next[c];
        }
        trie[node].words++;
        return node;
    }

    // Call once after inserting all patterns
    void build() {
        queue<int> q;
        trie[0].out = trie[0].words;

        for (int c = 0; c < 26; c++) {
            int ch = trie[0].next[c];
            if (ch == -1) {
                trie[0].next[c] = 0;
            } else {
                trie[ch].fail = 0;
                q.push(ch);
            }
        }

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            int f = trie[node].fail;
            trie[node].out = trie[node].words + trie[f].out;

            for (int c = 0; c < 26; c++) {
                int ch = trie[node].next[c];

                if (ch == -1) {
                    trie[node].next[c] = trie[f].next[c];
                } else {
                    trie[ch].fail = trie[f].next[c];
                    q.push(ch);
                }
            }
        }
    }

    // ans[i] = number of patterns ending at index i
    vector<int> search(const string &s) {
        vector<int> ans(s.size());
        int node = 0;

        for (int i = 0; i < (int)s.size(); i++) {
            node = trie[node].next[s[i] - 'a'];
            ans[i] = trie[node].out;
        }
        return ans;
    }
};