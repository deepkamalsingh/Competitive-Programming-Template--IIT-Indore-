// #include "disjoint set union"

struct edge{
  int to, id;
};

// returns node that form a 2 edge connected component
vector<vector<int>> two_edge_cc(const vector<vector<edge>> &g){

  int n = g.size();

  vector<int> depth(n,-1), low(n);
  DSU dsu(n);

  auto dfs = [&](auto self,int node,int parent_id) -> bool {
    low[node] = depth[node];
    for(auto [child,id] : g[node])
      if(id != parent_id){
        if(depth[child] < 0){
          depth[child] = depth[node] + 1;
          if(!self(self,child,id))
            dsu.merge(node,child);
          low[node] = min(low[node],low[child]);
        }else{
          low[node] = min(low[node],depth[child]);
        }
      }
        
      
    return low[node] == depth[node];
  }; 

  for(int i = 0; i < n; ++i)
    if(depth[i] < 0){
      depth[i] = 0;
      dfs(dfs,i,-1);
    }

  return dsu.groups();
}
