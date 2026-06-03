#include<iostream>

using namespace std;

//0 = vertical, 1 = horizontal
int moves[4][2] = {{0, 0}, {1, 2}, {2, 4}, {3, 5}};

//0 = north, 1 = east, 2 = south, 3 = west
int G[6][4] = {{3, 5, 1, 4}, {0, 1, 2, 1}, {1, 4, 3, 5}, {2, 3, 0, 3}, {4, 0, 4, 2}, {5, 2, 5, 0}};

inline int assign(int n, bool flag, int pos, int bits) {
  int ans;
  if (flag)
    ans = n | (1 << (bits - pos));
  else
    ans = n & ~(1 << (bits - pos));
  return ans;
}

inline bool verBit2(int n, int pos, int bits) {
  return n & (1 << (bits - pos));
}

int roll(int d, int c) {
  //d = direction, c = cube
  
  int pos, m = d % 2, ans = c;
  for (int i = 0; i < 4; ++i) {
    pos = moves[i][m];
    ans = assign(ans, verBit2(c, G[pos][d], 5), pos, 5);
  }
  return ans;
}


int main() {
  int act, ans;
  for (int i = 0; i < 64; ++i) {
    cout << '{' << i << ", {";
    for (int j = 0; j < 3; ++j) {
      ans = roll(j, i);
      cout << ans << ", ";
    }
    ans = roll(3, i);
    if (i != 63)
      cout << ans << "}}, ";
    else
      cout << ans << "}}";
	}
  return 0;
}
