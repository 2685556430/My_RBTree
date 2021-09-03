#pragma once
#include <cassert>
/*
RB_Tree(红黑树)：特殊的二叉查找树，每个节点上有红黑两种颜色。
*********************************************************************
红黑树的特性:
（1）每个节点或者是黑色，或者是红色。
（2）根节点是黑色。
（3）每个叶子节点（NIL）是黑色。 [注意：这里叶子节点，是指为空(NIL或NULL)的叶子节点！]---注意该节点不会储存任何数据！
（4）如果一个节点是红色的，则它的子节点必须是黑色的。
（5）从一个节点到该节点的子孙节点的所有路径上包含相同数目的黑节点。---要确保没有一条路径会比其他路径长度长出两倍，相比平衡二叉树这种平衡并不是那么严格。
*********************************************************************
需要了解的名词：
后继节点：指的是存放着大小最接近当前节点的节点（左子树最大的或者右子树最小的均可作为后继节点）
祖父节点：父节点的父节点
兄弟节点：一个节点其父节点的另一个节点
叔叔节点：一个节点其父节点的兄弟节点

*/
namespace ddwl {
	enum RBTColor { RED, BLACK };//0、1
	template <class T>
	class RBTNode {
	public:
		RBTColor color;     // 颜色
		RBTNode *lChild;    // 左孩子
		RBTNode *rChild;    // 右孩子
		RBTNode *parent;	// 父结点
		T value;            // 关键字(键值)

		RBTNode(T value, RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r) :
			value(value), color(c), parent(p), lChild(l), rChild(r) {}
		RBTNode() {};
	};

	template <class T>
	class RBT {
	private:

		RBTNode<T> *root;							// 根结点
		RBTNode<T> *tail;                           // 尾指针(结束标识)
		int mSize;
		#define nParent(r)		 (r->parent)
		#define nColor(r)		 (r->color)
		#define isRed(r)		 (r->color == RED)
		#define isBlack(r)		 (r->color == BLACK)
		#define toBlack(r)		 (r->color = BLACK)
		#define toRed(r)		 (r->color = RED)
		#define setParent(r,p)	 (r->parent = p)
		#define setColor(r,c)	 (r->color = c)
	private:
		// 查找"红黑树x"中键值为val的节点
		RBTNode<T>* search(RBTNode<T>* x, T key) const;
		// 查找最小结点：返回的是节点
		RBTNode<T>* minimum(RBTNode<T>* tree);
		// 查找最大结点：返回的是节点
		RBTNode<T>* maximum(RBTNode<T>* tree);
		// 左旋
		void LRotate(RBTNode<T>* &root, RBTNode<T>* x);
		// 右旋
		void RRotate(RBTNode<T>* &root, RBTNode<T>* y);
		// 插入函数
		void insert(RBTNode<T>* &root, RBTNode<T>* node);
		// 插入修正函数
		void insert_FixUp(RBTNode<T>* &root, RBTNode<T>* node);
		// 删除函数
		void Delete(RBTNode<T>* &root, RBTNode<T> *node);
		// 删除修正函数
		void delete_FixUp(RBTNode<T>* &root, RBTNode<T> *node, RBTNode<T> *parent);
		// 销毁红黑树
		void destroy(RBTNode<T>* &tree);
	public:
		class Iterator;
		explicit RBT();
		 ~RBT();
		// 查找"红黑树"中键值为key的节点
		RBTNode<T>* find(T key);
		// 查找最小结点：返回最小结点的键值。
		T minKey();
		// 查找最大结点：返回最大结点的键值。
		T maxKey();
		// 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
		RBTNode<T>* successor(RBTNode<T> *x);
		// 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
		RBTNode<T>* predecessor(RBTNode<T> *x);

		// 将结点(key为节点键值)插入到红黑树中
		void Insert(T key);

		// 删除结点(key为节点键值)
		void erase(T key);

		// 销毁红黑树
		void clear();
		// 元素数目
		int size()const {
			return mSize;
		}
		// 是否为空
		bool empty()const {
			return !mSize;
		}
		//返回头节点
		Iterator begin();
		Iterator end();

		/****************************内嵌iterator***************************************/
		class Iterator {
		private:
			RBTNode<T>* ptr;							//初始时直接指向树的起始节点
			RBT<T>* outPtr;								//外部类指针  （为了访问外部类的成员）
		public:
			explicit Iterator() {};
			explicit Iterator(RBT<T>* out, RBTNode<T>* p) :outPtr(out), ptr(p) {};
			~Iterator() {};
			T operator*()const {
				assert(ptr != NULL);
				return ptr->value;
			}
			Iterator operator=(Iterator& it) {
				outPtr = it.outPtr;
				ptr = it.ptr;
				return *this;
			}
			Iterator operator++(int) {//通过中序遍历实现从小到大查找
				if (ptr->rChild != NULL) {			//右孩子不为空  则向右走一步
					ptr = ptr->rChild;
					while (ptr->lChild != NULL)		//左孩子不为空一直向左走
						ptr = ptr->lChild;
					return  *this;
				}
				//如果右孩子为空  且父节点比当前节点值要小 说明之前访问过 一直向上走
				while (nParent(ptr) != NULL && ptr->value > nParent(ptr)->value)
					ptr = nParent(ptr);

				if (nParent(ptr) == NULL) ptr = outPtr->tail;
				else ptr = nParent(ptr);

				return *this;
			}
			Iterator operator++() {//与后置相同
				if (ptr->rChild != NULL) {
					ptr = ptr->rChild;
					while (ptr->lChild != NULL)
						ptr = ptr->lChild;
					return *this;
				}
				while (nParent(ptr) != NULL && ptr->value > nParent(ptr)->value)
					ptr = nParent(ptr);

				if (nParent(ptr) == NULL) ptr = outPtr->end();
				else ptr = nParent(ptr);
				
				return *this;
			}
			bool operator != (Iterator& it)const {
				return (ptr != it.ptr && ptr != NULL);
			}
			bool operator == (Iterator& it)const {
				return (ptr == it.ptr && ptr != NULL);
			}
		};

	};
	template <class T>
	using Myset = RBT<T>;
	/********************************************以下是RBTree成员函数********************************************************/
	template <class T>
	RBT<T>::RBT() :root(NULL) {
		mSize = 0;
		root = NULL;
		tail = new RBTNode<T>();
	}
	template <class T>
	RBT<T>::~RBT() {
		destroy(root);
	}
	template <class T>
	RBTNode<T>* RBT<T>::search(RBTNode<T>* x, T key) const {
		while ((x != NULL) && (x->value != key)) {
			if (key < x->value)
				x = x->lChild;
			else
				x = x->rChild;
		}

		return x;
	}

	template <class T>
	RBTNode<T>* RBT<T>::find(T key) {
		search(root, key);
	}

	/*
	* 查找最小结点：返回tree为根结点的红黑树的最小结点。
	*/
	template <class T>
	RBTNode<T>* RBT<T>::minimum(RBTNode<T>* tree) {
		if (tree == NULL)
			return NULL;

		while (tree->lChild != NULL)
			tree = tree->lChild;
		return tree;
	}

	template <class T>
	T RBT<T>::minKey() {
		RBTNode<T> *p = minimum(root);
		assert(p != NULL);
		return p->value;
	}

	/*
	* 查找最大结点：返回tree为根结点的红黑树的最大结点。
	*/
	template <class T>
	RBTNode<T>* RBT<T>::maximum(RBTNode<T>* tree) {
		if (tree == NULL)
			return NULL;

		while (tree->rChild != NULL)
			tree = tree->rChild;
		return tree;
	}

	template <class T>
	T RBT<T>::maxKey()
	{
		RBTNode<T> *p = maximum(root);
		assert(p != NULL);
		return p->value;
	}

	/*
	* 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
	*/
	template <class T>
	RBTNode<T>* RBT<T>::successor(RBTNode<T> *x) {
		if (x->rChild != NULL)
			return minimum(x->rChild);

		RBTNode<T>* y = x->parent;
		while ((y != NULL) && (x == y->rChild)) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	/*
	* 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
	*/
	template <class T>
	RBTNode<T>* RBT<T>::predecessor(RBTNode<T> *x) {
		if (x->lChild != NULL)
			return maximum(x->lChild);

		RBTNode<T>* y = x->parent;
		while ((y != NULL) && (x == y->lChild)) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	template <class T>
	void RBT<T>::LRotate(RBTNode<T>* &root, RBTNode<T>* x) {
		// 设置x的右孩子为y
		RBTNode<T> *y = x->rChild;

		// 将 “y的左孩子” 设为 “x的右孩子”；
		// 如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
		x->rChild = y->lChild;
		if (y->lChild != NULL)
			y->lChild->parent = x;

		// 将 “x的父亲” 设为 “y的父亲”
		y->parent = x->parent;

		if (x->parent == NULL) {
			root = y;            // 如果 “x的父亲” 是空节点，则将y设为根节点
		}
		else {
			if (x->parent->lChild == x)
				x->parent->lChild = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
			else
				x->parent->rChild = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
		}

		// 将 “x” 设为 “y的左孩子”
		y->lChild = x;
		// 将 “x的父节点” 设为 “y”
		x->parent = y;
	}

	template <class T>
	void RBT<T>::RRotate(RBTNode<T>* &root, RBTNode<T>* y) {
		// 设置x是当前节点的左孩子。
		RBTNode<T> *x = y->lChild;

		// 将 “x的右孩子” 设为 “y的左孩子”；
		// 如果"x的右孩子"不为空的话，将 “y” 设为 “x的右孩子的父亲”
		y->lChild = x->rChild;
		if (x->rChild != NULL)
			x->rChild->parent = y;

		// 将 “y的父亲” 设为 “x的父亲”
		x->parent = y->parent;

		if (y->parent == NULL) {
			root = x;            // 如果 “y的父亲” 是空节点，则将x设为根节点
		}
		else {
			if (y == y->parent->rChild)
				y->parent->rChild = x;    // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
			else
				y->parent->lChild = x;    // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
		}

		// 将 “y” 设为 “x的右孩子”
		x->rChild = y;

		// 将 “y的父节点” 设为 “x”
		y->parent = x;
	}

	/*
	* 红黑树插入修正函数
	*
	* 在向红黑树中插入节点之后(失去平衡)，再调用该函数；
	* 目的是将它重新塑造成一颗红黑树。
	*
	* 参数说明：
	*     root 红黑树的根
	*     node 插入的结点        // 对应《算法导论》中的z
	*/
	template <class T>
	void RBT<T>::insert_FixUp(RBTNode<T>* &root, RBTNode<T>* node) {
		RBTNode<T> *parent, *grandparent;

		// 若“父节点存在，并且父节点的颜色是红色”
		while ((parent = nParent(node)) && isRed(parent)) {
			grandparent = nParent(parent);

			//若“父节点”是“祖父节点的左孩子”
			if (parent == grandparent->lChild) {
				// Case 1条件：叔叔节点是红色
				{
					RBTNode<T> *uncle = grandparent->rChild;
					if (uncle && isRed(uncle)) {
						toBlack(uncle);
						toBlack(parent);
						toRed(grandparent);
						node = grandparent;
						continue;
					}
				}

				// Case 2条件：叔叔是黑色，且当前节点是右孩子
				if (parent->rChild == node) {
					RBTNode<T> *tmp;
					LRotate(root, parent);
					tmp = parent;
					parent = node;
					node = tmp;
				}

				// Case 3条件：叔叔是黑色，且当前节点是左孩子。
				toBlack(parent);
				toRed(grandparent);
				RRotate(root, grandparent);
			}
			else {//若“z的父节点”是“z的祖父节点的右孩子”
				// Case 1条件：叔叔节点是红色
				{
					RBTNode<T> *uncle = grandparent->lChild;
					if (uncle && isRed(uncle)) {
						toBlack(uncle);
						toBlack(parent);
						toRed(grandparent);
						node = grandparent;
						continue;
					}
				}

				// Case 2条件：叔叔是黑色，且当前节点是左孩子
				if (parent->lChild == node) {
					RBTNode<T> *tmp;
					RRotate(root, parent);
					tmp = parent;
					parent = node;
					node = tmp;
				}

				// Case 3条件：叔叔是黑色，且当前节点是右孩子。
				toBlack(parent);
				toRed(grandparent);
				LRotate(root, grandparent);
			}
		}

		// 将根节点设为黑色
		toBlack(root);
	}
	/*
	* 将结点插入到红黑树中
	*
	* 参数说明：
	*     root 红黑树的根结点
	*     node 插入的结点        // 对应《算法导论》中的node
	*/
	template <class T>
	void RBT<T>::insert(RBTNode<T>* &root, RBTNode<T>* node) {
		RBTNode<T> *y = NULL;
		RBTNode<T> *x = root;

		//将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
		while (x != NULL&&x->value != node->value) {
			y = x;
			if (node->value < x->value)
				x = x->lChild;
			else if (node->value > x->value)
				x = x->rChild;
		}
		//已经存在相同节点不再插入
		if (x != NULL) return;

		node->parent = y;
		if (y != NULL) {
			if (node->value < y->value)
				y->lChild = node;
			else
				y->rChild = node;
		}
		else
			root = node;
		// 2. 设置节点的颜色为红色
		node->color = RED;

		// 3. 将它重新修正为一颗二叉查找树
		insert_FixUp(root, node);

		mSize += 1;
	}

	template <class T>
	void RBT<T>::Insert(T key) {
		RBTNode<T> *z = NULL;

		// 如果新建结点失败，则返回。
		if ((z = new RBTNode<T>(key, BLACK, NULL, NULL, NULL)) == NULL)
			return;

		insert(root, z);
	}

	/*
	* 红黑树删除修正函数
	*
	* 在从红黑树中删除插入节点之后(红黑树失去平衡)，再调用该函数；
	* 目的是将它重新塑造成一颗红黑树。
	*
	* 参数说明：
	*     root 红黑树的根
	*     node 待修正的节点
	*/
	template <class T>
	void RBT<T>::delete_FixUp(RBTNode<T>* &root, RBTNode<T> *node, RBTNode<T> *parent) {
		RBTNode<T> *other;

		while ((!node || isBlack(node)) && node != root) {
			if (parent->lChild == node) {
				other = parent->rChild;
				if (isRed(other)) {
					// Case 1: x的兄弟w是红色的
					toBlack(other);
					toRed(parent);
					LRotate(root, parent);
					other = parent->rChild;
				}
				if ((!other->lChild || isBlack(other->lChild)) &&
					(!other->rChild || isBlack(other->rChild)))
				{
					// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
					toRed(other);
					node = parent;
					parent = nParent(node);
				}
				else {
					if (!other->rChild || isBlack(other->rChild)) {
						// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
						toBlack(other->lChild);
						toRed(other);
						RRotate(root, other);
						other = parent->rChild;
					}
					// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
					setColor(other, nColor(parent));
					toBlack(parent);
					toBlack(other->rChild);
					LRotate(root, parent);
					node = root;
					break;
				}
			}
			else {
				other = parent->lChild;
				if (isRed(other)) {
					// Case 1: x的兄弟w是红色的
					toBlack(other);
					toRed(parent);
					RRotate(root, parent);
					other = parent->lChild;
				}
				if ((!other->lChild || isBlack(other->lChild)) &&
					(!other->rChild || isBlack(other->rChild)))
				{
					// Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
					toRed(other);
					node = parent;
					parent = nParent(node);
				}
				else {
					if (!other->lChild || isBlack(other->lChild)) {
						// Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
						toBlack(other->rChild);
						toRed(other);
						LRotate(root, other);
						other = parent->lChild;
					}
					// Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
					setColor(other, nColor(parent));
					toBlack(parent);
					toBlack(other->lChild);
					RRotate(root, parent);
					node = root;
					break;
				}
			}
		}
		if (node)
			toBlack(node);
	}
	template <class T>
	void RBT<T>::Delete(RBTNode<T>* &root, RBTNode<T> *node) {
		RBTNode<T> *child, *parent;
		RBTColor color;

		// 被删除节点的"左右孩子都不为空"的情况。
		if ((node->lChild != NULL) && (node->rChild != NULL)) {
			// 被删节点的后继节点。(称为"取代节点")
			// 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
			RBTNode<T> *Successor = node;

			// 获取后继节点
			Successor = Successor->rChild;
			while (Successor->lChild != NULL)
				Successor = Successor->lChild;

			// "node节点"不是根节点(只有根节点不存在父节点)
			if (nParent(node)) {
				if (nParent(node)->lChild == node)
					nParent(node)->lChild = Successor;
				else
					nParent(node)->rChild = Successor;
			}
			else
				// "node节点"是根节点，更新根节点。
				root = Successor;

			// child是"取代节点"的右孩子，也是需要"调整的节点"。
			// "取代节点"肯定不存在左孩子！因为它是一个后继节点。
			child = Successor->rChild;
			parent = nParent(Successor);
			// 保存"取代节点"的颜色
			color = nColor(Successor);

			// "被删除节点"是"它的后继节点的父节点"
			if (parent == node) {
				parent = Successor;
			}
			else {
				// child不为空
				if (child)
					setParent(child, parent);
				parent->lChild = child;

				Successor->rChild = node->rChild;
				setParent(node->rChild, Successor);
			}

			Successor->parent = node->parent;
			Successor->color = node->color;
			Successor->lChild = node->lChild;
			node->lChild->parent = Successor;

			if (color == BLACK)
				delete_FixUp(root, child, parent);

			delete node;
			return;
		}

		if (node->lChild != NULL)
			child = node->lChild;
		else
			child = node->rChild;

		parent = node->parent;
		// 保存"取代节点"的颜色
		color = node->color;

		if (child)
			child->parent = parent;

		// "node节点"不是根节点
		if (parent) {
			if (parent->lChild == node)
				parent->lChild = child;
			else
				parent->rChild = child;
		}
		else
			root = child;

		if (color == BLACK)
			delete_FixUp(root, child, parent);
		delete node;
	}

	template <class T>
	void RBT<T>::erase(T key) {
		RBTNode<T> *node;

		// 查找key对应的节点(node)，找到的话就删除该节点
		if ((node = search(root, key)) != NULL) {
			Delete(root, node);
			mSize -= 1;
		}
	}

	//销毁红黑树

	template <class T>
	void RBT<T>::destroy(RBTNode<T>* &tree) {
		if (tree == NULL)
			return;

		if (tree->lChild != NULL)
			return destroy(tree->lChild);
		if (tree->rChild != NULL)
			return destroy(tree->rChild);

		delete tree;
		tree = NULL;
	}

	template <class T>
	void RBT<T>::clear() {
		destroy(root);
		mSize = 0;
	}
	template <class T>
	typename RBT<T>::Iterator RBT<T>::begin() {
		return Iterator(this, minimum(root));
	}
	template <class T>
	typename RBT<T>::Iterator RBT<T>::end() {
		return Iterator(this, tail);
	}

}