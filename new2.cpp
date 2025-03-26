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
    
    int n, m, t;
    cin >> n >> m >> t;

    mt19937 rng(time(0));
    cout<<"Preferences:"<<endl;
    int cnt=0;
    for (int trial = 0; trial < t; ++trial) {
        vector<vector<int>> permutations;
        for (int voter = 0; voter < m; ++voter) {
            vector<int> perm(n);
            for (int i = 0; i < n; ++i)
                perm[i] = i;
            shuffle(perm.begin(), perm.end(), rng);
            permutations.push_back(perm);
            cout<<"Voter "<<cnt+1<<":"<<endl;
            cnt++;
            for(auto it:perm) cout<<it+1<<" ";
            cout<<endl;
        }
        cout<<"Results"<<endl;

        // Plurality
        vector<int> plurality_counts(n, 0);
        for (const auto& perm : permutations) {
            int top = perm[0];
            plurality_counts[top]++;
        }
        int plurality_winner = 0;
        int max_plurality = plurality_counts[0];
        for (int i = 1; i < n; ++i) {
            if (plurality_counts[i] > max_plurality) {
                max_plurality = plurality_counts[i];
                plurality_winner = i;
            } else if (plurality_counts[i] == max_plurality && i < plurality_winner) {
                plurality_winner = i;
            }
        }

        // Borda Count
        vector<int> borda_scores(n, 0);
        for (const auto& perm : permutations) {
            for (int pos = 0; pos < n; ++pos) {
                int candidate = perm[pos];
                borda_scores[candidate] += (n - 1 - pos);
            }
        }
        int borda_winner = 0;
        int max_borda = borda_scores[0];
        for (int i = 1; i < n; ++i) {
            if (borda_scores[i] > max_borda) {
                max_borda = borda_scores[i];
                borda_winner = i;
            } else if (borda_scores[i] == max_borda && i < borda_winner) {
                borda_winner = i;
            }
        }

        // Copeland
        vector<vector<int>> counts(n, vector<int>(n, 0));
        for (const auto& perm : permutations) {
            for (int k = 0; k < n; ++k) {
                int a = perm[k];
                for (int l = k + 1; l < n; ++l) {
                    int b = perm[l];
                    counts[a][b]++;
                }
            }
        }

        vector<double> copeland(n, 0.0);
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int votes_i = counts[i][j];
                int votes_j = counts[j][i];
                if (votes_i > votes_j) {
                    copeland[i] += 1.0;
                } else if (votes_j > votes_i) {
                    copeland[j] += 1.0;
                } else {
                    copeland[i] += 0.5;
                    copeland[j] += 0.5;
                }
            }
        }

        vector<int> copeland_winners;
        double max_copeland = -1;
        for (int i = 0; i < n; ++i) {
            if (copeland[i] > max_copeland) {
                max_copeland = copeland[i];
                copeland_winners.clear();
                copeland_winners.push_back(i);
            } else if (copeland[i] == max_copeland) {
                copeland_winners.push_back(i);
            }
        }

        shuffle(copeland_winners.begin(), copeland_winners.end(), rng);
        int copeland_winner = copeland_winners[0];

        cout << "plurality_winner: "<<plurality_winner+1 << " " << "borda_winner: "<<borda_winner+1 << " " << "copeland_winner "<<copeland_winner+1 << endl;
    }
    cerr << "Run Time : " << ((double)(clock() - z) / CLOCKS_PER_SEC);

    return 0;
}