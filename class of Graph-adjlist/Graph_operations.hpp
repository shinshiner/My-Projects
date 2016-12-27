#pragma once
#include <iostream>
#include "Graph_define.hpp"
#include "linkQueue.hpp"
#include "priorityQueue.hpp"
#include "DisjointSet.hpp"

using namespace std;

//-----private----//

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::dfs(int start, bool visited[]) const {
	edgeNode *p = verList[start].head;

	cout << verList[start].ver << ' ';	//访问结点
	visited[start] = true;

	while (p != NULL) {
		if (visited[p->end] == false) dfs(p->end, visited);
		p = p->next;
	}
}

template <class TypeOfVer, class TypeOfEdge>
typename adjListGraph<TypeOfVer, TypeOfEdge>::EulerNode * 
		adjListGraph<TypeOfVer, TypeOfEdge>::EulerCircuit(int start, EulerNode *&end) {
	EulerNode *beg;
	int nextNode;

	beg = end = new EulerNode(start);			//将起始结点放入欧拉回路
	while (verList[start].head != NULL) {		//起始结点尚有边未被访问
		nextNode = verList[start].head->end;
		remove(start, nextNode);
		remove(nextNode, start);
		start = nextNode;
		end->next = new EulerNode(start);
		end = end->next;
	}
	return beg;
}

template <class TypeOfVer, class TypeOfEdge>
typename adjListGraph<TypeOfVer, TypeOfEdge>::verNode *
adjListGraph<TypeOfVer, TypeOfEdge>::clone() const {
	verNode *tmp = new verNode[Vers];
	edgeNode *p;

	for (int i = 0; i < Vers; ++i) {		//复制每个结点在邻接表中的信息
		tmp[i].ver = verList[i].ver;		//复制结点i的信息
		p = verList[i].head;
		while (p != NULL) {					//复制结点i对应的边的信息
			tmp[i].head = new edgeNode(p->end, p->weight, tmp[i].head);
			p = p->next;
		}
	}
	return tmp;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::printPath(int start, int end, int prev[]) const {
	if (start == end) { cout << verList[start].ver; return; }
	printPath(start, prev[end], prev);
	cout << '-' << verList[end].ver;
}

//-----public-----//

template <class TypeOfVer, class TypeOfEdge>
adjListGraph<TypeOfVer, TypeOfEdge>::adjListGraph(int vSize, const TypeOfVer d[]) {
	Vers = vSize;
	Edges = 0;
	verList = new verNode[vSize];

	for (int i = 0; i < Vers; ++i) verList[i].ver = d[i];
}

template <class TypeOfVer, class TypeOfEdge>
adjListGraph<TypeOfVer, TypeOfEdge>::~adjListGraph() {
	edgeNode *p;

	for(int i=0;i<Vers;++i)
		while ((p = verList[i].head) != NULL) {		//释放第i个结点的单链表
			verList[i].head = p->next;
			delete p;
		}

	delete[]verList;
}

template <class TypeOfVer, class TypeOfEdge>
bool adjListGraph<TypeOfVer, TypeOfEdge>::insert(int u, int v, TypeOfEdge w) {
	verList[u].head = new edgeNode(v, w, verList[u].head);
	++Edges;
	return true;
}

template <class TypeOfVer, class TypeOfEdge>
bool adjListGraph<TypeOfVer, TypeOfEdge>::remove(int u, int v) {
	edgeNode *p = verList[u].head, *q;

	if (p == NULL) return false;		//结点u没有相连的边

	if (p->end == v) {					//单链表中的第一个结点就是待删除的边
		verList[u].head = p->next;
		delete p;
		--Edges;
		return true;
	}

	while (p->next != NULL&&p->next->end != v)p = p->next;	//查找待删除的边
	if (p->next == NULL) return false;		//没有找到待删除的边
	q = p->next;
	p->next = q->next;
	delete q;
	--Edges;
	return true;
}

template <class TypeOfVer, class TypeOfEdge>
bool adjListGraph<TypeOfVer, TypeOfEdge>::exist(int u, int v) const {
	edgeNode *p = verList[u].head;

	while (p != NULL&&p->end != v)p = p->next;
	if (p == NULL) return false;
	else return true;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::dfs() const {
	bool *visited = new bool[Vers];		//记录每个结点是否已被访问
	for (int i = 0; i < Vers; ++i) visited[i] = false;

	cout << "当前图的深度优先遍历序列为：\n";
	for (int i = 0; i < Vers; ++i) {
		if (visited[i] == true) continue;
		dfs(i, visited);
		cout << '\n';
	}
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::bfs() const {
	bool *visited = new bool[Vers];
	int currentNode;
	linkQueue q;
	edgeNode *p;

	for (int i = 0; i < Vers; ++i) visited[i] = false;		//初始化所有元素为未访问

	cout << "当前图的广度优先遍历序列为：\n";
	for (int i = 0; i < Vers; ++i) {
		if (visited[i] == true) continue;
		q.enQueue(i);
		while (!q.isEmpty()) {
			currentNode = q.getHead();
			q.deQueue();
			if (visited[currentNode] == true) continue;
			cout << verList[currentNode].ver << ' ';
			visited[currentNode] = true;
			p = verList[currentNode].head;
			while (p != NULL) {			//currentNode的后继结点放入队列
				if (visited[p->end] == false) q.enQueue(p->end);
				p = p->next;
			}
			cout << '\n';
		}
	}
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::EulerCircuit(TypeOfVer start) {
	EulerNode *beg, *end, *p, *q, *tb, *te;		//beg,end为欧拉回路的起点和终点
	int numOfDegree;
	edgeNode *r;
	verNode *tmp;

	//检查是否存在欧拉回路
	if (Edges == 0) { cout << "不存在欧拉回路\n"; return; }
	for (int i = 0; i < Vers; ++i) {
		numOfDegree = 0;
		r = verList[i].head;
		while (r != 0) { ++numOfDegree; r = r->next; }
		if (numOfDegree == 0 || numOfDegree % 2) { cout << "不存在欧拉回路\n"; return; }
	}

	//寻找起始结点的编号
	int i;
	for (i = 0; i < Vers; ++i)
		if (verList[i].ver == start) break;
	if (i == Vers) { cout << "起始结点不存在\n"; return; }

	//创建一份邻接表的复制
	tmp = clone();

	//寻找从i出发的路径，路径的起点和终点地址分别是beg和end
	beg = EulerCircuit(i, end);

	while (true) {
		p = beg;
		while (p->next != NULL)			//检查是否有边尚未被访问
			if (verList[p->next->NodeNum].head != NULL) break;
			else p = p->next;
			if (p->next == NULL) break;	//所有的边都已被访问
			q = p->next;				//尚有未被访问过的结点
			tb = EulerCircuit(q->NodeNum, te);	//从此结点开始深度优先搜索
			te->next = q->next;			//将搜索到的路径拼接到原来的路径上
			p->next = tb;
			delete q;
	}

	//恢复原图
	delete[]verList;
	verList = tmp;

	//显示得到的欧拉回路
	cout << "欧拉回路是：\n";
	while (beg != NULL) {
		cout << verList[beg->NodeNum].ver << ' ';
		p = beg;
		beg = beg->next;
		delete p;
	}
	cout << '\n';
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::topSort() const {
	linkQueue q;
	edgeNode *p;
	int current, *inDegree = new int[Vers];

	//计算每个结点的入度
	for (int i = 0; i < Vers; ++i) inDegree[i] = 0;
	for (int i = 0; i < Vers; ++i)
		for (p = verList[i].head; p != NULL; p = p->next) ++inDegree[p->end];

	//入度为0的结点入队
	for (int i = 0; i < Vers; ++i)
		if (inDegree[i] == 0) q.enQueue(i);

	cout << "拓扑排序为：\n";
	while (!q.isEmpty()) {
		current = q.getHead();
		q.deQueue();
		cout << verList[current].ver << ' ';
		for (p = verList[current].head; p != NULL; p = p->next)
			if (--inDegree[p->end] == 0) q.enQueue(p->end);
	}
	cout << '\n';
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::kruskal() const {
	int edgesAccepted = 0, u, v;
	edgeNode *p;
	edge e;
	DisjointSet ds(Vers);
	priorityQueue<edge> pq;

	//生成优先级队列
	for(int i=0;i<Vers;++i)
		for(p=verList[i].head;p!=NULL;p=p->next)
			if (i < p->end) {	//邻接表把无向图的边存了两次，加上判断避免边入队两次
				e.beg = i;
				e.end = p->end;
				e.w = p->weight;
				pq.enQueue(e);
			}

	//开始归并
	while (edgesAccepted < Vers - 1) {
		e = pq.deQueue();		//取出权值最小的边
		u = ds.Find(e.beg);
		v = ds.Find(e.end);
		if (u != v) {			//加入e不会形成回路
			++edgesAccepted;
			ds.Union(u, v);
			cout << '(' << verList[e.beg].ver << ',' << verList[e.end].ver << ') ';
		}
	}
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::dijkstra(TypeOfVer start, TypeOfEdge noEdge) const {
	TypeOfEdge *distance = new TypeOfEdge[Vers], min;
	int *prev = new int[Vers];		//标记各结点的父亲
	bool *known = new bool[Vers];
	int u, sNo, i, j;
	edgeNode *p;

	//初始化
	for (i = 0; i < Vers; ++i) { known[i] = false; distance[i] = noEdge; }

	//寻找起始结点的编号
	for (sNo = 0; sNo < Vers; ++sNo)
		if (verList[sNo].ver == start) break;
	if (sNo = Vers) { cout << "起始结点不存在\n"; return; }

	distance[sNo] = 0;
	prev[sNo] = sNo;

	for (i = 1; i < Vers; ++i) {
		min = noEdge;
		for(j=0;j<Vers;++j)		//寻找具有最短距离的结点
			if (!known[j] && distance[j] < min) { min = distance[j]; u = j; }
		known[u] = true;		//将u放入集合
		for(p=verList[u].head;p!=NULL;p=p->next)	//更新u的邻接点距离
			if (!known[p->end] && distance[p->end] > min + p->weight) {
				distance[p->end] = min + p->weight;
				prev[p->end] = u;
			}
	}

	//输出所有的路径信息
	for (i = 0; i < Vers; ++i) {
		cout << "从" << start << "到" << verList[i].ver << "的路径为：\n";
		printPath(sNo, i, prev);
		cout << "\t长度为：" << distance[i] << '\n';
	}
}
