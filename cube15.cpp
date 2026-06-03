/*
Proyecto final

Nombre: Isabel Sofía Adrada Arboleda, Juan Luis Guevara Bustamante
Código de estudiante: 8977586,
*/

#include<cstdio>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;


//0 = north, 1 = east, 2 = south, 3 = west
int dire[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

//0 = north, 1 = east, 2 = south, 3 = west
int G[6][4] = {{3, 5, 1, 4}, {0, 1, 2, 1}, {1, 4, 3, 5}, {2, 3, 0, 3}, {4, 0, 4, 2}, {5, 2, 5, 0}};

//0 = vertical, 1 = horizontal
int moves[4][2] = {{0, 0}, {1, 2}, {2, 4}, {3, 5}};

int visSize[8][8] = {{2, 4, 8, 16, 32, 64, 127, 247},
		     {4, 16, 64, 247, 848, 2510, 6476, 14893},
		     {8, 64, 466, 2510, 9949, 31180, 82160, 190051},
		     {16, 247, 2510, 14893, 60460, 190051, 499178, 1149017},
		     {32, 848, 9949, 60460, 245506, 768212, 2007328, 4598479},
		     {64, 2510, 31180, 190051, 768212, 2391496, 6220768, 14196869},
		     {127, 6476, 82160, 499178, 2007328, 6220768, 16122226, 36684859},
		     {247, 14893, 190051, 1149017, 4598479, 14196869, 36684859, 83278001}};

class State {
  //estado: (rowCol, cube, goldMap)
  
private:
  int rc;
  int cube;
  int gm;  
  
public:
  State() {
  };
  
  State(int n, int c, int m) {
    rc = n;
    cube = c;
    gm = m;
  };

  int getRc() const {
    return rc;
  };

  int getCube() const {
    return cube;
  };

  int getGm() const {
    return gm;
  };

  bool operator <(const State &oth) const {   
    return false;
   };
};

inline bool verBit(unsigned long long n, int pos, int bits) {
  return n & (1 << (bits - pos));
}

inline unsigned long long swap(unsigned long long n, int pos, int bits) {
  return  n ^ (1 << (bits - pos));
}


inline bool verBit2(int n, int pos, int bits) {
  return n & (1 << (bits - pos));
}

inline int swap2(int n, int pos, int bits) {
  return  n ^ (1 << (bits - pos));
}

inline int assign(int n, bool flag, int pos, int bits) {
  int ans;
  if (flag)
    ans = n | (1 << (bits - pos));
  else
    ans = n & ~(1 << (bits - pos));
  return ans;
}

inline int fn(int r, int c, int C) {
  return r * C + c;
}

inline pair<int, int> f(int k, int C) {
  int r = k / C;
  int c = k - r * C;
  return make_pair(r, c);
}

inline pair<bool, pair<int, int>> move(int d, int rc, int R, int C) {
  //d = direction, rc = rowCol
  
  pair<int, int> prc = f(rc, C);
  int nr = prc.first + dire[d][0];
  int nc = prc.second + dire[d][1];
  bool flag = nr >= 0 && nr < R && nc >= 0 && nc < C;
  pair<bool, pair<int, int>> ans = make_pair(flag, make_pair(nr, nc));
  return ans;
}

inline int roll(int d, int c) {
  //d = direction, c = cube
  
  int pos, m = d % 2, ans = c;
  for (int i = 0; i < 4; ++i) {
    pos = moves[i][m];
    ans = assign(ans, verBit2(c, G[pos][d], 5), pos, 5);
  }
  return ans;
}

inline pair<bool, pair<int, int>> checkGold(unordered_map<unsigned long long, int> &id, vector<unsigned long long> &maps,
					    int nrc, int c, unsigned long long gm, int sz) {
  //nrc = rowCol, c = cube, gm = goldMap
  //0 = cara de abajo

  bool ans = false, flag = verBit2(c, 0, 5);
  int ngm;
  
  if (verBit(gm, nrc, sz)) {
    if (!flag) {
      c = swap2(c, 0, 5);
      gm = swap(gm, nrc, sz);
      ans = true;
    }
  } else {
    if (flag) {
      c = swap2(c, 0, 5);
      gm = swap(gm, nrc, sz);
    }
  }
  if (id.find(gm) == id.end()) {
    maps.push_back(gm);
    id[gm] = maps.size() - 1;
  } 
  return make_pair(ans, make_pair(c, id[gm]));
}

  
pair<bool, unsigned long long> dijkstra(int rc, unsigned long long gold, int R, int C, int A, int B) {
  //estado: (rowCol, cube, goldMap)

  vector<vector<vector<int>>> vis(64, vector<vector<int>>(64, vector<int>(visSize[R - 1][C - 1], -1)));
  priority_queue<pair<int, State>, vector<pair<int, State>>, greater<pair<int, State>>> q;
  unordered_map<unsigned long long, int> id;
  vector<unsigned long long> maps;
  
  pair<bool, pair<int, unsigned long long>> ncgm;
  pair<bool, pair<int, int>> pnrc;
  
  int co, c, nrc, nr, nc, nC, cost, ac, isVis, sz = R * C - 1;
  unsigned long long gm, ngm;

  pair<int, State> act;
  bool flag = false;
  State ns;

  q.push(make_pair(0, State(rc, 0, 0)));
  maps.push_back(gold);
  id[gold] = 0;
  vis[rc][0][0] = 0;
  
  while (!flag && !q.empty()) {
    act = q.top();
    co = act.first;
    rc = (act.second).getRc();
    c = (act.second).getCube();
    gm = (act.second).getGm();
    q.pop();

    //printf("%d %d %d %d\n", co, rc, c, gm);
    if (c == 63) {
      //63 = 111111
      flag = true;
      //printf("%llu\n", maps[gm]);
      
    } else if (vis[rc][c][gm] == co) {
      for (int i = 0; i < 4; ++i) {
	pnrc = move(i, rc, R, C);
	nr = (pnrc.second).first;
	nC = (pnrc.second).second;
	nrc = fn(nr, nC, C);
	ac = A;
                  
	if (pnrc.first) {
	  nc = roll(i, c);
	  ncgm = checkGold(id, maps, nrc, nc, maps[gm], sz);
	  nc = (ncgm.second).first;
	  ngm = (ncgm.second).second;
	  
	  if (ncgm.first)
	    ac = B;   

	  cost = co + ac;
	  isVis = vis[nrc][nc][ngm];
	  
	  if (isVis == -1 || isVis > cost) {
	    vis[nrc][nc][ngm] = cost;
	    q.push(make_pair(cost, State(nrc, nc, ngm)));
	  }
	}
      }
    }
  }
  return make_pair(flag, co);
}

int main() {
  int T, R, C, A, B, rc, gCount;
  pair<bool, unsigned long long> res;
  unsigned long long gold = 0;
  bool flag = false;
  char aux[9];

  scanf("%d", &T);
  for (int i = 0; i < T; ++i) {
    scanf("%d %d %d %d", &R, &C, &A, &B);
    gold = 0;
    gCount = 0;
    rc = -1;
    flag = false;
            
    for (int j = 0; j < R; ++j) {
      scanf("%s", aux);
      for (int k = 0; !flag && k < C; ++k) {
	if (aux[k] == 'G') {
	  gold = swap(gold, fn(j, k, C), R * C - 1);
	  gCount++;
	} else if (aux[k] == 'S')
	  rc = fn(j, k, C);
	flag = gCount == 6 && rc != -1;
      }
    }
            
    res = dijkstra(rc, gold, R, C, A, B);
            
    if (res.first)
      printf("Screw you guys, I got all the gold for %lld cost!\n", res.second);
    else
      printf("Oh my God, they killed Kenny!\n");
  } 
  return 0;
}
