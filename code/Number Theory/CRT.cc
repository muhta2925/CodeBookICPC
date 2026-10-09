ll egcd(ll a, ll b, ll &x, ll &y){
  if(b == 0) { x=1, y=0; return a;}
  ll g = egcd(b, a%b, y, x);
  y -= a/b*x;  return g;
}
ll crt(ll r1, ll m1, ll r2, ll m2){
  if(m1<m2) swap(r1, r2), swap(m1, m2);
  ll p, q, g = egcd(m1, m2, p, q);
  if((r2-r1)%g !=0 )  return -1;  //no solution
  ll x = (r2-r1)%m2*p%m2*m1/g + r1;
  return x<0? x+m1*m2/g: x;
}
ll crt(vector<ll>& r, vector<ll>& m){
  ll x = r[0], M=m[0];
  for (int i = 1; i < r.size(); ++i){
    x = crt(x, M, r[i], m[i]);
    ll g = __gcd(M, m[i]);
    M = (M/g)*m[i];
  }
  return x;
}