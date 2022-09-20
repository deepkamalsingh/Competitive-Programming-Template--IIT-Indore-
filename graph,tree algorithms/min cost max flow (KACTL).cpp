// https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/MinCostMaxFlow.h
 
// #include <bits/extc++.h>
 
template<class Cap,class Cost>
struct MCMF{
 
  public:
   
    struct edge{
      int to;
      Cap cap, flow;
      Cost cost;
    };
   
    MCMF(int N) : N(N), ed(N), red(N), seen(N), dist(N), pi(N), par(N) {}
   
    // cap[i][j] != cap[j][i] is allowed; double edges and self loops are not.
    void add_edge(int from,int to,Cap cap,Cost cost){
      int m = int(edges.size());
      edges.push_back({to,cap,0,cost});
      ed[from].push_back(m);
      edges.push_back({from,0,0,0});
      red[to].push_back(m+1);
    }
   
    void path(int s){
      fill(seen.begin(), seen.end(), 0);
      fill(dist.begin(), dist.end(), INF_cost);
      dist[s] = 0;
      Cost di;
   
      __gnu_pbds::priority_queue<pair<long long, int>> q;
      vector<decltype(q)::point_iterator> its(N);
      q.push({0, s});
   
      auto relax = [&](int e_id,int i,Cap cap,Cost cost,int dir){
        Cost val = di - pi[i] + cost;
        if(cap && val < dist[i]){
          dist[i] = val;
          par[i] = {s, e_id, dir};
          if(its[i] == q.end()) its[i] = q.push({-dist[i], i});
          else q.modify(its[i], {-dist[i], i});
        }
      };
   
      while(!q.empty()){
        s = q.top().second; q.pop();
        seen[s] = 1; di = dist[s] + pi[s];
        for(int i : ed[s]){
          edge e = edges[i];
          if(!seen[e.to]) relax(i, e.to, e.cap - e.flow, e.cost, 1);
        } 
        for(int i : red[s]){
          edge e = edges[i];
          if(!seen[e.to]) relax(i, e.to, edges[i^1].flow, -edges[i^1].cost, 0);
        } 
      }
   
      for(int i = 0; i < N; ++i) pi[i] = min(pi[i] + dist[i], INF_cost);
    }
   
    pair<Cap,Cost> max_flow(int s,int t){
      Cap tot_flow = 0;
      Cost tot_cost = 0;
      
      while(path(s), seen[t]){
        
        Cap fl = INF_cap;
        for(int p,e,r,x = t; tie(p,e,r) = par[x], x != s; x = p)
          fl = min(fl, r ? edges[e].cap - edges[e].flow : edges[e^1].flow);
      
        tot_flow += fl;
        for(int p,e,r,x = t; tie(p,e,r) = par[x], x != s; x = p)
          r ? (edges[e].flow += fl) : (edges[e^1].flow -= fl);
        
      }
   
      for(edge e : edges)
        tot_cost += e.cost * e.flow;
   
      return {tot_flow, tot_cost};
    }
   
    // If some costs can be negative, call this before maxflow.
    void set_pi(int s){ // (otherwise, leave this out).
      fill(pi.begin(), pi.end(), INF_cost); pi[s] = 0;
      int it = N, ch = 1;
      Cost v;
      while(ch-- && it--){
        for(int i = 0; i < N; ++i)
          if(pi[i] != INF_cost){
            for(int e : ed[i])
              if(edges[e].cap){
                if((v = pi[i] + edges[e].cost) < pi[edges[e].to]){
                  pi[edges[e].to] = v, ch = 1;
                }
              }
          }
      }
      assert(it >= 0); // negative cost cycle.
    }


  private:

    const Cap INF_cap = numeric_limits<Cap>::max() / 4;
    const Cost INF_cost = numeric_limits<Cost>::max() / 4;

    int N;
    vector<vector<int>> ed, red;
    vector<edge> edges;
    vector<Cost> dist, pi;
    vector<int> seen;
    vector<tuple<int,int,int>> par;

};

template<class Cap,class Cost>
using flow_edge = typename::MCMF<Cap,Cost>::edge;