#pragma once
template <class TypeOfVer, class TypeOfEdge>
class adjListGraph {
	int Vers, Edges;

	struct edgeNode {	//保存边的结点类
		int end;
		TypeOfEdge weight;
		edgeNode *next;

		edgeNode(int e, TypeOfEdge w, edgeNode *n = NULL) { end = e; weight = w; next = n; }
	};

	struct verNode {	//保存顶点的结点类
		TypeOfVer ver;
		edgeNode *head;

		verNode(edgeNode *h = NULL) { head = h; }
	};
	
	struct EulerNode {	//保存欧拉回路的结点类
		int NodeNum;
		EulerNode *next;

		EulerNode(int ver) { NodeNum = ver; next = NULL; }
	};

	struct edge {		//求最小生成树时用到
		int beg, end;
		TypeOfEdge w;

		bool operator<(const edge &rp) const { return w < rp.w; }
	};

	verNode * verList;

	void dfs(int start, bool p[]) const;
	EulerNode * EulerCircuit(int start, EulerNode *&end);
	verNode * clone() const;
	void printPath(int start, int end, int prev[]) const;

public:
	adjListGraph(int vSize, const TypeOfVer d[]);
	~adjListGraph();

	bool insert(int u, int v, TypeOfEdge w);
	bool remove(int u, int v);
	bool exist(int u, int v) const;

	void dfs() const;			//深度优先搜索
	void bfs() const;			//广度优先搜索

	void EulerCircuit(TypeOfVer start);		//寻找欧拉回路
	void topSort() const;					//寻找拓扑序列

	void kruskal() const;		//最小生成树
	void dijkstra(TypeOfVer start, TypeOfEdge noEdge) const;	//单源最短路径，noEdge表示距离无穷大的值
};
