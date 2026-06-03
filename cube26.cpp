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

int fn[8][8];
pair<int, int> f[64];
int moves[64][4];

unordered_map<int, int> vis[64][64];

//i: {north, east, south, west}
int roll[64][4] = {{0, 0, 0, 0}, {1, 32, 1, 8}, {2, 8, 2, 32}, {3, 40, 3, 40}, {32, 4, 8, 4}, {33, 36, 9, 12},
		   {34, 12, 10, 36}, {35, 44, 11, 44}, {4, 1, 16, 2}, {5, 33, 17, 10}, {6, 9, 18, 34}, {7, 41, 19, 42},
		   {36, 5, 24, 6}, {37, 37, 25, 14}, {38, 13, 26, 38}, {39, 45, 27, 46}, {8, 16, 32, 16}, {9, 48, 33, 24},
		   {10, 24, 34, 48}, {11, 56, 35, 56}, {40, 20, 40, 20}, {41, 52, 41, 28}, {42, 28, 42, 52}, {43, 60, 43, 60},
		   {12, 17, 48, 18}, {13, 49, 49, 26}, {14, 25, 50, 50}, {15, 57, 51, 58}, {44, 21, 56, 22}, {45, 53, 57, 30},
		   {46, 29, 58, 54}, {47, 61, 59, 62}, {16, 2, 4, 1}, {17, 34, 5, 9}, {18, 10, 6, 33}, {19, 42, 7, 41},
		   {48, 6, 12, 5}, {49, 38, 13, 13}, {50, 14, 14, 37}, {51, 46, 15, 45}, {20, 3, 20, 3}, {21, 35, 21, 11},
		   {22, 11, 22, 35}, {23, 43, 23, 43}, {52, 7, 28, 7}, {53, 39, 29, 15}, {54, 15, 30, 39}, {55, 47, 31, 47},
		   {24, 18, 36, 17}, {25, 50, 37, 25}, {26, 26, 38, 49}, {27, 58, 39, 57}, {56, 22, 44, 21}, {57, 54, 45, 29},
		   {58, 30, 46, 53}, {59, 62, 47, 61}, {28, 19, 52, 19}, {29, 51, 53, 27}, {30, 27, 54, 51}, {31, 59, 55, 59},
		   {60, 23, 60, 23}, {61, 55, 61, 31}, {62, 31, 62, 55}, {63, 63, 63, 63}};

class State {
  // (cost, rowCol, cube, goldMap)

private:
  int cost;
  int rc;
  int cube;
  unsigned long long gm;

public:
  State() {
  };
  
  State(int co, int n, int c, unsigned long long m) {
    cost = co;
    rc = n;
    cube = c;
    gm = m;
  };

  int getCost() const {
    return cost;
  }

  int getRc() const {
    return rc;
  };

  int getCube() const {
    return cube;
  };

  unsigned long long getGm() const {
    return gm;
  };

  bool operator >(const State &oth) const {
    return cost > oth.cost;
  }
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

inline int genFn(int r, int c, int C) {
  return r * C + c;
}

inline pair<int, int> genF(int k, int C) {
  int r = k / C;
  int c = k - r * C;
  return make_pair(r, c);
}

inline pair<bool, pair<int, int>> genMove(int d, int rc, int R, int C) {
  pair<int, int> prc = f[rc];
  int nr = prc.first + dire[d][0];
  int nc = prc.second + dire[d][1];
  bool flag = nr >= 0 && nr < R && nc >= 0 && nc < C;
  pair<bool, pair<int, int>> ans = make_pair(flag, make_pair(nr, nc));
  return ans;
}

inline pair<bool, pair<int, unsigned long long>> checkGold(unordered_map<unsigned long long, int> &id, vector<unsigned long long> &maps,
							   int nrc, int c, unsigned long long gm, int sz) {
  //0 = cara de abajo
  bool ans = false, flag = verBit2(c, 0, 5);
  
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

void precalc(int R, int C) {
  pair<bool, pair<int, int>> moveAux;
  pair<int, int> moveAuxF;
  int fAux;
  
  for (int j = 0; j < R; ++j) {
    for (int k = 0; k < C; ++k) {
      fAux = genFn(j, k, C);
      fn[j][k] = fAux;
      f[fAux] = make_pair(j, k);
    }
  }

  for (int j = 0; j < R * C; ++j) {
    for (int k = 0; k < 4; ++k) {
      moveAux = genMove(k, j, R, C);
      moveAuxF = moveAux.second;
	
      if (moveAux.first)
	moves[j][k] = fn[moveAuxF.first][moveAuxF.second];
      else
	moves[j][k] = -1;
    }
  }
}

unsigned long long dijkstra(int rc, unsigned long long gold, int R, int C, int A, int B) {
  //estado: (rowCol, cube, goldMap)

  for (int i = 0; i < R * C; ++i)
    for (int j = 0; j < 64; ++j)
      vis[i][j].clear();

  priority_queue<State, vector<State>, greater<State>> q;
  pair<bool, pair<int, unsigned long long>> ncgm;

  unordered_map<unsigned long long, int> id;
  vector<unsigned long long> maps;  

  int co, c, nrc, nc, cost, ac, sz = R * C - 1;
  unsigned long long gm, ngm;
  
  bool flag = false;
  State act;
  
  q.emplace(State(0, rc, 0, 0));
  maps.push_back(gold);
  id[gold] = 0;
  vis[rc][0][gold] = 0;
  
  while (!flag && !q.empty()) {
    act = q.top();
    co = act.getCost();
    rc = act.getRc();
    c = act.getCube();
    gm = act.getGm();
    q.pop();
    
    if (c == 63) {
      //63 = 111111
      flag = true;
      
    } else if (vis[rc][c][gm] == co){
      for (int i = 0; i < 4; ++i) {
	nrc = moves[rc][i];
	
	if (nrc != -1) {
	  nc = roll[c][i];
	  ncgm = checkGold(id, maps, nrc, nc, maps[gm], sz);
	  nc = (ncgm.second).first;
	  ngm = (ncgm.second).second;
	  ac = A;
	         
	  if (ncgm.first)
	    ac = B;   
	  cost = co + ac;

	  if (vis[nrc][nc].find(ngm) == vis[nrc][nc].end() || vis[nrc][nc][ngm] > cost) {
	    vis[nrc][nc][ngm] = cost;
	    q.emplace(State(cost, nrc, nc, ngm));
	  }
	}
      }
    }
  }
  
  if (!flag)
    co = -1;
  return co;
}


int main() {
  int T, R, C, A, B, rc, gCount, lR = -1, lC = -1;
  unsigned long long res, gold = 0;
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
	  gold = swap(gold, genFn(j, k, C), R * C - 1);
	  gCount++;
	} else if (aux[k] == 'S')
	  rc = genFn(j, k, C);
	flag = gCount == 6 && rc != -1;
      }
    }

    if (lR != R || lC != C)
      precalc(R, C);
    
    res = dijkstra(rc, gold, R, C, A, B);
    lR = R;
    lC = C;
            
    if (res != -1)
      printf("Screw you guys, I got all the gold for %lld cost!\n", res);
    else
      printf("Oh my God, they killed Kenny!\n");
  } 
  return 0;
}
