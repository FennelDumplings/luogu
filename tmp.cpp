#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

using ll = long long;
const int MOD = 59393;

// (4) Lucas 定理
ll exp_mod(ll a, ll b, ll p)
{
    //快速幂取模
    ll res = 1;
    while(b != 0)
    {
        if(b & 1) res = (res * a) % p;
        a = (a * a) % p;
        b >>= 1;
    }
    return res;
}

ll Comb(ll a, ll b, ll p)
{
    //求组合数C(a,b) % p
    if(a < b)   return 0;
    if(a == b)  return 1;
    if(b > a - b)   b = a - b;

    ll ans = 1, ca = 1, cb = 1;
    for(ll i = 0; i < b; ++i)
    {
        ca = (ca * (a - i)) % p;
        cb = (cb * (b - i)) % p;
    }
    ans = (ca * exp_mod(cb, p - 2, p)) % p;
    return ans;
}

ll Lucas(ll n, ll m, ll p)
{
    //Lucas定理求C(n,m)%p
    ll ans = 1;

    while(n && m && ans)
    {
        ans = (ans * Comb(n % p, m % p, p)) % p;
        n /= p;
        m /= p;
    }
    return ans;
}

const int mod=59393;

inline int read(){
	char ch=getchar();int x=0,f=0;
	while(ch<'0' || ch>'9') f|=ch=='-',ch=getchar();
	while(ch>='0' && ch<='9') x=x*10+ch-'0',ch=getchar();
	return f?-x:x;
}
#define FOR(i,a,b) for(int i=(a);i<=(b);i++)

struct pos{
	int x,y;
	bool operator<(const pos &p)const{
		if(x!=p.x) return x<p.x;
		return y<p.y;
	}
}p[22];
int n,m,k,x[22],y[22],cnt[22][22],fac[mod],inv[mod],invfac[mod],ans,sz[1111111],in[22],out[22];

void init(){
	fac[0]=fac[1]=inv[1]=invfac[0]=invfac[1]=1;
	FOR(i,2,mod-1){
		fac[i]=1ll*fac[i-1]*i%mod;
		inv[i]=mod-1ll*(mod/i)*inv[mod%i]%mod;
		invfac[i]=1ll*invfac[i-1]*inv[i]%mod;
	}
}

int C(int n,int m){
	if(n<0 || m<0 || n<m) return 0;
	return 1ll*fac[n]*invfac[m]%mod*invfac[n-m]%mod;
}

int lucas(int n,int m){
	if(n<mod) return C(n,m);
	return 1ll*lucas(n/mod,m/mod)*lucas(n%mod,m%mod)%mod;
}

int calc(int n,int m){
	int ans=0;
	FOR(i,0,min(n,m)) ans=(ans+1ll*Lucas(n+m-i,i,MOD)*Lucas(n+m-2*i,n-i,MOD))%mod;
	return ans;
}
int solve(int S){
	int pre=-1,ans=1;
	FOR(i,0,k-1) if((S>>i)&1){
		if(pre==-1) ans=1ll*ans*in[i]%mod;
		else{
			if(y[pre]>y[i]) return 0;
			ans=1ll*ans*cnt[pre][i]%mod;
		}
		pre=i;
	}
	if(~pre) return 1ll*ans*out[pre]%mod;
	else return calc(n,m);
}

int main()
{
    // int n, m, K;
    cin >> n >> m >> k;
	init();
	for(int i = 0; i < k; ++i)
    {
        cin >> p[i].x;
        cin >> p[i].y;
    }
	sort(p, p + k);

	FOR(i,0,k-1) x[i]=p[i].x,y[i]=p[i].y;
	FOR(i,0,k-1) FOR(j,i+1,k-1) cnt[i][j]=calc(x[j]-x[i],y[j]-y[i]);
	FOR(i,0,k-1) in[i]=calc(x[i]-1,y[i]-1),out[i]=calc(n+1-x[i],m+1-y[i]);
	FOR(i,1,(1<<k)-1) sz[i]=sz[i>>1]+(i&1);
	FOR(i,0,(1<<k)-1){
		if(sz[i]&1) ans=(ans-solve(i)+mod)%mod;
		else ans=(ans+solve(i))%mod;
	}
	printf("%d\n",ans);
}
