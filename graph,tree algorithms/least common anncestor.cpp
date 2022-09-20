struct LCA {
  int n, lg;
  vector<vector<pair<int,int>>> adj;
  vector<int> depth, parent, euler, tour, tour_end, tour_start;
  vector<vector<pair<int,int>>> jump;
  
  LCA(int n = 0){
    init(n);
  }

  void init(int _n){
    n = _n;
    lg = __lg(n);
    adj.assign(n, {});
    depth.resize(n);
    parent.resize(n);
    tour.resize(n);
    tour_end.resize(n);
    tour_start.resize(n);
    euler.clear();
    jump.assign(n, vector<pair<int,int>> (lg + 1));
  }
  
  void add_edge(int u,int v,int w = 1){
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);
    
    adj[u].emplace_back(v,w);
    adj[v].emplace_back(u,w);
  }

  int timer;
  
  void dfs(int x){

    tour_start[x] = timer++;
    tour[tour_start[x]] = x;

    euler.push_back(x);
    
    for(auto [y,w] : adj[x])
      if(y != parent[x]){
        depth[y] = depth[x] + 1;
        parent[y] = x;
        jump[y][0] = {x,w};
        for(int j = 1; j <= lg; ++j)
          jump[y][j] = {jump[jump[y][j-1].first][j-1].first,jump[y][j-1].second + jump[jump[y][j-1].first][j-1].second};
        dfs(y);

        euler.push_back(x);
      }

    tour_end[x] = timer;
  }
  
  void build(int R = 0){
    assert(0 <= R && R < n);

    timer = 0;
    for(int j = 0; j <= lg; ++j)
      jump[R][j] = {R,0};
    parent[R] = R;
    depth[R] = 0;
    dfs(R);
  }
  
  int lca(int u, int v){
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);

    if(depth[u] < depth[v])
      swap(u,v);
    
    u = find_kth_ancestor(u, depth[u] - depth[v]).first;

    if(u == v)
      return u;

    for(int j = lg; j >= 0; --j)
      if(jump[u][j].first != jump[v][j].first)
        u = jump[u][j].first, v = jump[v][j].first;
    
    assert(jump[u][0].first == jump[v][0].first);
    return jump[u][0].first;
  }   
  
  int dist(int u, int v,bool weighted = true){
    assert(0 <= u && u < n);
    assert(0 <= v && v < n);

    if(weighted){
      const int x = lca(u,v);

      int ret = 0;

      for(auto y : {u,v})
        ret += find_kth_ancestor(y,depth[y] - depth[x]).second;

      return ret;
    }else
      return depth[u] + depth[v] - 2 * depth[lca(u,v)];
  }

  pair<int,int> find_kth_ancestor(int x,int k){
    assert(0 <= x && x < n);
    assert(0 <= k && k < (1 << (lg + 1)));
    
    int dis = 0;
    for(int j = 0; j <= lg; ++j)
      if(k >> j & 1)
        dis += jump[x][j].second, x = jump[x][j].first;
    return {x, dis};
  }
};