#pragma once
#include <cassert>
/*
RB_Tree(�����)������Ķ����������ÿ���ڵ����к��������ɫ��
*********************************************************************
�����������:
��1��ÿ���ڵ�����Ǻ�ɫ�������Ǻ�ɫ��
��2�����ڵ��Ǻ�ɫ��
��3��ÿ��Ҷ�ӽڵ㣨NIL���Ǻ�ɫ�� [ע�⣺����Ҷ�ӽڵ㣬��ָΪ��(NIL��NULL)��Ҷ�ӽڵ㣡]---ע��ýڵ㲻�ᴢ���κ����ݣ�
��4�����һ���ڵ��Ǻ�ɫ�ģ��������ӽڵ�����Ǻ�ɫ�ġ�
��5����һ���ڵ㵽�ýڵ������ڵ������·���ϰ�����ͬ��Ŀ�ĺڽڵ㡣---Ҫȷ��û��һ��·���������·�����ȳ������������ƽ�����������ƽ�Ⲣ������ô�ϸ�
*********************************************************************
��Ҫ�˽�����ʣ�
��̽ڵ㣺ָ���Ǵ���Ŵ�С��ӽ���ǰ�ڵ�Ľڵ㣨���������Ļ�����������С�ľ�����Ϊ��̽ڵ㣩
�游�ڵ㣺���ڵ�ĸ��ڵ�
�ֵܽڵ㣺һ���ڵ��丸�ڵ����һ���ڵ�
����ڵ㣺һ���ڵ��丸�ڵ���ֵܽڵ�

*/
namespace ddwl {
	enum RBTColor { RED, BLACK };//0��1
	template <class T>
	class RBTNode {
	public:
		RBTColor color;     // ��ɫ
		RBTNode *lChild;    // ����
		RBTNode *rChild;    // �Һ���
		RBTNode *parent;	// �����
		T value;            // �ؼ���(��ֵ)

		RBTNode(T value, RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r) :
			value(value), color(c), parent(p), lChild(l), rChild(r) {}
		RBTNode() {};
	};

	template <class T>
	class RBT {
	private:

		RBTNode<T> *root;							// �����
		RBTNode<T> *tail;                           // βָ��(������ʶ)
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
		// ����"�����x"�м�ֵΪval�Ľڵ�
		RBTNode<T>* search(RBTNode<T>* x, T key) const;
		// ������С��㣺���ص��ǽڵ�
		RBTNode<T>* minimum(RBTNode<T>* tree);
		// ��������㣺���ص��ǽڵ�
		RBTNode<T>* maximum(RBTNode<T>* tree);
		// ����
		void LRotate(RBTNode<T>* &root, RBTNode<T>* x);
		// ����
		void RRotate(RBTNode<T>* &root, RBTNode<T>* y);
		// ���뺯��
		void insert(RBTNode<T>* &root, RBTNode<T>* node);
		// ������������
		void insert_FixUp(RBTNode<T>* &root, RBTNode<T>* node);
		// ɾ������
		void Delete(RBTNode<T>* &root, RBTNode<T> *node);
		// ɾ����������
		void delete_FixUp(RBTNode<T>* &root, RBTNode<T> *node, RBTNode<T> *parent);
		// ���ٺ����
		void destroy(RBTNode<T>* &tree);
	public:
		class Iterator;
		explicit RBT();
		 ~RBT();
		// ����"�����"�м�ֵΪkey�Ľڵ�
		RBTNode<T>* find(T key);
		// ������С��㣺������С���ļ�ֵ��
		T minKey();
		// ��������㣺���������ļ�ֵ��
		T maxKey();
		// �ҽ��(x)�ĺ�̽�㡣��������"�����������ֵ���ڸý��"��"��С���"��
		RBTNode<T>* successor(RBTNode<T> *x);
		// �ҽ��(x)��ǰ����㡣��������"�����������ֵС�ڸý��"��"�����"��
		RBTNode<T>* predecessor(RBTNode<T> *x);

		// �����(keyΪ�ڵ��ֵ)���뵽�������
		void Insert(T key);

		// ɾ�����(keyΪ�ڵ��ֵ)
		void erase(T key);

		// ���ٺ����
		void clear();
		// Ԫ����Ŀ
		int size()const {
			return mSize;
		}
		// �Ƿ�Ϊ��
		bool empty()const {
			return !mSize;
		}
		//����ͷ�ڵ�
		Iterator begin();
		Iterator end();

		/****************************��Ƕiterator***************************************/
		class Iterator {
		private:
			RBTNode<T>* ptr;							//��ʼʱֱ��ָ��������ʼ�ڵ�
			RBT<T>* outPtr;								//�ⲿ��ָ��  ��Ϊ�˷����ⲿ��ĳ�Ա��
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
			Iterator operator++(int) {//ͨ���������ʵ�ִ�С�������
				if (ptr->rChild != NULL) {			//�Һ��Ӳ�Ϊ��  ��������һ��
					ptr = ptr->rChild;
					while (ptr->lChild != NULL)		//���Ӳ�Ϊ��һֱ������
						ptr = ptr->lChild;
					return  *this;
				}
				//����Һ���Ϊ��  �Ҹ��ڵ�ȵ�ǰ�ڵ�ֵҪС ˵��֮ǰ���ʹ� һֱ������
				while (nParent(ptr) != NULL && ptr->value > nParent(ptr)->value)
					ptr = nParent(ptr);

				if (nParent(ptr) == NULL) ptr = outPtr->tail;
				else ptr = nParent(ptr);

				return *this;
			}
			Iterator operator++() {//�������ͬ
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
	/********************************************������RBTree��Ա����********************************************************/
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
	* ������С��㣺����treeΪ�����ĺ��������С��㡣
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
	* ��������㣺����treeΪ�����ĺ����������㡣
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
	* �ҽ��(x)�ĺ�̽�㡣��������"�����������ֵ���ڸý��"��"��С���"��
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
	* �ҽ��(x)��ǰ����㡣��������"�����������ֵС�ڸý��"��"�����"��
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
		// ����x���Һ���Ϊy
		RBTNode<T> *y = x->rChild;

		// �� ��y�����ӡ� ��Ϊ ��x���Һ��ӡ���
		// ���y�����ӷǿգ��� ��x�� ��Ϊ ��y�����ӵĸ��ס�
		x->rChild = y->lChild;
		if (y->lChild != NULL)
			y->lChild->parent = x;

		// �� ��x�ĸ��ס� ��Ϊ ��y�ĸ��ס�
		y->parent = x->parent;

		if (x->parent == NULL) {
			root = y;            // ��� ��x�ĸ��ס� �ǿսڵ㣬��y��Ϊ���ڵ�
		}
		else {
			if (x->parent->lChild == x)
				x->parent->lChild = y;    // ��� x�������ڵ�����ӣ���y��Ϊ��x�ĸ��ڵ�����ӡ�
			else
				x->parent->rChild = y;    // ��� x�������ڵ�����ӣ���y��Ϊ��x�ĸ��ڵ�����ӡ�
		}

		// �� ��x�� ��Ϊ ��y�����ӡ�
		y->lChild = x;
		// �� ��x�ĸ��ڵ㡱 ��Ϊ ��y��
		x->parent = y;
	}

	template <class T>
	void RBT<T>::RRotate(RBTNode<T>* &root, RBTNode<T>* y) {
		// ����x�ǵ�ǰ�ڵ�����ӡ�
		RBTNode<T> *x = y->lChild;

		// �� ��x���Һ��ӡ� ��Ϊ ��y�����ӡ���
		// ���"x���Һ���"��Ϊ�յĻ����� ��y�� ��Ϊ ��x���Һ��ӵĸ��ס�
		y->lChild = x->rChild;
		if (x->rChild != NULL)
			x->rChild->parent = y;

		// �� ��y�ĸ��ס� ��Ϊ ��x�ĸ��ס�
		x->parent = y->parent;

		if (y->parent == NULL) {
			root = x;            // ��� ��y�ĸ��ס� �ǿսڵ㣬��x��Ϊ���ڵ�
		}
		else {
			if (y == y->parent->rChild)
				y->parent->rChild = x;    // ��� y�������ڵ���Һ��ӣ���x��Ϊ��y�ĸ��ڵ���Һ��ӡ�
			else
				y->parent->lChild = x;    // (y�������ڵ������) ��x��Ϊ��x�ĸ��ڵ�����ӡ�
		}

		// �� ��y�� ��Ϊ ��x���Һ��ӡ�
		x->rChild = y;

		// �� ��y�ĸ��ڵ㡱 ��Ϊ ��x��
		y->parent = x;
	}

	/*
	* �����������������
	*
	* ���������в���ڵ�֮��(ʧȥƽ��)���ٵ��øú�����
	* Ŀ���ǽ������������һ�ź������
	*
	* ����˵����
	*     root ������ĸ�
	*     node ����Ľ��        // ��Ӧ���㷨���ۡ��е�z
	*/
	template <class T>
	void RBT<T>::insert_FixUp(RBTNode<T>* &root, RBTNode<T>* node) {
		RBTNode<T> *parent, *grandparent;

		// �������ڵ���ڣ����Ҹ��ڵ����ɫ�Ǻ�ɫ��
		while ((parent = nParent(node)) && isRed(parent)) {
			grandparent = nParent(parent);

			//�������ڵ㡱�ǡ��游�ڵ�����ӡ�
			if (parent == grandparent->lChild) {
				// Case 1����������ڵ��Ǻ�ɫ
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

				// Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ���
				if (parent->rChild == node) {
					RBTNode<T> *tmp;
					LRotate(root, parent);
					tmp = parent;
					parent = node;
					node = tmp;
				}

				// Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ������ӡ�
				toBlack(parent);
				toRed(grandparent);
				RRotate(root, grandparent);
			}
			else {//����z�ĸ��ڵ㡱�ǡ�z���游�ڵ���Һ��ӡ�
				// Case 1����������ڵ��Ǻ�ɫ
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

				// Case 2�����������Ǻ�ɫ���ҵ�ǰ�ڵ�������
				if (parent->lChild == node) {
					RBTNode<T> *tmp;
					RRotate(root, parent);
					tmp = parent;
					parent = node;
					node = tmp;
				}

				// Case 3�����������Ǻ�ɫ���ҵ�ǰ�ڵ����Һ��ӡ�
				toBlack(parent);
				toRed(grandparent);
				LRotate(root, grandparent);
			}
		}

		// �����ڵ���Ϊ��ɫ
		toBlack(root);
	}
	/*
	* �������뵽�������
	*
	* ����˵����
	*     root ������ĸ����
	*     node ����Ľ��        // ��Ӧ���㷨���ۡ��е�node
	*/
	template <class T>
	void RBT<T>::insert(RBTNode<T>* &root, RBTNode<T>* node) {
		RBTNode<T> *y = NULL;
		RBTNode<T> *x = root;

		//�����������һ�Ŷ�������������ڵ���ӵ�����������С�
		while (x != NULL&&x->value != node->value) {
			y = x;
			if (node->value < x->value)
				x = x->lChild;
			else if (node->value > x->value)
				x = x->rChild;
		}
		//�Ѿ�������ͬ�ڵ㲻�ٲ���
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
		// 2. ���ýڵ����ɫΪ��ɫ
		node->color = RED;

		// 3. ������������Ϊһ�Ŷ��������
		insert_FixUp(root, node);

		mSize += 1;
	}

	template <class T>
	void RBT<T>::Insert(T key) {
		RBTNode<T> *z = NULL;

		// ����½����ʧ�ܣ��򷵻ء�
		if ((z = new RBTNode<T>(key, BLACK, NULL, NULL, NULL)) == NULL)
			return;

		insert(root, z);
	}

	/*
	* �����ɾ����������
	*
	* �ڴӺ������ɾ������ڵ�֮��(�����ʧȥƽ��)���ٵ��øú�����
	* Ŀ���ǽ������������һ�ź������
	*
	* ����˵����
	*     root ������ĸ�
	*     node �������Ľڵ�
	*/
	template <class T>
	void RBT<T>::delete_FixUp(RBTNode<T>* &root, RBTNode<T> *node, RBTNode<T> *parent) {
		RBTNode<T> *other;

		while ((!node || isBlack(node)) && node != root) {
			if (parent->lChild == node) {
				other = parent->rChild;
				if (isRed(other)) {
					// Case 1: x���ֵ�w�Ǻ�ɫ��
					toBlack(other);
					toRed(parent);
					LRotate(root, parent);
					other = parent->rChild;
				}
				if ((!other->lChild || isBlack(other->lChild)) &&
					(!other->rChild || isBlack(other->rChild)))
				{
					// Case 2: x���ֵ�w�Ǻ�ɫ����w����������Ҳ���Ǻ�ɫ��
					toRed(other);
					node = parent;
					parent = nParent(node);
				}
				else {
					if (!other->rChild || isBlack(other->rChild)) {
						// Case 3: x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��
						toBlack(other->lChild);
						toRed(other);
						RRotate(root, other);
						other = parent->rChild;
					}
					// Case 4: x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ��
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
					// Case 1: x���ֵ�w�Ǻ�ɫ��
					toBlack(other);
					toRed(parent);
					RRotate(root, parent);
					other = parent->lChild;
				}
				if ((!other->lChild || isBlack(other->lChild)) &&
					(!other->rChild || isBlack(other->rChild)))
				{
					// Case 2: x���ֵ�w�Ǻ�ɫ����w����������Ҳ���Ǻ�ɫ��
					toRed(other);
					node = parent;
					parent = nParent(node);
				}
				else {
					if (!other->lChild || isBlack(other->lChild)) {
						// Case 3: x���ֵ�w�Ǻ�ɫ�ģ�����w�������Ǻ�ɫ���Һ���Ϊ��ɫ��
						toBlack(other->rChild);
						toRed(other);
						LRotate(root, other);
						other = parent->lChild;
					}
					// Case 4: x���ֵ�w�Ǻ�ɫ�ģ�����w���Һ����Ǻ�ɫ�ģ�����������ɫ��
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

		// ��ɾ���ڵ��"���Һ��Ӷ���Ϊ��"�������
		if ((node->lChild != NULL) && (node->rChild != NULL)) {
			// ��ɾ�ڵ�ĺ�̽ڵ㡣(��Ϊ"ȡ���ڵ�")
			// ������ȡ��"��ɾ�ڵ�"��λ�ã�Ȼ���ٽ�"��ɾ�ڵ�"ȥ����
			RBTNode<T> *Successor = node;

			// ��ȡ��̽ڵ�
			Successor = Successor->rChild;
			while (Successor->lChild != NULL)
				Successor = Successor->lChild;

			// "node�ڵ�"���Ǹ��ڵ�(ֻ�и��ڵ㲻���ڸ��ڵ�)
			if (nParent(node)) {
				if (nParent(node)->lChild == node)
					nParent(node)->lChild = Successor;
				else
					nParent(node)->rChild = Successor;
			}
			else
				// "node�ڵ�"�Ǹ��ڵ㣬���¸��ڵ㡣
				root = Successor;

			// child��"ȡ���ڵ�"���Һ��ӣ�Ҳ����Ҫ"�����Ľڵ�"��
			// "ȡ���ڵ�"�϶����������ӣ���Ϊ����һ����̽ڵ㡣
			child = Successor->rChild;
			parent = nParent(Successor);
			// ����"ȡ���ڵ�"����ɫ
			color = nColor(Successor);

			// "��ɾ���ڵ�"��"���ĺ�̽ڵ�ĸ��ڵ�"
			if (parent == node) {
				parent = Successor;
			}
			else {
				// child��Ϊ��
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
		// ����"ȡ���ڵ�"����ɫ
		color = node->color;

		if (child)
			child->parent = parent;

		// "node�ڵ�"���Ǹ��ڵ�
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

		// ����key��Ӧ�Ľڵ�(node)���ҵ��Ļ���ɾ���ýڵ�
		if ((node = search(root, key)) != NULL) {
			Delete(root, node);
			mSize -= 1;
		}
	}

	//���ٺ����

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