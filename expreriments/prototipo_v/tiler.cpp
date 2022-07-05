// g++ -std=c++11 -O3 -o tiler tiler.cpp
#include <algorithm>
#include <numeric>
#include <iostream>
#include <random>
#include <vector>
#include <iterator>
#include <chrono>

using namespace std;

void vprint(const vector<int> v) {
  copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));
//  for(auto i: v) cout << i << ' ';
  cout << endl;
}

// shuffle lgn indexes
void vshuffle(vector<int> & v, vector<int>::size_type n, mt19937 & generator) {
  int max = v.size();
  for(int i = 0; i < n; i++) {
    uniform_int_distribution<> dis(i, max - 1);
    int random = dis(generator);
    swap(v[i], v[random]);
  }
}

int main() {
  const int v_size = 40000;
  vector<int> v(v_size);
  vector<int> f(v_size);
  vector<int> lgn = {22, 33};
  int t_size = 30;
  int iter   = 2;

  cout << "lgn :"; vprint(lgn);
// just once
  iota(v.begin(), v.end(), 0);
  for(int i = 0; i != lgn.size(); i++) {	// swap lgn to the end
    swap(v[lgn[i]],  v[v.size() - i - 1]);
  }
  v.erase(v.end() - lgn.size(), v.end());	// erase lgn indexes

  unsigned seed = chrono::high_resolution_clock::now().time_since_epoch().count();
  mt19937 generator(seed);

// main loop
while(iter > 0) {
  shuffle(v.begin(), v.end(), generator);	// shuffle

  cout << "----" << iter << "----" << endl;
  vprint(v);
  cout << "---------" << endl;
//------------------------------

  int tn = 0;
  int k  = t_size - lgn.size();
  vector<int> out;
  //out.reserve(v.size());

  while(true) {
    out = lgn;
    int max = (tn + 1) * k;
    if(max <= v.size()) {
      out.insert(out.end(), v.begin() + (tn * k), v.begin() + (tn * k) + k);
      vshuffle(out, lgn.size(), generator);
      vprint(out);
    } else {
      out.insert(out.end(), v.begin() + (tn * k), v.end());
      int need = out.size() - lgn.size();
      for(int i = 0; i <  max - v.size(); i++) out.push_back(v[i]);  // or copy
      vshuffle(out, lgn.size(), generator);
      vprint(out);
      break;
    }
    tn++;
  }
 iter--;
}
}

