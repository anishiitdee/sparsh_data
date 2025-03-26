#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

int main() {
    clock_t z = clock();

#ifndef ONLINE_JUDGE
    freopen("input.txt",  "r",  stdin);
    freopen("output.txt", "w", stdout);
#endif
    
    int t;
    cin >> t;

    mt19937 rng(time(0));
    uniform_int_distribution<int> dist_n(1, 10); // Ensuring n >= 1
    uniform_int_distribution<int> dist_m(1, 500);
    cout<<t<<" trials:"<<endl;
    int curr=0;
    int n,m;
    for (int trial = 0; trial < t; ++trial) {
        if(curr%25==0) {
                n = dist_n(rng);
                m = dist_m(rng);
        }
        cout<<m<<" voters "<<n<<" candidates"<<endl;
        cout << "Preferences:" << endl;
        vector<vector<int>> permutations;
        int cnt=0;
        for (int voter = 0; voter < m; ++voter) {
            vector<int> perm(n);
            for (int i = 0; i < n; ++i)
                perm[i] = i;
            shuffle(perm.begin(), perm.end(), rng);
            permutations.push_back(perm);
            cout << "Voter " << cnt + 1 << ":" << endl;
            cnt++;
            curr++;
            for (auto it : perm) cout << it + 1 << " ";
            cout << endl;
        }
        cout << "Results" << endl;

        // Plurality
        vector<int> plurality_counts(n, 0);
        for (const auto& perm : permutations) {
            int top = perm[0];
            plurality_counts[top]++;
        }
        int plurality_winner = max_element(plurality_counts.begin(), plurality_counts.end()) - plurality_counts.begin();

        // Borda Count
        vector<int> borda_scores(n, 0);
        for (const auto& perm : permutations) {
            for (int pos = 0; pos < n; ++pos) {
                borda_scores[perm[pos]] += (n - 1 - pos);
            }
        }
        int borda_winner = max_element(borda_scores.begin(), borda_scores.end()) - borda_scores.begin();

        // Copeland
        vector<vector<int>> counts(n, vector<int>(n, 0));
        for (const auto& perm : permutations) {
            for (int k = 0; k < n; ++k) {
                for (int l = k + 1; l < n; ++l) {
                    counts[perm[k]][perm[l]]++;
                }
            }
        }

        vector<double> copeland(n, 0.0);
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (counts[i][j] > counts[j][i]) copeland[i]++;
                else if (counts[j][i] > counts[i][j]) copeland[j]++;
                else {
                    copeland[i] += 0.5;
                    copeland[j] += 0.5;
                }
            }
        }

        vector<int> copeland_winners;
        double max_copeland = *max_element(copeland.begin(), copeland.end());
        for (int i = 0; i < n; ++i) {
            if (copeland[i] == max_copeland) copeland_winners.push_back(i);
        }

        shuffle(copeland_winners.begin(), copeland_winners.end(), rng);
        int copeland_winner = copeland_winners[0];

        cout << "plurality_winner: " << plurality_winner + 1 << " "
             << "borda_winner: " << borda_winner + 1 << " "
             << "copeland_winner " << copeland_winner + 1 << endl;
    }
    cerr << "Run Time : " << ((double)(clock() - z) / CLOCKS_PER_SEC);
    return 0;
}
