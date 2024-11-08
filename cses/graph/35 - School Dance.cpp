// School Dance
//
// Problem name: School Dance
// Problem Link: https://cses.fi/problemset/task/1696
// Author: Bernardo Archegas (https://codeforces.com/profile/Ber)

// There are n boys and m girls in a school. Next week a school dance will be organized. 
// A dance pair consists of a boy and a girl, and there are k potential pairs.
// Your task is to find out the maximum number of dance pairs and show how this number can be achieved.

#include <bits/stdc++.h>
 
using namespace std;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
 
mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());
    
const int MOD = 1e9 + 7;
const int MAXN = 2e5 + 5;
const int INF = 1e9;
 
// Dinic
//
// O(min(m * max_flow, n^2 m))
// Grafo com capacidades 1 -> O(sqrt(n)*m)
struct dinic {
	const bool scaling = false;
	int lim;
	struct edge {
		int to, cap, rev, flow;
		bool res;
		edge(int to_, int cap_, int rev_, bool res_)
			: to(to_), cap(cap_), rev(rev_), flow(0), res(res_) {}
	};
 
	vector<vector<edge>> g;
	vector<int> lev, beg;
	ll F;
	dinic(int n) : g(n), F(0) {}
 
	void add(int a, int b, int c) {
		g[a].emplace_back(b, c, g[b].size(), false);
		g[b].emplace_back(a, 0, g[a].size()-1, true);
	}
	bool bfs(int s, int t) {
		lev = vector<int>(g.size(), -1); lev[s] = 0;
		beg = vector<int>(g.size(), 0);
		queue<int> q; q.push(s);
		while (q.size()) {
			int u = q.front(); q.pop();
			for (auto& i : g[u]) {
				if (lev[i.to] != -1 or (i.flow == i.cap)) continue;
				if (scaling and i.cap - i.flow < lim) continue;
				lev[i.to] = lev[u] + 1;
				q.push(i.to);
			}
		}
		return lev[t] != -1;
	}
	int dfs(int v, int s, int f = INF) {
		if (!f or v == s) return f;
		for (int& i = beg[v]; i < g[v].size(); i++) {
			auto& e = g[v][i];
			if (lev[e.to] != lev[v] + 1) continue;
			int foi = dfs(e.to, s, min(f, e.cap - e.flow));
			if (!foi) continue;
			e.flow += foi, g[e.to][e.rev].flow -= foi;
			return foi;
		}
		return 0;
	}
	ll max_flow(int s, int t) {
		for (lim = scaling ? (1<<30) : 1; lim; lim /= 2)
			while (bfs(s, t)) while (int ff = dfs(s, t)) F += ff;
		return F;
	}
 
	// arestas com fluxo
	vector<pii> flow_edges() {
		vector<pii> ans;
		int n = g.size();
		for (int i = 0; i < n; i++) {
			for (auto edge : g[i]) {
				if (!edge.res && edge.flow) 
					ans.emplace_back(i, edge.to);
			}
		}
		return ans;
	}
 
	// arestas no min cut
	vector<pii> cut_edges(int s, int t) {
		max_flow(s, t);
		vector<pii> cut;
		vector<int> vis(g.size(), 0), st = {s};
		vis[s] = 1;
		while (st.size()) {
			int u = st.back(); st.pop_back();
			for (auto e : g[u]) if (!vis[e.to] and e.flow < e.cap)
				vis[e.to] = 1, st.push_back(e.to);
		}
		for (int i = 0; i < g.size(); i++) for (auto e : g[i])
			if (vis[i] and !vis[e.to] and !e.res) cut.emplace_back(i, e.to);
		return cut;
	}
};
 
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
	int n, m, k;
	cin >> n >> m >> k;
	int src = n + m, snk = n + m + 1;
	dinic ber(snk + 1);
	for (int i = 0; i < k; i++) {
		int a, b;
		cin >> a >> b;
		a--, b--;
		ber.add(a, n + b, 1);
	}
	for (int i = 0; i < n; i++) {
		ber.add(src, i, 1);
	}
	for (int i = 0; i < m; i++) {
		ber.add(i + n, snk, 1);
	}
	cout << ber.max_flow(src, snk) << '\n';
	vector<pii> ed = ber.flow_edges();
	for (auto x : ed) {
		if (x.first != src && x.second != snk) cout << x.first + 1 << ' ' << x.second - n + 1 << '\n';
	}
    return 0;
}