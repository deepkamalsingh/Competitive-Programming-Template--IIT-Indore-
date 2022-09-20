// #include "lattice"

// return convex_hull indices in counter-clockwise order.
vector<int> convex_hull(vector<P> points){
  int n = points.size(), ind = 0;
  for(int i = 1; i < n; ++i)
    if(points[i].y < points[ind].y || (points[i].y == points[ind].y && points[i].x > points[ind].x))
      ind = i;

  vector<int> cand, hull = {ind};
  for(int i = 0; i < n; ++i)
    if(points[i] != points[ind])
      cand.push_back(i);

  sort(cand.begin(), cand.end(), [&](auto l,auto r){
    P x = points[l] - points[ind], y = points[r] - points[ind];
    int c = cross(x,y);
    return c != 0 ? c > 0 : euclid(x) < euclid(y);
  });

  for(auto c : cand){
    while(int(hull.size()) > 1 && cross(points[end(hull)[-2]],points[hull.back()],points[c]) <= 0)
      hull.pop_back();
    hull.push_back(c);
  }
  return hull;
}
