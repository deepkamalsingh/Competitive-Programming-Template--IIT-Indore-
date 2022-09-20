// https://atcoder.github.io/ac-library/master/document_en/maxflow.html

template<class F>
struct mf_graph{

  public:
    mf_graph(int n = 0) : _n(n), g(n) {}

    int add_edge(int from, int to, F cap){
      assert(0 <= from && from < _n);
      assert(0 <= to && to < _n);
      assert(0 <= cap);
      int m = pos.size();
      pos.push_back({from,(int)g[from].size()});
      int from_id = g[from].size(), to_id = g[to].size();
      if(from == to) to_id++;
      g[from].push_back({to,to_id,cap});
      g[to].push_back({from,from_id,0});
      return m;
    } 

    struct edge{
      int from, to;
      F cap, flow;
    };

    edge get_edge(int i){
      assert(0 <= i && i < (int)pos.size());
      auto _e = g[pos[i].first][pos[i].second];
      auto _re = g[_e.to][_e.rev];
      return edge{pos[i].first, _e.to, _e.cap + _re.cap, _re.cap};
    }

    vector<edge> edges(){
      vector<edge> res;
      for(int i = 0; i < (int)pos.size(); ++i)
        res.push_back(get_edge(i));
      return res;
    }

    F flow(int s,int t){
      return flow(s,t,numeric_limits<F>::max());
    }

    F flow(int s,int t,int flow_limit){
      assert(0 <= s && s < _n);
      assert(0 <= t && t < _n);
      assert(s != t);

      vector<int> level(_n), iter(_n);

      auto bfs = [&]() -> void {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);

        while(!q.empty()){
          int v = q.front(); q.pop();
          for(auto e : g[v]){
            if(e.cap == 0 || level[e.to] >= 0) continue;
            level[e.to] = level[v] + 1;
            if(e.to == t) return;
            q.push(e.to);
          }
        }
      };

      auto dfs = [&](auto self,int v,int up) -> F {
        if(v == s)  return up;
        F res = 0;
        int level_v = level[v];
        for(int& i = iter[v]; i < (int)g[v].size(); ++i){
          auto e = g[v][i];
          if(level_v <= level[e.to] || g[e.to][e.rev].cap == 0)
            continue;
          F d = self(self,e.to,min(up - res, g[e.to][e.rev].cap));
          if(d <= 0) continue;
          g[v][i].cap += d;
          g[e.to][e.rev].cap -= d;
          res += d;
          if(res == up) break;
        }
        return res;
      };

      F flow = 0;
      while(flow < flow_limit){
        bfs();
        if(level[t] == -1) break;
        fill(iter.begin(), iter.end(), 0);
        while(flow < flow_limit){
          F f = dfs(dfs,t,flow_limit - flow);
          if(!f) break;
          flow += f;
        }
      }

      return flow;
    }

    vector<bool> min_cut(int s){
      vector<bool> vis(_n, false);
      queue<int> q;
      q.push(s);
      while(!q.empty()){
        int v = q.front(); q.pop();
        vis[v] = true;
        for(auto e : g[v]){
          if(e.cap && !vis[e.to]){
            vis[e.to] = true;
            q.push(e.to);
          }
        }
      }
      return vis;
    }

  private:
    int _n;
    struct _edge{
      int to, rev;
      F cap;
    };
    vector<pair<int,int>> pos;
    vector<vector<_edge>> g;
};

template<typename F>
using mf_edge = typename::mf_graph<F>::edge;