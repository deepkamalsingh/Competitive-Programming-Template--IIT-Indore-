template<int N>
struct binary_trie{

  struct node{
    int val;
    int nxt[2];
    int cnt;
    node(int val_){
      val = val_;
      nxt[0] = nxt[1] = -1;
      cnt = 0;
    }
  };

  vector<node> nodes;

  binary_trie() { nodes.push_back(node(0)); }



  void add(int x){
    int now = 0;
    nodes[now].cnt++;

    for(int i = N-1; i >= 0; --i){
      int f = x >> i & 1;
      if(nodes[now].nxt[f] < 0){
        nodes[now].nxt[f] = nodes.size();
        nodes.push_back(node(f));
        now = nodes.size() - 1;
      }else{
        now = nodes[now].nxt[f];
      }
      nodes[now].cnt++;
    }
  }

  bool del(int x){
    int now = 0;

    if(nodes[now].cnt == 0)
      return false;

    for(int i = N-1; i >= 0; --i){
      int f = x >> i & 1;
      if(nodes[now].nxt[f] < 0 || nodes[nodes[now].nxt[f]].cnt == 0){
        return false;
      }else{
        now = nodes[now].nxt[f];
      }
    }

    now = 0;
    nodes[now].cnt--;

    for(int i = N-1; i >= 0; --i){
      int f = x >> i & 1;
      now = nodes[now].nxt[f];
      nodes[now].cnt--;
    }

    return true;
  }

  int query_max(int x){
    int ret = 0;
    int now = 0;

    for(int i = N-1; i >= 0; --i){
      int f = x >> i & 1;
      if(nodes[now].nxt[!f] >= 0 && nodes[nodes[now].nxt[!f]].cnt > 0){
        ret |= 1 << i;
        now = nodes[now].nxt[!f];
      }else{
        now = nodes[now].nxt[f];
      }
    }

    return ret;
  }

  int query_min(int x){
    int ret = 0;
    int now = 0;

    for(int i = N-1; i >= 0; --i){
      int f = x >> i & 1;
      if(nodes[now].nxt[f] >= 0 && nodes[nodes[now].nxt[f]].cnt > 0){
        now = nodes[now].nxt[f];
      }else{
        ret |= 1 << i;
        now = nodes[now].nxt[!f];
      }
    }

    return ret;
  }


};