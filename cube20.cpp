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

// R * C
int visSize[8][8] = {{2, 4, 8, 16},
		     {4, 16, 64, 247},
		     {8, 64, 466, 1510},
		     {16, 247, 2510, 14893}};

// cube: {north, east, south, west}
unordered_map<int, vector<int>> roll = {{0, {0, 0, 0, 0}}, {1, {1, 32, 1, 8}}, {2, {2, 8, 2, 32}},
					{3, {3, 40, 3, 40}}, {4, {32, 4, 8, 4}}, {5, {33, 36, 9, 12}},
					{6, {34, 12, 10, 36}}, {7, {35, 44, 11, 44}}, {8, {4, 1, 16, 2}},
					{9, {5, 33, 17, 10}}, {10, {6, 9, 18, 34}}, {11, {7, 41, 19, 42}},
					{12, {36, 5, 24, 6}}, {13, {37, 37, 25, 14}}, {14, {38, 13, 26, 38}},
					{15, {39, 45, 27, 46}}, {16, {8, 16, 32, 16}}, {17, {9, 48, 33, 24}},
					{18, {10, 24, 34, 48}}, {19, {11, 56, 35, 56}}, {20, {40, 20, 40, 20}},
					{21, {41, 52, 41, 28}}, {22, {42, 28, 42, 52}}, {23, {43, 60, 43, 60}},
					{24, {12, 17, 48, 18}}, {25, {13, 49, 49, 26}}, {26, {14, 25, 50, 50}},
					{27, {15, 57, 51, 58}}, {28, {44, 21, 56, 22}}, {29, {45, 53, 57, 30}},
					{30, {46, 29, 58, 54}}, {31, {47, 61, 59, 62}}, {32, {16, 2, 4, 1}},
					{33, {17, 34, 5, 9}}, {34, {18, 10, 6, 33}}, {35, {19, 42, 7, 41}},
					{36, {48, 6, 12, 5}}, {37, {49, 38, 13, 13}}, {38, {50, 14, 14, 37}},
					{39, {51, 46, 15, 45}}, {40, {20, 3, 20, 3}}, {41, {21, 35, 21, 11}},
					{42, {22, 11, 22, 35}}, {43, {23, 43, 23, 43}}, {44, {52, 7, 28, 7}},
					{45, {53, 39, 29, 15}}, {46, {54, 15, 30, 39}}, {47, {55, 47, 31, 47}},
					{48, {24, 18, 36, 17}}, {49, {25, 50, 37, 25}}, {50, {26, 26, 38, 49}},
					{51, {27, 58, 39, 57}}, {52, {56, 22, 44, 21}}, {53, {57, 54, 45, 29}},
					{54, {58, 30, 46, 53}}, {55, {59, 62, 47, 61}}, {56, {28, 19, 52, 19}},
					{57, {29, 51, 53, 27}}, {58, {30, 27, 54, 51}}, {59, {31, 59, 55, 59}},
					{60, {60, 23, 60, 23}}, {61, {61, 55, 61, 31}}, {62, {62, 31, 62, 55}},
					{63, {63, 63, 63, 63}}};



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


inline pair<bool, pair<int, int>> checkGold(unordered_map<unsigned long long, int> &id, vector<unsigned long long> &maps,
					    int nrc, int c, unsigned long long gm, int sz) {
  //nrc = rowCol, c = cube, gm = goldMap
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

  
pair<bool, unsigned long long> dijkstra(int rc, unsigned long long gold, int R, int C, int A, int B) {
  //estado: (rowCol, cube, goldMap)

  vector<vector<unordered_map<int, int>>> vis(64, vector<unordered_map<int, int>>(64));  
  priority_queue<pair<int, State>, vector<pair<int, State>>, greater<pair<int, State>>> q;
  unordered_map<unsigned long long, int> id;
  vector<unsigned long long> maps;
  
  pair<bool, pair<int, unsigned long long>> ncgm;
  pair<bool, pair<int, int>> pnrc;
  
  int co, c, nrc, nr, nc, nC, cost, ac, sz = R * C - 1;
  unsigned long long gm, ngm;

  pair<int, State> act;
  bool flag = false;
  State ns;
  
  q.emplace(make_pair(0, State(rc, 0, 0)));
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
	  nc = roll[c][i];
	  ncgm = checkGold(id, maps, nrc, nc, maps[gm], sz);
	  nc = (ncgm.second).first;
	  ngm = (ncgm.second).second;
	  
	  if (ncgm.first)
	    ac = B;
	  cost = co + ac;
	 
	  if (vis[nrc][nc].find(ngm) == vis[nrc][nc].end() || vis[nrc][nc][ngm] > cost) {
	    vis[nrc][nc][ngm] = cost;
	    q.emplace(make_pair(cost, State(nrc, nc, ngm)));
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
