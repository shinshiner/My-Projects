template <class T>
class seqStack {
private:
	T *elem;
	int top_p, maxSize;
	void doublespace();
public:
	seqStack(int initSize = 10) { elem = new T[initSize]; maxSize = initSize; top_p = -1; }
	~seqStack() { delete[]elem; }
	bool isEmpty() const { return top_p == -1; }
	void push(const T &x) { if (top_p == maxSize - 1) doublespace(); elem[++top_p] = x; }
	T pop() { return elem[top_p--]; }
	T top() const { return elem[top_p]; }
};

template <class T>
void seqStack<T>::doublespace() {
	T *tmp = elem;
	elem = new T[2 * maxSize];
	for (int i = 0; i<maxSize; ++i) elem[i] = tmp[i];
	maxSize *= 2;
	delete[]tmp;
}
