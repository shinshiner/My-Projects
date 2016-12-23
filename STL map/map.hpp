#pragma once
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { (r)->color = (c); } while (0)

namespace sjtu {
	enum RBTColor { RED, BLACK };
	template<
		class Key,
		class T,
		class Compare = std::less<Key>
	> class map {
		friend class const_iterator;
		friend class iterator;
	public:
		typedef pair<const Key, T> value_type;
		Compare com;

		struct RBTNode {
		public:
			RBTColor color;		// 颜色
			value_type v;	    // 键值对
			RBTNode *left;	    // 左孩子
			RBTNode *right;     // 右孩子
			RBTNode *parent;    // 父结点

			RBTNode(value_type value, RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r) :
				v(value), color(c), parent(p), left(l), right(r) {}
			RBTNode() {}
		};
		RBTNode *m_NIL;			// 空结点 

	public:
		RBTNode *mRoot;			// 根结点
		size_t m_Size;			//元素个数

		// (递归实现)查找"红黑树"中键值为key的节点
		RBTNode* search(Key key) const { return search(mRoot, key); }

		// 插入函数
		void ininsert(RBTNode* &root, RBTNode* node){
			RBTNode *y = NULL;
			RBTNode *x = root;
			// 1. 将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
			while (x != NULL){
				y = x;
				if (com(node->v.first, x->v.first)) //(node->key < x->key)
					x = x->left;
				else x = x->right;
			}
			node->parent = y;
			if (y != NULL){
				if (com(node->v.first, y->v.first))//(node->key < y->key)
					y->left = node;
				else
					y->right = node;
			}
			else root = node;
			// 2. 设置节点的颜色为红色
			node->color = RED;
			// 3. 将它重新修正为一颗二叉查找树
			insertFixUp(root, node);
		}

		// 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
		RBTNode* successor(const RBTNode *x) const {
			// 如果x存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。
			if (x->right != NULL)	return minimum(x->right);
			// 如果x没有右孩子。则x有以下两种可能：
			// (01) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
			// (02) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
			RBTNode* y = x->parent;
			while ((y != NULL) && (x == y->right)) { x = y;	y = y->parent; }
			return y;
		}

		// 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
		RBTNode* predecessor(const RBTNode *x) const {
			if (x == NULL) return (maximum(mRoot));
			// 如果x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
			if (x->left != NULL)	return maximum(x->left);
			// 如果x没有左孩子。则x有以下两种可能：
			// (01) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
			// (01) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
			RBTNode* y = x->parent;
			while ((y != NULL) && x == y->left) { x = y; y = y->parent; }
			return y;
		}

		// 将结点(key为节点键值)插入到红黑树中
		void ininsert(value_type key){
			RBTNode *z = NULL;
			// 如果新建结点失败，则返回。
			if ((z = new RBTNode(key, BLACK, NULL, NULL, NULL)) == NULL)	return;
			ininsert(mRoot, z);
			++m_Size;
		}

		// 删除结点(key为节点键值)
		void remove(Key key){
			RBTNode *node;
			// 查找key对应的节点(node)，找到的话就删除该节点
			if ((node = search(mRoot, key)) != NULL)	remove(mRoot, node);
			--m_Size;
		}

		// 销毁红黑树
		void destroy() { destroy(mRoot); m_Size = 0; }
	private:
		//查找最小结点：返回tree为根结点的红黑树的最小结点。
		RBTNode* minimum(RBTNode* tree) const {
			if (tree == NULL)	return NULL;
			while (tree->left != NULL)	tree = tree->left;
			return tree;
		}

		//查找最大结点：返回tree为根结点的红黑树的最大结点。
		RBTNode* maximum(RBTNode* tree) const {
			if (tree == NULL)	return NULL;
			while (tree->right != NULL)    tree = tree->right;
			return tree;
		}

		// (递归实现)查找"红黑树x"中键值为key的节点
		RBTNode* search(RBTNode* x, Key key) const {
			if (x == NULL || !(com(x->v.first, key) || com(key, x->v.first)))	return x;
			if (com(key, x->v.first)) 	return search(x->left, key);
			else	return search(x->right, key);
		}

		// 左旋
		void leftRotate(RBTNode* &root, RBTNode* x)	{
			// 设置x的右孩子为y
			RBTNode *y = x->right;
			// 将 “y的左孩子” 设为 “x的右孩子”；
			// 如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
			x->right = y->left;
			if (y->left != NULL)	y->left->parent = x;
			// 将 “x的父亲” 设为 “y的父亲”
			y->parent = x->parent;
			if (x->parent == NULL)	root = y;    // 如果 “x的父亲” 是空节点，则将y设为根节点
			else{
				if (x->parent->left == x)
						x->parent->left = y;     // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
				else	x->parent->right = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
			}
			// 将 “x” 设为 “y的左孩子”
			y->left = x;
			// 将 “x的父节点” 设为 “y”
			x->parent = y;
		}

		// 右旋
		void rightRotate(RBTNode* &root, RBTNode* y) {
			// 设置x是当前节点的左孩子。
			RBTNode *x = y->left;
			// 将 “x的右孩子” 设为 “y的左孩子”；
			// 如果"x的右孩子"不为空的话，将 “y” 设为 “x的右孩子的父亲”
			y->left = x->right;
			if (x->right != NULL)	x->right->parent = y;
			// 将 “y的父亲” 设为 “x的父亲”
			x->parent = y->parent;
			if (y->parent == NULL)	root = x;     // 如果 “y的父亲” 是空节点，则将x设为根节点}
			else{
				if (y == y->parent->right)
						y->parent->right = x;    // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
				else	y->parent->left = x;     // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
			}
			// 将 “y” 设为 “x的右孩子”
			x->right = y;
			// 将 “y的父节点” 设为 “x”
			y->parent = x;
		}

		// 插入修正函数
		void insertFixUp(RBTNode* &root, RBTNode* node){
			RBTNode *parent, *gparent;
			// 若“父节点存在，并且父节点的颜色是红色”
			while ((parent = rb_parent(node)) && rb_is_red(parent)){
				gparent = rb_parent(parent);
				//若“父节点”是“祖父节点的左孩子”
				if (parent == gparent->left){
					// Case 1条件：叔叔节点是红色
					RBTNode *uncle = gparent->right;
					if (uncle && rb_is_red(uncle)){
						rb_set_black(uncle);
						rb_set_black(parent);
						rb_set_red(gparent);
						node = gparent;
						continue;
					}
					// Case 2条件：叔叔是黑色，且当前节点是右孩子
					if (parent->right == node){
						RBTNode *tmp;
						leftRotate(root, parent);
						tmp = parent;
						parent = node;
						node = tmp;
					}
					// Case 3条件：叔叔是黑色，且当前节点是左孩子。
					rb_set_black(parent);
					rb_set_red(gparent);
					rightRotate(root, gparent);
				}
				else{  //若“z的父节点”是“z的祖父节点的右孩子”
					// Case 1条件：叔叔节点是红色
					RBTNode *uncle = gparent->left;
					if (uncle && rb_is_red(uncle)){
						rb_set_black(uncle);
						rb_set_black(parent);
						rb_set_red(gparent);
						node = gparent;
						continue;
					}
					// Case 2条件：叔叔是黑色，且当前节点是左孩子
					if (parent->left == node){
						RBTNode *tmp;
						rightRotate(root, parent);
						tmp = parent;
						parent = node;
						node = tmp;
					}
					// Case 3条件：叔叔是黑色，且当前节点是右孩子。
					rb_set_black(parent);
					rb_set_red(gparent);
					leftRotate(root, gparent);
				}
			}
			// 将根节点设为黑色
			rb_set_black(root);
		}

		// 删除函数
		void remove(RBTNode* &root, RBTNode *node){
			RBTNode *child, *parent;
			RBTColor color;
			// 被删除节点的"左右孩子都不为空"的情况。
			if ((node->left != NULL) && (node->right != NULL)){
				// 被删节点的后继节点。(称为"取代节点")
				// 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
				RBTNode *replace = node;
				// 获取后继节点
				replace = replace->right;
				while (replace->left != NULL)	replace = replace->left;
				// "node节点"不是根节点(只有根节点不存在父节点)
				if (rb_parent(node)){
					if (rb_parent(node)->left == node)	rb_parent(node)->left = replace;
					else	rb_parent(node)->right = replace;
				}
				else	root = replace;		// "node节点"是根节点，更新根节点。
				// child是"取代节点"的右孩子，也是需要"调整的节点"。
				// "取代节点"肯定不存在左孩子！因为它是一个后继节点。
				child = replace->right;
				parent = rb_parent(replace);
				// 保存"取代节点"的颜色
				color = rb_color(replace);
				// "被删除节点"是"它的后继节点的父节点"
				if (parent == node)		parent = replace;
				else{
					// child不为空
					if (child)	rb_set_parent(child, parent);
					parent->left = child;
					replace->right = node->right;
					rb_set_parent(node->right, replace);
				}
				replace->parent = node->parent;
				replace->color = node->color;
				replace->left = node->left;
				node->left->parent = replace;
				if (color == BLACK)	removeFixUp(root, child, parent);
				delete node;
				return;
			}
			if (node->left != NULL)	child = node->left;
			else child = node->right;
			parent = node->parent;
			// 保存"取代节点"的颜色
			color = node->color;
			if (child) child->parent = parent;
			// "node节点"不是根节点
			if (parent){
				if (parent->left == node) parent->left = child;
				else parent->right = child;
			}
			else root = child;
			if (color == BLACK)	removeFixUp(root, child, parent);
			delete node;
		}

		// 删除修正函数
		void removeFixUp(RBTNode* &root, RBTNode *node, RBTNode *parent){
			RBTNode *other;
			while ((!node || rb_is_black(node)) && node != root){
				if (parent->left == node){
					other = parent->right;
					if (rb_is_red(other)){
						// Case 1: x的兄弟w是红色的  
						rb_set_black(other);
						rb_set_red(parent);
						leftRotate(root, parent);
						other = parent->right;
					}
					if ((!other->left || rb_is_black(other->left)) &&
						(!other->right || rb_is_black(other->right))){
						// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
						rb_set_red(other);
						node = parent;
						parent = rb_parent(node);
					}
					else{
						if (!other->right || rb_is_black(other->right)){
							// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
							rb_set_black(other->left);
							rb_set_red(other);
							rightRotate(root, other);
							other = parent->right;
						}
						// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
						rb_set_color(other, rb_color(parent));
						rb_set_black(parent);
						rb_set_black(other->right);
						leftRotate(root, parent);
						node = root;
						break;
					}
				}
				else{
					other = parent->left;
					if (rb_is_red(other)){
						// Case 1: x的兄弟w是红色的  
						rb_set_black(other);
						rb_set_red(parent);
						rightRotate(root, parent);
						other = parent->left;
					}
					if ((!other->left || rb_is_black(other->left)) &&
						(!other->right || rb_is_black(other->right))){
						// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
						rb_set_red(other);
						node = parent;
						parent = rb_parent(node);
					}
					else{
						if (!other->left || rb_is_black(other->left)){
							// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
							rb_set_black(other->right);
							rb_set_red(other);
							leftRotate(root, other);
							other = parent->left;
						}
						// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
						rb_set_color(other, rb_color(parent));
						rb_set_black(parent);
						rb_set_black(other->left);
						rightRotate(root, parent);
						node = root;
						break;
					}
				}
			}
			if (node) rb_set_black(node);
		}

		// 销毁红黑树
		void destroy(RBTNode* &tree){
			if (tree == NULL) return;
			if (tree->left != NULL)	destroy(tree->left);
			if (tree->right != NULL) destroy(tree->right);
			delete tree;
			tree = NULL;
		}

		//复制结点
		void nodecopy(RBTNode *n) {
			if (n == NULL) return;
			if (n->left != NULL) nodecopy(n->left);
			if (n->right != NULL) nodecopy(n->right);
			ininsert(n->v);
		}
	public:
		class const_iterator;
		class iterator {
			friend class map;
			friend class const_iterator;
		private:
			RBTNode *k;		//迭代器指向的结点
			map *t;			//迭代器指向的容器
		public:
			iterator() { k = NULL; t = NULL; }
			iterator(const iterator &other) { k = other.k; t = other.t; }

			iterator operator++(int){
				iterator tmp;
				tmp.k = this->k;
				tmp.t = this->t;

				this->k = t->successor(this->k);
				return tmp;
			}

			iterator & operator++() { this->k = t->successor(this->k); return *this; }

			iterator operator--(int){
				iterator tmp;
				tmp.k = this->k;
				tmp.t = this->t;

				this->k = t->predecessor(this->k);
				return tmp;
			}

			iterator & operator--() { this->k = t->predecessor(this->k); return *this; }

			value_type & operator*() const { return k->v; }

			bool operator==(const iterator &rhs) const { return (k == rhs.k&&t == rhs.t); }
			bool operator==(const const_iterator &rhs) const { return (k == rhs.ck&&t == rhs.ct); }
			bool operator!=(const iterator &rhs) const { return (k != rhs.k || t != rhs.t); }
			bool operator!=(const const_iterator &rhs) const { return (k != rhs.ck || t != rhs.ct); }

			value_type* operator->() const noexcept { return &(k->v); }
		};
		class const_iterator {
			friend class iterator;
			friend class map;
		public:
			const RBTNode *ck;		//迭代器指向的结点
			const map *ct;			//迭代器指向的容器

			const_iterator() { ck = NULL; ct = NULL; }
			const_iterator(const map *m, const RBTNode *r) :ct(m), ck(r) {}
			const_iterator(const const_iterator &other) { ck = other.ck;	ct = other.ct; }
			const_iterator(const iterator &other) { ck = other.k; ct = other.t; }

			const_iterator operator++(int){
				const_iterator tmp(*this);
				this->ck = ct->successor(this->ck);
				return tmp;
			}

			const_iterator & operator++() { this->ck = ct->successor(this->ck); return *this; }

			const_iterator operator--(int){
				const_iterator tmp(this->ct, this->ck);
				this->ck = ct->predecessor(this->ck);
				return tmp;
			}

			const_iterator & operator--() { this->ck = ct->predecessor(this->ck);	return *this; }

			const value_type & operator*() const { return ck->v; }

			bool operator==(const iterator &rhs) const { return (ck == rhs.k&&ct == rhs.t); }
			bool operator==(const const_iterator &rhs) const { return (ck == rhs.ck&&ct == rhs.ct); }
			bool operator!=(const iterator &rhs) const { return (ck != rhs.k || ct != rhs.t); }
			bool operator!=(const const_iterator &rhs) const { return (ck != rhs.ck || ct != rhs.ct); }

			const value_type* operator->() const noexcept { return &(ck->v); }
		};

		map() { m_Size = 0;	mRoot = NULL; m_NIL = NULL; }
		map(const map &other) {
			m_NIL = NULL;
			m_Size = 0;
			mRoot = NULL;
			nodecopy(other.mRoot);
		}

		map & operator=(const map &other) {
			if (this == &other) return *this;
			destroy();
			nodecopy(other.mRoot);
			return *this;
		}

		~map() { destroy(); }

		T & at(const Key &key){
			RBTNode* tmp = search(key);
			if (tmp == NULL) throw(index_out_of_bound());
			return (tmp->v.second);
		}
		const T & at(const Key &key) const {
			RBTNode* tmp = search(key);
			if (tmp == NULL) throw(index_out_of_bound());
			return (tmp->v.second);
		}

		T & operator[](const Key &key){
			RBTNode *tmp = search(key);
			if (tmp != NULL) return (tmp->v.second);
			else{
				value_type vv(key, T());
				RBTNode *z = new RBTNode(vv, BLACK, NULL, NULL, NULL);
				ininsert(mRoot, z);
				++m_Size;
				tmp = search(key);
				return (tmp->v.second);
			}
		}

		const T & operator[](const Key &key) const{
			T ans;
			RBTNode *tmp = search(key);
			if (tmp != NULL) ans = tmp->v.second;
			else throw(index_out_of_bound());
			return ans;
		}

		iterator begin(){
			iterator ans;
			ans.k = minimum(mRoot);
			ans.t = this;
			return ans;
		}
		const_iterator cbegin() const { const_iterator ans(this, minimum(mRoot));	return ans; }

		iterator end() { iterator ans; ans.k = m_NIL; ans.t = this; return ans; }
		const_iterator cend() const { const_iterator ans(this, m_NIL); return ans; }

		bool empty() const { return (mRoot == NULL); }

		size_t size() const { return m_Size; }

		void clear() {destroy();}

		pair<iterator, bool> insert(const value_type &value){
			bool judge;
			RBTNode *tmp = search(value.first);
			if (tmp != NULL) judge = false;
			else { ininsert(value); tmp = search(value.first); judge = true; }

			iterator i;
			i.t = this;
			i.k = tmp;
			pair<iterator, bool> p;
			p.first = i;
			p.second = judge;
			return p;
		}

		void erase(iterator pos){
			if (pos.k == m_NIL||this!=pos.t) throw(index_out_of_bound());
			else pos.t->remove(pos.k->v.first);
		}

		size_t count(const Key &key) const{
			RBTNode *tmp = search(key);
			if (tmp == NULL) return 0;
			else return 1;
		}

		iterator find(const Key &key){
			iterator ans;
			RBTNode *tmp = search(key);
			if (tmp == NULL) ans = end();
			else { ans.t = this; ans.k = tmp; }
			return ans;
		}

		const_iterator find(const Key &key) const{
			RBTNode *tmp = search(key);
			if (tmp == NULL) return cend();
			else { const_iterator ans(this, tmp);	return ans; }
		}
	};
}
#endif
