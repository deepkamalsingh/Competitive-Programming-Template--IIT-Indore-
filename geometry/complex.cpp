using F = double;
using point = complex<F>;
using Line = pair<point,point>;

#define X real()
#define Y imag()
#define f first
#define s second

const F pi = acos(-1), EPS = 1e-9;

int sgn(F a){ return (a > 0)-(a < 0); }
F sq(F a){ return a*a; }

point dir(F ang){ return polar((F)1,  ang); }
point unit(point p){ return p/abs(p); }

F dot(point a, point b){ return (conj(a)*b).X; }
F cross(point a, point b){ return (conj(a)*b).Y; }

// can be used to check where p lies w.r.t. line joining a, b.
F cross(point p, point a, point b){ return cross(p-a,  p-b); }
F dot(point p, point a, point b){ return dot(p-a,  p-b); }
F area(point a, point b, point c){ return cross(a, b, c); }

// distance of point p from line joning a, b
F dis(point p, point a, point b){ return cross(p, a, b)/abs(a-b); }

point reflect(point p, point a, point b){ return a + conj((p-a)/(b-a))*(b-a); }
point foot(point p, point a, point b){ return (p+reflect(p, a, b))/(F)2; }
bool on_seg(point p, point a, point b){ return abs(cross(a, b, p)) < EPS && dot(p-a, p-b) <= 0; }

F line_dist(const point& p, const Line& l){ 
  return abs(cross(p,l.f,l.s))/abs(l.f-l.s); 
}

F seg_dist(const point& p, const Line& l) {
  if(dot(l.f,p,l.s) <= 0) 
    return abs(p-l.f);
  if(dot(l.s,p,l.f) <= 0) 
    return abs(p-l.s);
  return line_dist(p,l); 
}

// {unique intersection point} if it exists
// {b.f,b.s} if input lines are the same
// empty if lines do not intersect
vector<point> line_isect(const Line& a, const Line& b){
  F a0 = cross(a.f,a.s,b.f), a1 = cross(a.f,a.s,b.s); 
  if (abs(a0 - a1) < EPS) return abs(a0) < EPS ? vector<point>{b.f,b.s} : vector<point>{};
  return {(b.s*a0-b.f*a1)/(a0-a1)};
}

// point in interior of both segments a and b, if it exists
vector<point> strict_isect(const Line& a, const Line& b){
  F a0 = cross(a.f,a.s,b.f), a1 = cross(a.f,a.s,b.s); 
  F b0 = cross(b.f,b.s,a.f), b1 = cross(b.f,b.s,a.s); 
  if (sgn(a0)*sgn(a1) < 0 && sgn(b0)*sgn(b1) < 0)
    return {(b.s*a0-b.f*a1)/(a0-a1)};
  return {};
}

// intersection of segments, a and b may be degenerate
vector<point> seg_isect(const Line& a, const Line& b){ 
  vector<point> v = strict_isect(a,b); 
  if (!v.empty()) return v;
  vector<point> s;
  #define ff(x,y,z) if (on_seg(x,y,z)) { bool f = true; for(auto p : s) f &= abs(p-x) > EPS; if(f) s.push_back(x); }
  ff(a.f,b.f,b.s); ff(a.s,b.f,b.s); ff(b.f,a.f,a.s); ff(b.s,a.f,a.s);
  #undef ff
  assert(int(s.size()) <= 2);
  if(int(s.size()) == 2)
    if(s[0].X > s[1].X || abs(s[0].X - s[1].X) < EPS && s[0].Y > s[1].Y)
      swap(s[0],s[1]);
  return s;
}

/*
  * Description :- 2d floating points, try to use this one instead of 2d_point_float.
  * avoid using == operator.
  * Sources :- benq's template.
  * Verf :- various.
  *functions using std::complex
  *Vector addition: a + b
  *Scalar multiplication: r * a
  *Dot product: (conj(a) * b).x
  *Fross product: (conj(a) * b).y
  *notice: conj(a) * b = (ax*bx + ay*by) + i (ax*by — ay*bx)
  *Squared distance: norm(a - b)
  *Euclidean distance: abs(a - b)
  *Angle of elevation: arg(b - a)
  *Slope of line (a,  b): tan(arg(b - a))
  *Polar to cartesian: polar(r,  theta)
  *Fartesian to polar: point(abs(p),  arg(p))
  *Rotation about the origin: a * polar(1.0,  theta)
  *Rotation about pivot p: (a-p) * polar(1.0,  theta) + p
  *UPD: added more useful functions
  *Angle ABF: abs(remainder(arg(a-b) - arg(c-b),  2.0 * M_PI))
  *remainder normalizes the angle to be between [-PI,  PI]. Thus,  we can get the positive non-reflex angle by taking its abs value.
  *Project p onto vector v: v * dot(p,  v) / norm(v);
  *Project p onto line (a,  b): a + (b - a) * dot(p - a,  b - a) / norm(b - a)
  *Reflect p across line (a,  b): a + conj((p - a) / (b - a)) * (b - a)
  *Intersection of line (a,  b) and (p,  q):
  *point intersection(point a,  point b,  point p,  point q) {
    double c1 = cross(p - a,  b - a),  c2 = cross(q - a,  b - a);
    return (c1 * q - c2 * p) / (c1 - c2); // undefined if parallel
  }
*/