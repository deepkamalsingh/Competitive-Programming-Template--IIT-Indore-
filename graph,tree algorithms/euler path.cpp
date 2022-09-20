// pair = {destination, edge index}
// for undirected graph forward/backward edges have the same index
vector<int> euler_walk(vector<vector<pair<int,int>>> graph, int m, int src){
  int n = graph.size();
  vector<int> D(n), its(n), eu(m), ret, s = {src};
  D[src]++;
  while(!s.empty()){
    int x = s.back(), y, e, &it = its[x], end = graph[x].size();
    if(it == end){
      ret.push_back(x);
      s.pop_back();
      continue;
    }
    tie(y,e) = graph[x][it++];
    if(!eu[e]){
      D[x]--,D[y]++;
      eu[e] = 1;
      s.push_back(y);
    }
  }
  for(auto x : D) if(x < 0 || ret.size() != m + 1) return {};
  return {ret.rbegin(), ret.rend()};
}
