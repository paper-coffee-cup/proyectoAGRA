#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <tuple>
#include <string>

using namespace std;

bool verBit(unsigned long long n, int pos, int bits) {
    bool ans = (n & (1ULL << (bits - pos))) != 0;
    return ans;
}

unsigned long long swap_bit(unsigned long long n, int pos, int bits) {
    unsigned long long ans = n ^ (1ULL << (bits - pos));
    return ans;
}

unsigned long long assign(unsigned long long n, bool flag, int pos, int bits) {
    unsigned long long ans = 0;
    if (flag) {
        ans = n | (1ULL << (bits - pos));
    } else {
        ans = n & ~(1ULL << (bits - pos));
    }
    return ans;
}

long long fn(long long r, long long c, long long C) {
    long long ans = r * C + c;
    return ans;
}

pair<long long, long long> f(long long k, long long C) {
    long long r = k / C;
    long long c = k - r * C;
    pair<long long, long long> ans = make_pair(r, c);
    return ans;
}

pair<bool, pair<long long, long long>> move_fn(int d, pair<long long, long long> rc, long long R, long long C) {
    int dire[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    pair<long long, long long> nrc = make_pair(rc.first + dire[d][0], rc.second + dire[d][1]);
    bool flag = nrc.first >= 0 && nrc.first < R && nrc.second >= 0 && nrc.second < C;
    pair<bool, pair<long long, long long>> ans = make_pair(flag, nrc);
    return ans;
}

unsigned long long roll(int d, unsigned long long c) {
    int move_arr[4][2] = {{0, 0}, {1, 2}, {2, 4}, {3, 5}};
    int m = d % 2;
    unsigned long long ans = c;
    for (int i = 0; i < 4; i++) {
        int pos = move_arr[i][m];
        ans = assign(ans, verBit(ans, pos, 5), pos, 5);
    }
    return ans;
}

tuple<bool, unsigned long long, unsigned long long> checkGold(
    pair<long long, long long> nrc, unsigned long long c, unsigned long long gm, long long R, long long C) {
    bool ans_bool = false;
    unsigned long long ans_c = c;
    unsigned long long ans_gm = gm;
    bool flag = verBit(c, 0, 5);
    if (verBit(gm, fn(nrc.first, nrc.second, C), R * C - 1)) {
        if (!flag) {
            ans_c = swap_bit(c, 0, 5);
            ans_gm = swap_bit(gm, fn(nrc.first, nrc.second, C), R * C - 1);
            ans_bool = true;
        }
    } else {
        if (flag) {
            ans_c = swap_bit(c, 0, 5);
            ans_gm = swap_bit(gm, fn(nrc.first, nrc.second, C), R * C - 1);
        }
    }
    tuple<bool, unsigned long long, unsigned long long> ans = make_tuple(ans_bool, ans_c, ans_gm);
    return ans;
}

struct State {
    long long cost;
    long long r, c;
    unsigned long long cube;
    unsigned long long gm;
    
    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

pair<bool, long long> dijkstra(pair<long long, long long> rc, unsigned long long gold, long long R, long long C, long long A, long long B) {
    priority_queue<State, vector<State>, greater<State>> q;
    map<tuple<long long, long long, unsigned long long, unsigned long long>, long long> vis;
    
    State act = {0, rc.first, rc.second, 0, gold};
    vis[make_tuple(rc.first, rc.second, 0, gold)] = 0;
    
    bool flag = false;
    long long cost = 0;
    
    q.push(act);
    
    while (!flag && !q.empty()) {
        State current = q.top();
        q.pop();
        
        long long co = current.cost;
        pair<long long, long long> curr_rc = make_pair(current.r, current.c);
        unsigned long long c = current.cube;
        unsigned long long gm = current.gm;
        
        int i = 0;
        tuple<long long, long long, unsigned long long, unsigned long long> state_tuple = make_tuple(current.r, current.c, c, gm);
        
        while (!flag && i < 4 && vis[state_tuple] == co) {
            cout << co << " (" << curr_rc.first << ", " << curr_rc.second << ") " << c << " " << gm << "\n";
            pair<bool, pair<long long, long long>> move_res = move_fn(i, curr_rc, R, C);
            bool nflag = move_res.first;
            pair<long long, long long> nrc = move_res.second;
            
            long long ac = A;
            
            if (nflag) {
                unsigned long long nc = roll(i, c);
                tuple<bool, unsigned long long, unsigned long long> g_res = checkGold(nrc, nc, gm, R, C);
                bool gflag = get<0>(g_res);
                nc = get<1>(g_res);
                unsigned long long ngm = get<2>(g_res);
                
                if (gflag) {
                    ac = B;
                }
                
                tuple<long long, long long, unsigned long long, unsigned long long> ns = make_tuple(nrc.first, nrc.second, nc, ngm);
                cost = co + ac;
                
                bool insert_flag = false;
                if (vis.count(ns) == 0) {
                    insert_flag = true;
                } else if (vis[ns] > cost) {
                    insert_flag = true;
                }
                
                if (insert_flag) {
                    if (nc == 63) {
                        flag = true;
                    } else {
                        vis[ns] = cost;
                        q.push({cost, nrc.first, nrc.second, nc, ngm});
                    }
                }
            }
            i++;
        }
    }
    pair<bool, long long> ans = make_pair(flag, cost);
    return ans;
}

int main() {
    int T;
    int ans_main = 0;
    if (cin >> T) {
        for (int i = 0; i < T; i++) {
            long long R, C, A, B;
            cin >> R >> C >> A >> B;
            
            unsigned long long gold = 0;
            int gCount = 0;
            pair<long long, long long> rc = make_pair(-1, -1);
            bool flag = false;
            
            for (int j = 0; j < R; j++) {
                string aux;
                cin >> aux;
                int k = 0;
                while (!flag && k < C && k < (int)aux.length()) {
                    if (aux[k] == 'G') {
                        gold = swap_bit(gold, fn(j, k, C), R * C - 1);
                        gCount++;
                    } else if (aux[k] == 'S') {
                        rc = make_pair(j, k);
                    }
                    flag = (gCount == 6 && rc.first != -1);
                    k++;
                }
            }
            
            pair<bool, long long> res = dijkstra(rc, gold, R, C, A, B);
            bool res_flag = res.first;
            long long res_cost = res.second;
            
            if (res_flag) {
                cout << "Screw you guys, I got all the gold for " << res_cost << " cost!\n";
            } else {
                cout << "Oh my God, they killed Kenny!\n";
            }
        }
    }
    return ans_main;
}
