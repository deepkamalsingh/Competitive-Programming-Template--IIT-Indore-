struct dsu_with_rollbacks{

  int n, comp;
  vector<int> parent_or_size;
  stack<pair<pair<int,int>,int>> stk;

  dsu_with_rollbacks(int n) : n(n), comp(n), parent_or_size(n, -1){}

  bool merge(int a, int b){
    int x = leader(a), y = leader(b);
    if(x == y) 
      return false;
    if(-parent_or_size[x] < -parent_or_size[y]) 
      swap(x, y);
    stk.push({{x,y},-parent_or_size[y]});
    comp--;
    parent_or_size[x] += parent_or_size[y];
    parent_or_size[y] = x;
    return true;
  }

  bool same(int a, int b){
    return leader(a) == leader(b);
  }

  int leader(int a){
    return parent_or_size[a] < 0 ? a : leader(parent_or_size[a]);
  }

  int size(int a){
    return -parent_or_size[leader(a)];
  }

  void rollback(){
    assert(!stk.empty());
    auto [p,z] = stk.top(); stk.pop();
    auto [x,y] = p;
    parent_or_size[x] += z;
    parent_or_size[y] = -z; 
    comp++;
  }
};
