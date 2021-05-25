#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#include <cmath>
#include <numeric>
#include <set>
#include <map>
#include <array>
#include <complex>
#include <iomanip>
#include <cassert>
#include <random>
#include <chrono>
#include <valarray>
#include <bitset>
using ll = long long;
using std::cin;
using std::cout;
using std::endl;
std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return 1; } return 0; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return 1; } return 0; }
const int inf = (int)1e9 + 7;
const long long INF = 1LL << 60;

std::tuple<int, int, int, int> input()
{
	int si, sj, ti, tj;
	cin >> si >> sj >> ti >> tj;
	return std::make_tuple(si, sj, ti, tj);
}
const int H = 30;
const int W = 30;
int ch[H][W];
int cw[H][W];
int chcount[H][W];
int cwcount[H][W];
int sh, sw, th, tw;
bool usedW[H], usedH[W];
std::vector<std::vector<int>> A;
std::vector<int> LIST[1800];
std::vector<int> B, C;
std::mt19937 engine(0);
int enc(int h, int w, int dir)
{
	return dir * 900 + h * W + w;
}
std::tuple<int, int, int> dec(int X)
{
	return std::make_tuple(X / 900, (X % 900) / W,  X % W);
}
inline int cost(int X)
{
	auto [dir, h, w] = std::make_tuple(X / 900, (X % 900) / W,  X % W);
	if(dir == 0)
		return cw[h][w];
	else
		return ch[h][w];
}
inline int cost(int sh, int sw, int th, int tw)
{
	if(sh == th)
	{
		if(sw > tw)
			std::swap(sw, tw);
		return cw[sh][sw];
	}
	else
	{
		if(sh > th)
			std::swap(sh, th);
		return ch[sh][sw];
	}
}

void updatech(int i, int j, int val)
{
	ll sum = 1LL * ch[i][j] * chcount[i][j] + val;
	chcount[i][j] += 1;
	ch[i][j] = sum / chcount[i][j];
}
void updatecw(int i, int j, int val)
{
	ll sum = 1LL * cw[i][j] * cwcount[i][j] + val;
	cwcount[i][j] += 1;
	cw[i][j] = sum / cwcount[i][j];
}
void solve_last()
{
	using T = std::tuple<int, int, int>;
	std::priority_queue<T, std::vector<T>, std::greater<>> pq;
	std::array<std::array<int, W>, H> d;
	std::array<std::array<int, W>, H> b;
	for (int i = 0; i < H; ++i)
	{
		for (int j = 0; j < W; ++j)
		{
			d[i][j] = inf;
			b[i][j] = -1;
		}
	}
	d[sh][sw] = 0;
	pq.emplace(0, sh, sw);
	const int dh[] = {1, 0, -1, 0};
	const int dw[] = {0, 1, 0, -1};
	const std::string dir = "DRUL";
	while(not pq.empty())
	{
		auto [dist, h, w] = pq.top();
		pq.pop();
		if(dist > d[h][w])
			continue;
		if(h == th and w == tw)
			break;
		for (int i = 0; i < 4; ++i)
		{
			const int nh = h + dh[i];
			const int nw = w + dw[i];
			if(nh < 0 or nh >= H or nw < 0 or nw >= W)
				continue;
			if(chmin(d[nh][nw], d[h][w] + cost(h, w, nh, nw)))
			{
				pq.emplace(d[nh][nw], nh, nw);
				b[nh][nw] = i;
			}
		}
	}
	int cnt = 0;
	std::string res;
	int curh = th, curw = tw;
	while(curh != sh or curw != sw)
	{
		const int idx = b[curh][curw];
		cnt += 1;
		res += dir[idx];
		curh -= dh[idx];
		curw -= dw[idx];
	}
	std::reverse(res.begin(), res.end());
	cout << res << endl;
	ll len; cin >> len;
	B.emplace_back(len);
	len /= cnt;
	curh = sh, curw = sw;
	std::vector<int> a;
	for(const auto &c : res)
	{
		if(c == 'R')
		{
			updatecw(curh, curw, len);
			a.emplace_back(enc(curh, curw, 0));
			curw += 1;
		}
		if(c == 'L')
		{
			updatecw(curh, curw - 1, len);
			a.emplace_back(enc(curh, curw - 1, 0));
			curw -= 1;
		}
		if(c == 'U')
		{
			updatech(curh - 1, curw, len);
			a.emplace_back(enc(curh - 1, curw, 1));
			curh -= 1;
		}
		if(c == 'D')
		{
			updatech(curh, curw, len);
			a.emplace_back(enc(curh, curw, 1));
			curh += 1;
		}
	}
	std::sort(a.begin(), a.end());
	A.emplace_back(a);
}

void solve_first()
{
	int curh = sh, curw = sw;
	int cnth = std::abs(sh - th);
	int cntw = std::abs(sw - tw);
	std::string res;
	std::vector<int> a;
	auto movew = [&](int tw)
	{
		while(curw < tw)
		{
			a.emplace_back(enc(curh, curw, 0));
			res += 'R';
			curw += 1;
		}
		while(curw > tw)
		{
			a.emplace_back(enc(curh, curw - 1, 0));
			res += 'L';
			curw -= 1;
		}
	};
	auto moveh = [&](int th)
	{
		while(curh < th)
		{
			a.emplace_back(enc(curh, curw, 1));
			res += 'D';
			curh += 1;
		}
		while(curh > th)
		{
			a.emplace_back(enc(curh - 1, curw, 1));
			res += 'U';
			curh -= 1;
		}
	};
	if(std::max(cnth, cntw) * 10 < (cnth + cntw) * 7)
	{
		movew(tw);
		moveh(th);
		cout << res << endl;
		ll len; cin >> len;
		B.emplace_back(len);
		len /= (cnth + cntw);
		for (int i = std::min(sw, tw); i <= std::max(sw, tw); ++i)
		{
			updatecw(sh, i, len);
		}
		for (int i = std::min(sh, th); i <= std::max(sh, th); ++i)
		{
			updatech(i, tw, len);
		}
		std::sort(a.begin(), a.end());
		A.emplace_back(a);
	}
	else
	{
		if(cnth > cntw)
		{
			int w = -1;
			for (int i = std::min(sw, tw); i <= std::max(sw, tw); ++i)
			{
				if(not usedH[i])
				{
					w = i;
					if(cnth > 10)
						usedH[w] = true;
					break;
				}
			}
			if(w == -1)
			{
				solve_last();
			}
			else
			{
				movew(w);
				moveh(th);
				movew(tw);
				cout << res << endl;
				ll len; cin >> len;
				B.emplace_back(len);
				len /= (cnth + cntw);
				for (int i = 0; i < H - 1; ++i)
				{
					updatech(i, w, len);
				}
				std::sort(a.begin(), a.end());
				A.emplace_back(a);
			}
		}
		else
		{
			int h = -1;
			for (int i = std::min(sh, th); i <= std::max(sh, th); ++i)
			{
				if(not usedW[i])
				{
					h = i;
					if(cntw > 10)
						usedW[h] = true;
					break;
				}
			}
			if(h == -1)
			{
				solve_last();
			}
			else
			{
				moveh(h);
				movew(tw);
				moveh(th);
				cout << res << endl;
				ll len; cin >> len;
				B.emplace_back(len);
				len /= (cnth + cntw);
				for (int i = 0; i < W - 1; ++i)
				{
					updatecw(h, i, len);
				}
				std::sort(a.begin(), a.end());
				A.emplace_back(a);
			}
		}
	}
}

void solve(int kkt)
{
	std::tie(sh, sw, th, tw) = input();
	if(kkt < 200)
	{
		solve_first();
	}
	else
	{
		solve_last();
	}
	for (const auto &e : A.back())
	{
		LIST[e].emplace_back(kkt);
	}
	// if(kkt % 10 == 0)
	{
		C.resize(B.size());
		for (int i = 0; i < (int)A.size(); ++i)
		{
			C[i] = 0;
			for (int j = 0; j < (int)A[i].size(); ++j)
			{
				C[i] += cost(A[i][j]);
			}
		}
		for (int jupi = 0; jupi < 1500; ++jupi)
		{
			const int row = engine() % A.size();
			const int col = engine() % A[row].size();
			int change = (int)(engine() % 100) - 50;
			const int cval = A[row][col];
			if(cost(cval) + change < 0)
			{
				change = -cost(cval);
			}
			int diff = 0;
			for(const auto &idx : LIST[cval])
			{
				const int pc = C[idx];
				const int nc = C[idx] + change;
				int pd = std::abs(B[idx] - pc);
				int nd = std::abs(B[idx] - nc);
				diff += nd - pd;
			}
			if(diff < 0)
			{
				auto [dir, h, w] = dec(cval);
				if(dir == 0)
				{
					cw[h][w] += change;
				}
				else
				{
					ch[h][w] += change;
				}
			}
		}
	}
}
int main()
{
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  for (int i = 0; i < H; ++i)
  {
  	for (int j = 0; j < W; ++j)
  	{
  		chcount[i][j] = cwcount[i][j] = 0;
  		ch[i][j] = cw[i][j] = 5000;
  	}
  }

  for (int kkt = 0; kkt < 1000; ++kkt)
  {
  	solve(kkt);
  }
  return 0;
}