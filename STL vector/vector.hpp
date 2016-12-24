#pragma once
#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <malloc.h>
#include <memory>

namespace sjtu {
	template<typename T>
	class vector {
	private:
		T * elem;		//存储元素的数组
		size_t sz;		//当前元素个数
		size_t cpct;	//数组容量
		std::allocator<T> alloc;

		void doublespace(){
			T * tmp;
			tmp = elem;
			cpct = 2 * cpct;
			elem = (T*)malloc(cpct*sizeof(T));
			memset(elem, 0, cpct * sizeof(T));
			for (int i = 0; i<sz; ++i) { elem[i] = tmp[i]; }
			for (size_t i = 0; i < sz; ++i) tmp[i].~T();
			free(tmp);
		}
	public:
		class const_iterator;
		class iterator {
			friend class const_iterator;
			friend class vector;
		private:
			unsigned loc;	//指向的位置
			T * ielem;		//指向的数组
		public:
			iterator() { loc = 0; ielem = NULL; }
			~iterator() {}
			iterator(const iterator &right) { ielem = right.ielem; loc = right.loc; }

			iterator operator+(const int &n) const {
				iterator res;
				res.ielem = ielem;
				res.loc = loc + n;
				return res;
			}

			iterator operator-(const int &n) const {
				iterator res;
				res.ielem = ielem;
				res.loc = loc - n;
				return res;
			}

			int operator-(const iterator &rhs) const {
				if (rhs.ielem != ielem) throw invalid_iterator();
				else if (loc>rhs.loc) return loc - rhs.cloc;
				else return rhs.cloc - loc;
			}

			iterator operator+=(const int &n) {
				iterator res;
				res.ielem = ielem;
				res.loc = loc + n;
				return res;
			}

			iterator operator-=(const int &n) {
				iterator res;
				res.ielem = ielem;
				res.loc = loc - n;
				return res;
			}

			iterator operator++(int){
				iterator res;
				res.ielem = ielem;
				res.loc = loc;
				++loc;
				return res;
			}

			iterator& operator++() { ++loc; return *this; }

			iterator operator--(int){
				iterator res;
				res.ielem = ielem;
				res.loc = loc;
				--loc;
				return res;
			}

			iterator& operator--() { --loc;	return *this; }

			T & operator*() const { return ielem[loc]; }

			bool operator==(const iterator &rhs) const { return (ielem == rhs.ielem&&loc == rhs.loc); }
			bool operator==(const const_iterator &rhs) const { return (ielem == rhs.cielem&&loc == rhs.cloc); }
			bool operator!=(const iterator &rhs) const { return (ielem != rhs.ielem || loc != rhs.loc); }
			bool operator!=(const const_iterator &rhs) const { return (ielem != rhs.cielem || loc != rhs.cloc); }
		};

		class const_iterator {
			friend class iterator;
			friend class vector;
		private:
			unsigned cloc;	//指向的位置
			T * cielem;		//指向的数组
		public:
			const_iterator(const const_iterator &right) { cielem = right.cielem; cloc = right.cloc; }
			const_iterator() { cloc = 0; cielem = NULL; }

			bool operator==(const iterator &rhs) const { return (cielem == rhs.ielem&&cloc == rhs.loc); }
			bool operator==(const const_iterator &rhs) const { return (cielem == rhs.cielem&&cloc == rhs.cloc); }
			bool operator!=(const iterator &rhs) const { return (cielem != rhs.ielem || cloc != rhs.loc); }
			bool operator!=(const const_iterator &rhs) const { return (cielem != rhs.cielem || cloc != rhs.cloc); }

			const_iterator& operator++() { ++cloc; return *this; }

			T & operator*() const { return cielem[cloc]; }
		};

		vector() { elem = alloc.allocate(10); cpct = 10; sz = 0; }

		vector(const vector &other){
			sz = other.size();
			std::allocator<T> alloc;
			elem = alloc.allocate(sizeof(T) * 10);

			for (int i = 0; i<sz; ++i)	elem[i] = other.elem[i];
			cpct = sz;
		}

		~vector() { for (size_t i = 0; i < sz; ++i)	elem[i].~T(); free(elem); }

		vector &operator=(const vector &other){
			if (this == &other) return *this;

			T * tmp;
			tmp = elem;
			sz = other.size();
			elem = new T[sz];
			for (int i = 0; i<sz; i++) elem[i] = other.elem[i];
			cpct = sz;
			delete[]tmp;
		}

		T & at(const size_t &pos) { if (pos < 0 || pos >= sz) { throw index_out_of_bound(); } return elem[pos]; }
		const T & at(const size_t &pos) const { if (pos < 0 || pos >= sz) { throw index_out_of_bound(); }return elem[pos]; }

		T & operator[](const size_t &pos) { if (pos < 0 || pos >= sz) throw index_out_of_bound(); else return elem[pos]; }
		const T & operator[](const size_t &pos) const { if (pos < 0 || pos >= sz) throw index_out_of_bound(); else return elem[pos]; }

		const T & front() const { if (sz == 0) throw container_is_empty(); return elem[0]; }
		const T & back() const { if (sz == 0) throw container_is_empty(); return elem[sz - 1]; }

		iterator begin() { iterator res; res.loc = 0; res.ielem = elem; return res; }
		const_iterator cbegin() const { const_iterator res; res.cloc = 0; res.cielem = elem; return res; }
		iterator end() { iterator res; res.loc = sz; res.ielem = elem; return res; }
		const_iterator cend() const { const_iterator res; res.cloc = sz; res.cielem = elem; return res; }

		bool empty() const { return (sz == 0); }

		size_t size() const { return sz; }
		size_t capacity() const { return cpct; }

		void clear() { sz = 0; }

		iterator insert(iterator pos, const T &value){
			int i;
			if (sz == cpct) doublespace();
			++sz;
			for (i = sz; i > pos.loc; --i) elem[i] = elem[i - 1];
			elem[pos.loc] = value;
			return pos;
		}

		iterator insert(const size_t &ind, const T &value){
			if (ind>sz) throw index_out_of_bound();

			int i;
			iterator res;
			if (sz == cpct) doublespace();
			for (i = sz - 1; i >= ind; --i) elem[i + 1] = elem[i];
			elem[ind] = value;
			++sz;
			res.ielem = elem;
			res.loc = ind;
			return res;
		}

		iterator erase(iterator pos){
			for (int i = pos.loc; i<sz - 1; ++i) elem[i] = elem[i + 1];
			--sz;
			return pos;
		}

		iterator erase(const size_t &ind){
			if (ind >= sz) throw index_out_of_bound();
			iterator res;
			for (int i = ind; i<sz - 1; ++i) elem[i] = elem[i + 1];
			--sz;
			res.ielem = elem;
			res.loc = ind;
			return res;
		}

		void push_back(const T &value){
			if (sz == cpct) doublespace();
			alloc.construct(elem + sz, value);
			++sz;
		}

		void pop_back(){
			if (sz == 0) throw container_is_empty();
			--sz;
		}
	};
}
