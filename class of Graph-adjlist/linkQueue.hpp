#pragma once
#include <iostream>

using namespace std;

class linkQueue {
private:
	struct node {
		int data;
		node *next;
		node(const int &x, node *N = NULL) { data = x; next = N; }
		node() :next(NULL) {}
		~node() {}
	};
	node *front, *rear;
public:
	linkQueue() { front = rear = NULL; }
	~linkQueue();
	bool isEmpty() { return front == NULL; }
	void enQueue(const int &x);
	void deQueue();
	int getHead() { return front->data; }
};

linkQueue::~linkQueue() {
	node *tmp;
	while (front != NULL) {
		tmp = front;
		front = front->next;
		delete tmp;
	}
}

void linkQueue::enQueue(const int &x) {
	if (rear == NULL) front = rear = new node(x);
	else { rear->next = new node(x); rear = rear->next; }
}

void linkQueue::deQueue() {
	if (front == NULL) return; //队列为空，不做任何操作
	node *tmp = front;
	front = front->next;
	if (front == NULL) rear = NULL;
	delete tmp;
}
