template <typename T> 
int sgn(T x){
  return (x > 0)-(x < 0);
}

template <typename T> 
int abs(T x){
  return x > 0 ? x : -x;
}


template <typename T> 
struct Point {
  T x, y;
  Point() : x(0), y(0) {}
  Point(T x_, T y_) : x(x_), y(y_) {}
  template <typename U> explicit Point(const Point<U>& p) : x(p.x), y(p.y) {}

  Point operator + () const { return Point(+x, +y); }
  Point operator - () const { return Point(-x, -y); }

  Point& operator += (const Point& p){ x += p.x; y += p.y; return *this; }
  Point& operator -= (const Point& p){ x -= p.x; y -= p.y; return *this; }
  Point& operator *= (const T& t){ x *= t; y *= t; return *this; }
  Point& operator /= (const T& t){ x /= t; y /= t; return *this; }

  friend Point operator + (const Point& a, const Point& b){ return Point(a.x+b.x, a.y+b.y); }
  friend Point operator - (const Point& a, const Point& b){ return Point(a.x-b.x, a.y-b.y); }
  friend Point operator * (const Point& a, const T& t){ return Point(a.x*t, a.y*t); }
  friend Point operator * (const T& t, const Point& a){ return Point(t*a.x, t*a.y); }
  friend Point operator / (const Point& a, const T& t){ return Point(a.x/t, a.y/t); }

  friend bool operator == (const Point& a, const Point& b){ return a.x == b.x && a.y == b.y; }
  friend bool operator != (const Point& a, const Point& b){ return !(a==b); }

  friend Point conj(const Point& p){ return Point(p.x, -p.y); }

  friend T dot(const Point& a, const Point& b){ return a.x*b.x+a.y*b.y; }
  friend T cross(const Point& a, const Point& b){ return a.x*b.y-a.y*b.x; }
  friend T cross(const Point& O,const Point& a, const Point& b){ return cross(a-O,b-O); }
  friend T euclid(const Point& a){ return dot(a,a); }
  friend T euclid(const Point& a, const Point& b){ return euclid(a-b); }
  friend T manhattan(const Point& a, const Point& b){ return abs(a.x-b.x)+abs(a.y-b.y); }

  friend bool left_turn(const Point& a, const Point& b, const Point& c){
    return cross(b-a,c-b) > 0;
  }
};

using P = Point<int>;
ostream &operator<<(ostream &os, const P &p) { return os << "(" << p.x << "," << p.y << ")"; }