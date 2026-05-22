/*
Tarea 1
A - Problem A

Nombre: Isabel Sofía Adrada Arboleda, Juan Luis Guevara Bustamante
Código de estudiante: 8977586,
*/

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

class State {
  //estado: (rowCol, cube, goldMap)
  
private:
  unsigned long long rc;
  unsigned long long cube;
  unsigned long long gm;  
  
public:
  State() {
  };
  
  State(unsigned long long n, unsigned long long c, unsigned long long m) {
    rc = n;
    cube = c;
    gm = m;
  };

  unsigned long long getRc() const {
    return rc;
  };

  unsigned long long getCube() const {
    return cube;
  };

  unsigned long long getGm() const {
    return gm;
  };

  bool operator == (const State &oth) const {
    return rc == oth.rc && cube == oth.cube && gm == oth.gm;
  };
};

namespace std {
  template <>
  class hash<State> {
  public:
    size_t operator()(const State &s) const {
      //Obtener el hash de cada componente
      size_t h1 = hash<unsigned long long>{}(s.getRc());
      size_t h2 = hash<unsigned long long>{}(s.getCube());
      size_t h3 = hash<unsigned long long>{}(s.getGm());

      //Combinar los tres hashes consecutivamente (Algoritmo de Boost)
      size_t ans = h1;
      ans ^= h2 + 0x9e3779b9 + (ans << 6) + (ans >> 2);
      ans ^= h3 + 0x9e3779b9 + (ans << 6) + (ans >> 2);

      return ans;
    }
  };
}

bool verBit(unsigned long long n, int pos, int bits) {
    bool ans = (n & (1 << (bits - pos))) != 0;
    return ans;
}

unsigned long long swap(unsigned long long n, int pos, int bits) {
    unsigned long long ans = n ^ (1 << (bits - pos));
    return ans;
}

unsigned long long assign(unsigned long long n, bool flag, int pos, int bits) {
    unsigned long long ans;
    if (flag)
        ans = n | (1 << (bits - pos));
    else
        ans = n & ~(1 << (bits - pos));
    return ans;
}

unsigned long long fn(unsigned long long r, unsigned long long c, int C) {
    unsigned long long ans = r * C + c;
    return ans;
}

pair<unsigned long long, unsigned long long> f(unsigned long long k, int C) {
    unsigned long long r = k / C;
    unsigned long long c = k - r * C;
    pair<unsigned long long, unsigned long long> ans = make_pair(r, c);
    return ans;
}

 pair<bool, pair<unsigned long long,  unsigned long long>> move(int d, unsigned long long rc, int R, int C) {
  //0 = north, 1 = east, 2 = south, 3 = west
  int dire[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  
  pair<unsigned long long, unsigned long long> prc = f(rc, C);
  unsigned long long nr = prc.first + dire[d][0];
  unsigned long long nc = prc.second + dire[d][1];
  bool flag = nr >= 0 && nr < R && nc >= 0 && nc < C;
  pair<bool, pair<unsigned long long,  unsigned long long>> ans = make_pair(flag, make_pair(nr, nc));
  return ans;
}

unsigned long long roll(int d, unsigned long long c) {
  //0 = north, 1 = east, 2 = south, 3 = west
  int G[6][4] = {{3, 5, 1, 4}, {0, 1, 2, 1}, {1, 4, 3, 5}, {2, 3, 0, 3}, {4, 0, 4, 2}, {5, 2, 5, 0}};
  //0 = vertical, 1 = horizontal
  int move[4][2] = {{0, 0}, {1, 2}, {2, 4}, {3, 5}};
  
  int pos, m = d % 2;
  unsigned long long ans = c;
  
  for (int i = 0; i < 4; ++i) {
    pos = move[i][m];
    ans = assign(ans, verBit(c, G[pos][d], 5), pos, 5);
  }
  return ans;
}

 pair<bool, pair< unsigned long long, unsigned long long>> checkGold(unsigned long long nr, unsigned long long nC,
								     unsigned long long c, unsigned long long gm, int R, int C) {
   //0 = cara de abajo
   
   bool ans = false, flag = verBit(c, 0, 5);
   
   if (verBit(gm, fn(nr, nC, C), R * C - 1)) {
     if (!flag) {
       c = swap(c, 0, 5);
       gm = swap(gm, fn(nr, nC, C), R * C - 1);
       ans = true;
     }
   } else {
     if (flag) {
       c = swap(c, 0, 5);
       gm = swap(gm, fn(nr, nC, C), R * C - 1);
     }
   }
   return make_pair(ans, make_pair(c, gm));
 }



pair<bool, unsigned long long> dijkstra(unsigned long long rc, unsigned long long gold, int R, int C, int A, int B) {
  //estado: (rowCol, cube, goldMap)

  priority_queue<vector<unsigned long long>, vector<vector<unsigned long long>>, greater<vector<unsigned long long>>> q;
  unordered_map<State, unsigned long long> vis;
  vector<unsigned long long> nxt, act = {0, rc, 0, gold};
  unsigned long long co, c, gm, nrc, nc, ngm, nr, nC, cost = 0;
  State ns;
  bool flag = false;
  int ac;

  vis[State(rc, 0, gold)] = 0;
  q.push(act);
    
  while (!flag && !q.empty()) {
    act = q.top();
    co = act[0];
    rc = act[1];
    c = act[2];
    gm = act[3];
    q.pop();
                
    for (int i = 0; !flag && i < 4 && vis[State(rc, c, gm)] == co; ++i) {
      pair<bool, pair<unsigned long long,  unsigned long long>> pnrc = move(i, rc, R, C);
      nr = (pnrc.second).first;
      nC = (pnrc.second).second;
      nrc = fn(nr, nC, C);
      ac = A;
                  
      if (pnrc.first) {
        nc = roll(i, c);
	pair<bool, pair< unsigned long long,  unsigned long long>> ncgm = checkGold(nr, nC, nc, gm, R, C);
	nc = (ncgm.second).first;
	ngm = (ncgm.second).second;
	                
	if (ncgm.first)
	  ac = B;
                
        ns = State(nrc, nc, ngm);
	cost = co + ac;

	if (vis.find(ns) == vis.end() || vis[ns] > cost) {
	  //63 = 111111
	  if (nc == 63)
	    flag = true;
	  else {
	    vis[ns] = cost;
	    nxt = {cost, nrc, nc, ngm};
	    q.push(nxt);
	  }
	}
      }
    }
  }
  return make_pair(flag, cost);
}

int main() {
  int T, R, C, A, B, gCount;
  unsigned long long rc, gold = 0;
  bool flag = false;
  string aux;
  
  cin >> T;
  for (int i = 0; i < T; i++) {
    cin >> R >> C >> A >> B;        
    gold = 0;
    gCount = 0;
    rc = -1;
    flag = false;
            
    for (int j = 0; j < R; j++) {
      cin >> aux;
      for (int k = 0; !flag && k < C; ++k) {
	if (aux[k] == 'G') {
	  gold = swap(gold, fn(j, k, C), R * C - 1);
	  gCount++;
	} else if (aux[k] == 'S')
	  rc = fn(j, k, C);
	flag = gCount == 6 && rc != -1;
      }
    }
            
    pair<bool, unsigned long long> res = dijkstra(rc, gold, R, C, A, B);
            
    if (res.first)
      cout << "Screw you guys, I got all the gold for " << res.second << " cost!" << endl;
    else
      cout << "Oh my God, they killed Kenny!" << endl;
  } 
  return 0;
}
