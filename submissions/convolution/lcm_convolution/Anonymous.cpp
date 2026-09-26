#include <bits/stdc++.h>
using namespace std;






/*
    https://github.com/old-yan/CP-template
*/
#ifndef __OY_LINUXIO__
#define __OY_LINUXIO__
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>
#ifdef __unix__
#include <sys/mman.h>
#include <sys/stat.h>
#endif
#if __cpp_constexpr >= 201907L
#define CONSTEXPR20 constexpr
#define INLINE20 constexpr
#else
#define CONSTEXPR20
#define INLINE20 inline
#endif
#define cin OY::LinuxIO::InputHelper<>::get_instance()
#define cout OY::LinuxIO::OutputHelper::get_instance()
#define endl '\n'
#ifndef INPUT_FILE
#define INPUT_FILE "in.txt"
#endif
#ifndef OUTPUT_FILE
#define OUTPUT_FILE "out.txt"
#endif
namespace OY {
	namespace LinuxIO {
		static constexpr size_t INPUT_BUFFER_SIZE = 1 << 26, OUTPUT_BUFFER_SIZE = 1 << 20;
#ifdef OY_LOCAL
		static constexpr char input_file[] = INPUT_FILE, output_file[] = OUTPUT_FILE;
#else
		static constexpr char input_file[] = "", output_file[] = "";
#endif
		template <typename U, size_t E>
		struct TenPow {
			static constexpr U value = TenPow<U, E - 1>::value * 10;
		};
		template <typename U>
		struct TenPow<U, 0> {
			static constexpr U value = 1;
		};
		struct InputPre {
			uint32_t m_data[0x10000];
			CONSTEXPR20 InputPre() {
				std::fill(m_data, m_data + 0x10000, -1);
				for (size_t i = 0, val = 0; i != 10; i++)
					for (size_t j = 0; j != 10; j++) m_data[0x3030 + i + (j << 8)] = val++;
			}
		};
		struct OutputPre {
			uint32_t m_data[10000];
			CONSTEXPR20 OutputPre() {
				uint32_t *c = m_data;
				for (size_t i = 0; i != 10; i++)
					for (size_t j = 0; j != 10; j++)
						for (size_t k = 0; k != 10; k++)
							for (size_t l = 0; l != 10; l++) *c++ = i + (j << 8) + (k << 16) + (l << 24) + 0x30303030;
			}
		};
		template <size_t MMAP_SIZE = 1 << 30>
		struct InputHelper {
			static INLINE20 InputPre pre{};
			struct stat m_stat;
			char *m_p, *m_c, *m_end;
			InputHelper(FILE *file = stdin) {
#ifdef __unix__
				auto fd = fileno(file);
				fstat(fd, &m_stat);
				m_c = m_p = (char *)mmap(nullptr, m_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
				m_end = m_p + m_stat.st_size;
#else
				uint32_t size = fread(m_c = m_p = new char[INPUT_BUFFER_SIZE], 1, INPUT_BUFFER_SIZE, file);
				m_end = m_p + size;
#endif
			}
			static InputHelper<MMAP_SIZE> &get_instance() {
				static InputHelper<MMAP_SIZE> s_obj(*input_file ? fopen(input_file, "rt") : stdin);
				return s_obj;
			}
			template <typename Tp, typename std::enable_if<std::is_unsigned<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
			InputHelper &operator>>(Tp &x) {
				x = 0;
				while (!isdigit(*m_c)) m_c++;
				x = *m_c++ ^ '0';
				while (~pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)]) x = x * 100 + pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)++];
				if (isdigit(*m_c)) x = x * 10 + (*m_c++ ^ '0');
				return *this;
			}
			template <typename Tp, typename std::enable_if<std::is_signed<Tp>::value & std::is_integral<Tp>::value>::type * = nullptr>
			InputHelper &operator>>(Tp &x) {
				typename std::make_unsigned<Tp>::type t{};
				bool sign{};
				while (!isdigit(*m_c)) sign = (*m_c++ == '-');
				t = *m_c++ ^ '0';
				while (~pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)]) t = t * 100 + pre.m_data[*reinterpret_cast<uint16_t *&>(m_c)++];
				if (isdigit(*m_c)) t = t * 10 + (*m_c++ ^ '0');
				x = sign ? -t : t;
				return *this;
			}
			InputHelper &operator>>(char &x) {
				while (*m_c <= ' ') m_c++;
				x = *m_c++;
				return *this;
			}
			InputHelper &operator>>(std::string &x) {
				while (*m_c <= ' ') m_c++;
				char *c = m_c;
				while (*c > ' ') c++;
				x.assign(m_c, c - m_c), m_c = c;
				return *this;
			}
			InputHelper &operator>>(std::string_view &x) {
				while (*m_c <= ' ') m_c++;
				char *c = m_c;
				while (*c > ' ') c++;
				x = std::string_view(m_c, c - m_c), m_c = c;
				return *this;
			}
		};
		struct OutputHelper {
			static INLINE20 OutputPre pre{};
			FILE *m_file;
			char m_p[OUTPUT_BUFFER_SIZE], *m_c, *m_end;
			OutputHelper(FILE *file = stdout) {
				m_file = file;
				m_c = m_p, m_end = m_p + OUTPUT_BUFFER_SIZE;
			}
			~OutputHelper() { flush(); }
			static OutputHelper &get_instance() {
				static OutputHelper s_obj(*output_file ? fopen(output_file, "wt") : stdout);
				return s_obj;
			}
			void flush() { fwrite(m_p, 1, m_c - m_p, m_file), m_c = m_p; }
			OutputHelper &operator<<(char x) {
				if (m_end - m_c < 20) flush();
				*m_c++ = x;
				return *this;
			}
			OutputHelper &operator<<(std::string_view s) {
				if (m_end - m_c < s.size()) flush();
				memcpy(m_c, s.data(), s.size()), m_c += s.size();
				return *this;
			}
			OutputHelper &operator<<(uint64_t x) {
				if (m_end - m_c < 20) flush();
#define CASEW(w)                                                           \
				case TenPow<uint64_t, w - 1>::value... TenPow<uint64_t, w>::value - 1: \
				*(uint32_t *)m_c = pre.m_data[x / TenPow<uint64_t, w - 4>::value]; \
				m_c += 4, x %= TenPow<uint64_t, w - 4>::value;
				switch (x) {
					CASEW(19);
					CASEW(15);
					CASEW(11);
					CASEW(7);
				case 100 ... 999:
					*(uint32_t *)m_c = pre.m_data[x * 10];
					m_c += 3;
					break;
					CASEW(18);
					CASEW(14);
					CASEW(10);
					CASEW(6);
				case 10 ... 99:
					*(uint32_t *)m_c = pre.m_data[x * 100];
					m_c += 2;
					break;
					CASEW(17);
					CASEW(13);
					CASEW(9);
					CASEW(5);
				case 0 ... 9:
					*m_c++ = '0' + x;
					break;
				default:
					*(uint32_t *)m_c = pre.m_data[x / TenPow<uint64_t, 16>::value];
					m_c += 4;
					x %= TenPow<uint64_t, 16>::value;
					CASEW(16);
					CASEW(12);
					CASEW(8);
				case 1000 ... 9999:
					*(uint32_t *)m_c = pre.m_data[x];
					m_c += 4;
					break;
				}
#undef CASEW
				return *this;
			}
			OutputHelper &operator<<(uint32_t x) {
				if (m_end - m_c < 20) flush();
#define CASEW(w)                                                           \
				case TenPow<uint32_t, w - 1>::value... TenPow<uint32_t, w>::value - 1: \
				*(uint32_t *)m_c = pre.m_data[x / TenPow<uint32_t, w - 4>::value]; \
				m_c += 4, x %= TenPow<uint32_t, w - 4>::value;
				switch (x) {
				default:
					*(uint32_t *)m_c = pre.m_data[x / TenPow<uint32_t, 6>::value];
					m_c += 4;
					x %= TenPow<uint32_t, 6>::value;
					CASEW(6);
				case 10 ... 99:
					*(uint32_t *)m_c = pre.m_data[x * 100];
					m_c += 2;
					break;
					CASEW(9);
					CASEW(5);
				case 0 ... 9:
					*m_c++ = '0' + x;
					break;
					CASEW(8);
				case 1000 ... 9999:
					*(uint32_t *)m_c = pre.m_data[x];
					m_c += 4;
					break;
					CASEW(7);
				case 100 ... 999:
					*(uint32_t *)m_c = pre.m_data[x * 10];
					m_c += 3;
					break;
				}
#undef CASEW
				return *this;
			}
			OutputHelper &operator<<(int64_t x) {
				if (x >= 0)
					return (*this) << uint64_t(x);
				else
					return (*this) << '-' << uint64_t(-x);
			}
			OutputHelper &operator<<(int32_t x) {
				if (x >= 0)
					return (*this) << uint32_t(x);
				else
					return (*this) << '-' << uint32_t(-x);
			}
		};
	}
}
#endif















typedef unsigned int i32;
typedef unsigned long long i64;
typedef unsigned int u32;
typedef unsigned long long u64;
constexpr int mod=998244353;




u32 qpow(u32 x,int y){
	u32 ans=1;
	while(y){
		if(y&1) ans=(u64)ans*x%mod;
		x=(u64)x*x%mod;
		y>>=1;
	}
	return ans;
}
u32 mod_inv(u32 x){
	return qpow(x,mod-2);
}
int isqrt(int x){return (int)sqrt(x+0.5);}



namespace dgf{
//	constexpr int N=1e7+1145,M=1e6+114,N1_2=5e3+114,B=100000;
	const int B=100000;
//	bitset<N> isp;
//	int pri[M],pl;
//	void sieve(int n){
//		n+=1000;
//		for(int i=2;i<=n;++i) isp[i]=1;
//		for(int i=2;i<=n;++i){
//			if(isp[i]) pri[++pl]=i;
//			for(int j=1;i*pri[j]<=n;++j){
//				isp[i*pri[j]]=0;
//				if(i%pri[j]==0) break;
//			}
//		}
//	}
//	int A[N],U[N1_2];
	void dgf_add(u32 a[],u32 b[],u32 res[],int n){
		for(int i=1;i<=n;++i){
			res[i]=min(a[i]+b[i],a[i]+b[i]-mod);
		}
	}
	void dgf_sub(u32 a[],u32 b[],u32 res[],int n){
		for(int i=1;i<=n;++i){
			res[i]=min(a[i]-b[i],a[i]-b[i]+mod);
		}
	}
	void dgf_mul(u32 a[],u32 b[],u32 res[],int n){
		for(int i=1;i<=n;++i){
			res[i]=(u64)a[i]*b[i]%mod;
		}
	}
	void dgf_add(u32 a[],u32 b[],int n){
		for(int i=1;i<=n;++i){
			a[i]=min(a[i]+b[i],a[i]+b[i]-mod);
		}
	}
	void dgf_sub(u32 a[],u32 b[],int n){
		for(int i=1;i<=n;++i){
			a[i]=min(a[i]-b[i],a[i]-b[i]+mod);
		}
	}
	void dgf_mul(u32 a[],u32 b[],int n){
		for(int i=1;i<=n;++i){
			a[i]=(u64)a[i]*b[i]%mod;
		}
	}
	void conv(u32 a[],u32 b[],u32 res[],int n){
		res[1]=(u64)a[1]*b[1]%mod;
		for(int i=2;i<=n;++i){
			res[i]=((u64)a[1]*b[i]+(u64)a[i]*b[1])%mod;
		}
		for(int i=2;i*i<=n;++i){
			res[i*i]=(res[i*i]+(u64)a[i]*b[i])%mod;
		}
		for(int L=0;L<n;){
			int R=min(n,L+B);
			for(int i=2;i*i<=R;++i){
				for(int j=max(i+1,L/i+1);i*j<=R;++j){
					res[i*j]=(res[i*j]+(u64)a[i]*b[j]+(u64)a[j]*b[i])%mod;
				}
			}
			L=R;
		}
	}
#define redu(x) x=min(x,x-mod)
#define redu2(x) x=min(x,x-mod*2);
#define nredu(x) x=min(x,x+mod)
#define nredu2(x) x=min(x,x+mod*2);
	void pre_sum(u32 a[],u32 res[],int n){
		res[1]=a[1];
		for(int i=2;i<=n;++i){
			res[i]=a[1]+a[i];
		}
		for(int i=2;i*i<=n;++i){
			res[i*i]+=a[i],redu2(res[i*i]);
		}
		for(int L=0;L<n;){
			int R=min(n,L+B);
			for(int i=2;i*i<=R;++i){
				for(int j=max(i+1,L/i+1);i*j<=R;++j){
					res[i*j]+=a[i]+a[j],redu2(res[i*j]);
				}
			}
			L=R;
		}
		for(int i=1;i<=n;++i) redu(res[i]);
	}
	void suf_sum(u32 a[],u32 res[],int n){
		res[1]=a[1];
		for(int i=2;i<=n;++i){
			res[1]+=a[1],redu(res[1]);
			res[i]=a[i];
		}
		for(int i=2;i*i<=n;++i){
			res[i]+=a[i*i],redu(res[i]);
		}
		for(int L=0;L<n;){
			int R=min(n,L+B);
			for(int i=2;i*i<=R;++i){
				for(int j=max(i+1,L/i+1);i*j<=R;++j){
					res[i]+=a[i*j],redu(res[i]);
					res[j]+=a[i*j],redu(res[j]);
				}
			}
			L=R;
		}
	}
	void pre_dif(u32 a[],u32 res[],int n){
		res[1]=a[1];
		for(int i=2;i<=n;++i) res[i]=res[1];
		for(int L=1;L<n;){
			int R=min({L*2+1,n,L+B});
			for(int i=isqrt(L)+1;i*i<=R;++i){
				res[i*i]+=res[i],redu2(res[i*i]);
			}
			for(int i=2;i*i<=R;++i){
				for(int j=max(i+1,L/i+1);i*j<=R;++j){
					res[i*j]+=res[j]+res[i],redu2(res[i*j]);
				}
			}
			for(int i=L+1;i<=R;++i){
				redu(res[i]);
				res[i]=a[i]-res[i],nredu(res[i]);
			}
			L=R;
		}
	}
	void suf_dif(u32 a[],u32 res[],int n){
		
	}
	void conv_gcd(u32 a[],u32 res[],int n){
		
	}
	void conv_lcm(u32 a[],u32 res[],int n){
		
	}
	void inv(u32 a[],u32 res[],int n){
		int inva1=mod_inv(a[1]);
		res[1]=inva1;
		for(int i=2;i<=n;++i) res[i]=(u64)res[1]*a[i]%mod;
		for(int L=1;L<n;){
			int R=min({L*2+1,n,L+B});
			for(int i=isqrt(L)+1;i*i<=R;++i){
				res[i*i]=(res[i*i]+(u64)a[i]*res[i])%mod;
			}
			for(int i=2;i*i<=R;++i){
				for(int j=max(i+1,L/i+1);i*j<=R;++j){
					res[i*j]=(res[i*j]+(u64)a[i]*res[j]+(u64)a[j]*res[i])%mod;
				}
			}
			for(int i=L+1;i<=R;++i){
				res[i]=(u64)(mod-res[i])*inva1%mod;
			}
			L=R;
		}
	}
	void log(u32 a[],u32 res[],int n){
		
	}
	void exp(u32 a[],u32 res[],int n){
		
	}
	void div(u32 a[],u32 b[],u32 res[],int n){
		int invb1=mod_inv(b[1]);
		res[1]=(u64)a[1]*invb1%mod;
		for(int i=2;i<=n;++i) res[i]=(u64)res[1]*b[i]%mod;
		for(int L=1;L<n;){
			int R=min({L*2+1,n,L+B});
			for(int i=isqrt(L)+1;i*i<=R;++i){
				res[i*i]=(res[i*i]+(u64)b[i]*res[i])%mod;
			}
			for(int i=2;i*i<=R;++i){
				for(int j=max(i+1,L/i+1);i*j<=R;++j){
					res[i*j]=(res[i*j]+(u64)b[i]*res[j]+(u64)b[j]*res[i])%mod;
				}
			}
			for(int i=L+1;i<=R;++i){
				res[i]=(u64)(a[i]+mod-res[i])*invb1%mod;
			}
			L=R;
		}
	}
	void composition(u32 a[],u32 p[],u32 res[],int n,int m){
		
	}
#undef redu
#undef redu2
}
u32 seed;
inline u32 getnext(){
	seed^=seed<<13;
	seed^=seed>>17;
	seed^=seed<<5;
	return seed;
}
const int N=1e6+114;
int n;
u32 a[N],b[N],c[N];
signed main(){
//	std::ios_base::sync_with_stdio(0);
//	cin.tie(0);
//	cout.tie(0);
	cin >> n;
	for(int i=1;i<=n;++i) cin >> a[i];
	for(int i=1;i<=n;++i) cin >> b[i];
	dgf::pre_sum(a,c,n);
	dgf::pre_sum(b,a,n);
	dgf::dgf_mul(a,c,n);
	dgf::pre_dif(a,c,n);
	for(int i=1;i<=n;++i) cout << c[i] << ' ';
	cout << endl;
	return 0;
}