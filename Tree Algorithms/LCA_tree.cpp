struct BinaryLifting {
	int n;
	int maxLog;
	ll maxRequirement;
	vector<vector<int>> parent;
	vector<int> logValues;
	bool precomputedLogs = false;
	BinaryLifting(int n1, vector<vector<int>> &edges, ll requirement, int root) {
		n = n1;
		parent.resize(n + 1);
		maxLog = log2(requirement + 1);
		maxRequirement = requirement;
		for (int i = 1; i <= n; i++) {
			parent[i].resize(maxLog + 1);
			for (int j = 0; j <= maxLog; j++) {
				parent[i][j] = 0;
			}
		}
		fillParentTable(root, edges);
		if (maxRequirement <= 1000000LL)
			precomputeLogs();
	}
	void fillParentTable(int root, vector<vector<int>> &edges) {
		vector<bool> visited(n + 1);
		dfsBinaryLifting(root, edges, visited);
		int intermediate = 0;
		for (int i = 1; i <= maxLog; i++) {
			for (int j = 1; j <= n; j++) {
				intermediate = parent[j][i - 1];
				if (intermediate != 0) {
					parent[j][i] = parent[intermediate][i - 1];
				}
			}
		}
	}
	void dfsBinaryLifting(int root, vector<vector<int>> &edges, vector<bool> &visited) {
		visited[root] = true;
		for (auto i : edges[root]) {
			if (!visited[i]) {
				parent[i][0] = root;
				dfsBinaryLifting(i, edges, visited);
			}
		}
	}
	void precomputeLogs() {
		precomputedLogs = true;
		logValues.resize(maxRequirement + 1);
		logValues[1] = 0;
		for (int i = 2; i <= maxRequirement; i++) {
			logValues[i] = logValues[i / 2] + 1;
		}
	}
	int kthParent(int start, int k) {
		int a = start;
		while (k > 0) {
			int x = getLog(k);
			a = parent[a][x];
			if (a == 0)
				return a;
			k -= (1 << x);
		}
		return a;
	}
	inline int getLog(ll x) {
		return precomputedLogs ? logValues[x] : log2(x);
	}
};

struct LCA {
	int n;
	vector<int> level;
	LCA(int n1, vector<vector<int>> &edges, int root) {
		n = n1;
		level.resize(n + 1);
		dfsLCA(root, edges, -1);
	}
	void dfsLCA(int root, vector<vector<int>> &edges, int parent) {
		for (auto i : edges[root]) {
			if (i != parent) {
				level[i] = level[root] + 1;
				dfsLCA(i, edges, root);
			}
		}
	}
	int getLCA(int a, int b, BinaryLifting &bl_object) {
		if (level[a] > level[b]) {
			swap(a, b);
		}
		b = bl_object.kthParent(b, level[b] - level[a]);
		if (a == b)
			return a;
		for (int i = bl_object.maxLog; i >= 0; i--) {
			int parent1 = bl_object.parent[a][i];
			int parent2 = bl_object.parent[b][i];
			if (parent2 != parent1 && parent1 != -1 && parent2 != -1) {
				a = parent1;
				b = parent2;
			}
		}
		return bl_object.parent[a][0];
	}
};
