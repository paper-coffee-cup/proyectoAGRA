/*
Proyecto final

Nombre: Isabel Sofía Adrada Arboleda, Juan Luis Guevara Bustamante
Código de estudiante: 8977586,
*/

#include<cstdio>
#include <vector>
#include<set>
#include <queue>
#include <unordered_map>

using namespace std;

//0 = north, 1 = east, 2 = south, 3 = west
int dire[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

//0 = north, 1 = east, 2 = south, 3 = west
int G[6][4] = {{3, 5, 1, 4}, {0, 1, 2, 1}, {1, 4, 3, 5}, {2, 3, 0, 3}, {4, 0, 4, 2}, {5, 2, 5, 0}};

//0 = vertical, 1 = horizontal
int moves[4][2] = {{0, 0}, {1, 2}, {2, 4}, {3, 5}};

class State {
  //estado: (rowCol, cube, goldMap)
  
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

  bool operator <(const State &oth) const {
    bool ans;
    if (cost != oth.cost) {
      ans = cost < oth.cost;
    } else if (rc != oth.rc) {
      ans = rc < oth.rc;
    } else if (cube != oth.cube) {
      ans = cube < oth.cube;
    } else {
      ans = gm < oth.gm;
    }
    return ans;
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
  pair<int, int> prc = f(rc, C);
  int nr = prc.first + dire[d][0];
  int nc = prc.second + dire[d][1];
  bool flag = nr >= 0 && nr < R && nc >= 0 && nc < C;
  pair<bool, pair<int, int>> ans = make_pair(flag, make_pair(nr, nc));
  return ans;
}

inline int roll(int d, int c) {
  int pos, m = d % 2, ans = c;
  for (int i = 0; i < 4; ++i) {
    pos = moves[i][m];
    ans = assign(ans, verBit2(c, G[pos][d], 5), pos, 5);
  }
  return ans;
}

inline pair<bool, pair<int, unsigned long long>> checkGold(int nrc, int c, unsigned long long gm, int sz) {
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
  return make_pair(ans, make_pair(c, gm));
}

const int TAMANIO = 64;
using MapaElementos = std::unordered_map<unsigned long long, int>;

void imprimirMatrizDeMapas(const MapaElementos matriz[TAMANIO][TAMANIO]) {
  for (int i = 0; i < TAMANIO; ++i) {
    for (int j = 0; j < TAMANIO; ++j) {
      printf("[%d][%d]: { ", i, j);
            
      // Declaración explícita del iterador constante
      MapaElementos::const_iterator it;
            
      // Recorrido clásico del mapa usando iteradores
      for (it = matriz[i][j].begin(); it != matriz[i][j].end(); ++it) {
	// Al usar iteradores se accede con el operador flecha (->)
	printf("%llu -> %d, ", it->first, it->second);
      }
            
      printf("}  ");
    }
    printf("\n");
  }
}

pair<bool, unsigned long long> dijkstra(int rc, unsigned long long gold, int R, int C, int A, int B) {
  //estado: (rowCol, cube, goldMap)
 
  unordered_map<unsigned long long, int> vis[64][64];
  unordered_map<unsigned long long, int> elm;
  unordered_map<unsigned long long, int>::iterator it2;
  pair<bool, pair<int, unsigned long long>> ncgm;
  pair<bool, pair<int, int>> pnrc;
  int co, c, nrc, nr, nc, nC, cost, ac, sz = R * C - 1;
  unsigned long long gm, ngm;
  set<State> q;
  set<State>::iterator it;
  bool flag = false;
  State act, ns = State(0, rc, 0, gold);

  vis[rc][0][gold] = 0;
  q.insert(ns);
    
  while (!flag && !q.empty()) {
    it = q.begin();
    act = (*it);
    co = act.getCost();
    rc = act.getRc();
    c = act.getCube();
    gm = act.getGm();
    q.erase(it);
      
    if (c == 63) {
      //63 = 111111
      flag = true;
      
    } else if (vis[rc][c][gm] == co){
      for (int i = 0; i < 4; ++i) {
	pnrc = move(i, rc, R, C);
	nr = (pnrc.second).first;
	nC = (pnrc.second).second;
	nrc = fn(nr, nC, C);
	ac = A;
                  
	if (pnrc.first) {
	  nc = roll(i, c);
	  ncgm = checkGold(nrc, nc, gm, sz);
	  nc = (ncgm.second).first;
	  ngm = (ncgm.second).second;
	                
	  if (ncgm.first)
	    ac = B;
	  cost = co + ac;
	  ns = State(cost, nrc, nc, ngm);
	  it2 = vis[nrc][nc].find(ngm);

	  if (it2 == vis[nrc][nc].end() || vis[nrc][nc][ngm] > cost) {
	    if (it2 != vis[nrc][nc].end())
	      q.erase(State(elm[ngm], nrc, nc, ngm));
	    vis[nrc][nc][ngm] = cost;
 	    q.insert(ns);
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

