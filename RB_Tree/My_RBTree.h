#pragma once
typedef bool COLOR;//���ڱ�ʾ�ڵ���ɫ
typedef int DIR;
#define NULL 0
#define LEFT -1
#define RIGHT 1
#define ROOT 0
#define RED 0
#define BLACK 1
#define isRed(x) (x != NULL && x->c == RED) //�սڵ�Ĭ���Ǻڽڵ㣨�ж��Ƿ�Ϊ��ɫ�ڵ㣩
#define isNull(x) (x == NULL)				//�����ǲ��ǿսڵ�
#define setColor(x, COL) (x = COL)		//Ⱦɫ
template<class T>
class MySet{
private:
	typedef struct RB_Tree {
		COLOR c;
		T value;
		RB_Tree *lChild;
		RB_Tree *rChild;
		RB_Tree *parent;
	}RBT;
	RBT *root;
	int curSize;
	void setNode(RBT *node, RBT *p, T val) {
		setColor(node->c, RED);//�½ڵ���ɫΪ��ɫ
		node->lChild = NULL;
		node->rChild = NULL;
		node->parent = p;
		node->value = val;
	}
	RBT *LRotate(RBT *root, RBT *node) {//��nodeΪ��������
		RBT *parent = node->parent;
		RBT *rChild = node->rChild;
		node->rChild = rChild->lChild;
		if (!isNull(rChild->lChild)) {//����Һ��ӵ����Ӳ��ǿսڵ��ٽ��丸�ڵ����
			rChild->lChild->parent = node;
		}

		rChild->parent = parent;//�� parent ��Ϊ rChild �µĵĸ��ڵ�
		if (isNull(parent)) {//���parent�ǿ�˵����ǰ�ڵ�ֻ������
			root = rChild;
		}
		else if (parent->lChild == node) {//���������parent�ڵ�����ӵ����� �� �Һ��ӵ�����
			parent->lChild = rChild;
		}
		else parent->rChild = rChild;

		node->parent = rChild;
		rChild->lChild = node;
		return root;
	}

	RBT* RRotate(RBT *root, RBT *node) {//��nodeΪ�������� �������ֻҪ���� �� �� ����
		RBT *parent = node->parent;
		RBT *lChild = node->lChild;
		node->lChild = lChild->rChild;
		if (!isNull(lChild->rChild)) {
			lChild->rChild->parent = node;
		}

		lChild->parent = parent;
		if (isNull(parent)) {
			root = lChild;
		}
		else if (parent->rChild == node) {
			parent->rChild = lChild;
		}
		else parent->lChild = lChild;
		node->parent = lChild;
		lChild->rChild = node;
		return root;
	}
	RBT* insert_FixUp(RBT *root, RBT *node) {
		RBT *grandparent = NULL;	//�游�ڵ�
		RBT *parent = NULL;			//���ڵ�
		RBT *uncle = NULL;			//����ڵ�
									//����λ�ô�����Ӱ������µ��ϴ��ݿ�������Ҫ���е���
									//whileѭ��Ҳ����ʹ�õݹ�����⣺���£�����λ�ã����ϣ����ڵ㣩���ϵ�����ɫ����ת

		while (isRed(node)) {

			parent = node->parent;
			if (!isRed(parent)) break;//����ڵ��ýڵ�ĸ��׽ڵ�����Ǻ�ɫ�Ͳ��ã�������ڵ��ǿսڵ���ôҲ�˳���

			grandparent = parent->parent;
			if (parent == grandparent->lChild) {  //���ڵ����游�ڵ������
				uncle = grandparent->rChild;      //˵������ڵ����游�ڵ���Һ���
				if (isRed(uncle)) {
					/*
					��ǰ�ڵ�ĸ��ڵ��Ǻ�ɫ���ҵ�ǰ�ڵ���游�ڵ����һ���ӽڵ㣨����ڵ㣩Ҳ�Ǻ�ɫ��
					(01) �������ڵ㡱��Ϊ��ɫ��
					(02) ��������ڵ㡱��Ϊ��ɫ��
					(03) �����游�ڵ㡱��Ϊ����ɫ����
					(04) �����游�ڵ㡱��Ϊ����ǰ�ڵ㡱(��ɫ�ڵ�)������֮������ԡ���ǰ�ڵ㡱���в�����
					*/
					setColor(parent->c, BLACK);
					setColor(uncle->c, BLACK);
					setColor(grandparent->c, RED);
					node = grandparent;//�ڵ����Ƶ����游�ڵ�
				}
				else if (parent->rChild == node) {
					/*
					��ǰ�ڵ�ĸ��ڵ��Ǻ�ɫ������ڵ��Ǻ�ɫ���ҵ�ǰ�ڵ����丸�ڵ���Һ���
					(01) �������ڵ㡱��Ϊ���µĵ�ǰ�ڵ㡱��
					(02) �ԡ��µĵ�ǰ�ڵ㡱Ϊ֧�����������
					*/
					root = LRotate(root, parent);
					node = parent;//�ڵ����Ƶ��丸�ڵ��λ��
				}
				else if (parent->lChild == node) {
					/*
					��ǰ�ڵ�ĸ��ڵ��Ǻ�ɫ������ڵ��Ǻ�ɫ���ҵ�ǰ�ڵ����丸�ڵ������
					(01) �������ڵ㡱��Ϊ����ɫ����
					(02) �����游�ڵ㡱��Ϊ����ɫ����
					(03) �ԡ��游�ڵ㡱Ϊ֧�����������
					*/
					setColor(parent->c, BLACK);
					setColor(grandparent->c, RED);
					root = RRotate(root, grandparent);
				}
			}//if
			else {//��ǰ��һ��ֻ�����ҵ�������
				uncle = grandparent->lChild;
				if (isRed(uncle)) {
					setColor(parent->c, BLACK);
					setColor(uncle->c, BLACK);
					setColor(grandparent->c, RED);
					node = grandparent;
				}
				else if (parent->lChild == node) {
					root = RRotate(root, parent);
					node = parent;
				}
				else if (parent->rChild == node) {
					setColor(parent->c, BLACK);
					setColor(grandparent->c, RED);
					root = LRotate(root, grandparent);
				}
			}//else
		}//while:�����ǰ�ڵ��Ǻ�ɫ�Ͳ�����whileѭ����ʲôҲ��������

		root->c = BLACK;//���ڵ�Ⱦ�ɺ�ɫ
		return root;
	}
	RBT* RB_insert(RBT *root, T val) {
		RBT *nowNode = root;//��ǰ�ڵ㣺����Ѱ��Ҫ�����λ��
		RBT *newNode = NULL;//�½��ڵ���ΪNULL

		while (nowNode != NULL) {
			newNode = nowNode;
			if (nowNode->value < val) nowNode = nowNode->rChild;
			else nowNode = nowNode->lChild;
		}

		//RBT *Node = (RBT *)malloc(sizeof(struct RB_Tree));//Ҫ����Ľڵ�
		RBT *Node = new RBT;
		if (!Node) exit(0);
		Node->parent = newNode;//���ո��ҵ���λ�ýڵ���ΪҪ����ڵ�ĸ��ڵ�

		setNode(Node, newNode, val);//�ڵ㸳ֵ
		if (newNode == NULL) {//����½��ڵ�Ϊ��˵��������������ǿ���
			root = Node;      //��ô������Ľڵ���Ϊ���ڵ�
		}
		else if (Node->value < newNode->value) {//�½ڵ��ֵС�������ڵ��ֵ������Ϊ���ڵ������
			newNode->lChild = Node;
		}
		else if (Node->value > newNode->value)newNode->rChild = Node;//�����Һ���
		else return root;
		root = insert_FixUp(root, Node);
		curSize += 1;//�ߴ���һ
		return root;
	}
	RBT* Find_Successor(RBT *node) {//Ѱ�Һ�̽ڵ�
		while (!isNull(node->rChild))
			node = node->rChild;
		return node;
	}
	DIR getDirection(RBT *parent, RBT *node) {
		if (isNull(parent)) return ROOT;
		else if (parent->lChild == node) return LEFT;
		return RIGHT;
	}
	RBT *delete_FixUp(RBT *root, RBT *node, COLOR C, DIR dir) {
		if (isNull(node)) return root;//�սڵ�ֱ���˳�
		RBT *parent;
		RBT *brother;
		while (node != root && !isRed(node)) {//�����ǰ�ڵ㲻�Ǹ��ڵ����Ǻ�ɫ������ǰ�ڵ��Ǻ�ɫ�ڵ��Ҳ��Ǹ��ڵ�Ž���ѭ����
			parent = node->parent;
			if (parent->value > node->value || dir == LEFT) {//��Ϊ����ǰ��ɾ�������еĲ�������parent��node�ڵ����Ӷ�ʧ���Լ���dir�ж�node�ڵ������ӻ����Һ���
				dir = ROOT;
				brother = parent->rChild;
				if (isRed(brother)) {//�ֵܽڵ�Ϊ��ɫ
									 /*
									 (01) ��x���ֵܽڵ���Ϊ����ɫ����
									 (02) ��x�ĸ��ڵ���Ϊ����ɫ����
									 (03) ��x�ĸ��ڵ����������
									 (04) ��������������x���ֵܽڵ㡣
									 */
					setColor(brother->c, BLACK);
					setColor(parent->c, RED);
					root = LRotate(root, parent);
					brother = parent->rChild;

				}
				//�ϱ������������ǲ��й�ϵ��Ҫ��else if
				if (!isRed(brother->lChild) && !isRed(brother->rChild)) {//�ֵܽڵ��Ǻ�ɫ�������ӽڵ�Ҳ�Ǻ�ɫ
					setColor(brother->c, RED);
					node = parent;

				}
				else if (isRed(brother->lChild) && !isRed(brother->rChild)) {//����Һ�
																			 /*
																			 (01) ��x�ֵܽڵ��������Ϊ����ɫ����
																			 (02) ��x�ֵܽڵ���Ϊ����ɫ����
																			 (03) ��x���ֵܽڵ����������
																			 (04) ��������������x���ֵܽڵ㡣
																			 */
					setColor(brother->lChild->c, BLACK);
					setColor(brother->c, RED);
					root = RRotate(root, brother);
					brother = parent->rChild;

				}
				else {//�Һ�������
					  /*
					  (01) ��x���ڵ���ɫ ��ֵ�� x���ֵܽڵ㡣
					  (02) ��x���ڵ���Ϊ����ɫ����
					  (03) ��x�ֵܽڵ�����ӽ���Ϊ����ɫ����
					  (04) ��x�ĸ��ڵ����������
					  (05) ���á�x��Ϊ�����ڵ㡱��
					  */
					setColor(brother->c, parent->c);
					setColor(parent->c, BLACK);
					setColor(brother->rChild->c, BLACK);
					root = LRotate(root, parent);
					node = root;

				}
			}
			else {
				brother = parent->lChild;
				if (isRed(brother)) {//�ֵܽڵ�Ϊ��ɫ
					setColor(brother->c, BLACK);
					setColor(parent->c, RED);
					root = RRotate(root, parent);
					brother = parent->lChild;
				}
				if (!isRed(brother->rChild) && !isRed(brother->lChild)) {//�ֵܽڵ��Ǻ�ɫ�������ӽڵ�Ҳ�Ǻ�ɫ
					setColor(brother->c, RED);
					node = parent;
				}
				else if (isRed(brother->rChild) && !isRed(brother->lChild)) {//����Һ�
					setColor(brother->rChild->c, BLACK);
					setColor(brother->c, RED);
					root = LRotate(root, brother);
					brother = parent->lChild;
				}
				else {//�Һ�������
					setColor(brother->c, parent->c);
					setColor(parent->c, BLACK);
					setColor(brother->lChild->c, BLACK);
					root = RRotate(root, parent);
					node = root;
				}
			}
		}
		node->c = BLACK;
		return root;
	}
	RBT *RB_delete(RBT *root, T val) {//�˺���ֻ����ɾ������������
		COLOR C;//��ɾ���ڵ����ɫ
		RBT *D = NULL;//ָ������Ҫ���ͷŵ��Ľڵ�
		RBT *nowNode = root;
		RBT *newNode = nowNode;
		while (!isNull(nowNode) && nowNode->value != val) {//���ҵ�ɾ����λ��
			if (nowNode->value < val) nowNode = nowNode->rChild;
			else nowNode = nowNode->lChild;
			newNode = nowNode;
		}
		//û�ҵ�Ҫɾ�Ľڵ�ֱ���˳�
		if (isNull(nowNode)) return root;

		RBT *node = newNode;//nodeָ��Ҫɾ���Ľڵ�
		RBT *successor = NULL;//��̽ڵ㣨ָ����������������������ӽ���ɾ���ڵ����ݴ�С�Ľڵ㣩
		RBT *parent = NULL;//������ɾ���ڵ�ĸ��ڵ�
		DIR direction;//ɾ���Ľڵ����丸�ڵ�����ӻ����Һ��ӻ�ɾ���Ľڵ�Ϊ���ڵ�
		C = node->c;

		/*���"ɾ����������������ɾ���ڵ�ķ�����һ����"����3�������
		�� ��ɾ���ڵ�û�ж��ӣ���ΪҶ�ڵ㡣��ô��ֱ�ӽ��ýڵ�ɾ����OK�ˡ�
		�� ��ɾ���ڵ�ֻ��һ�����ӡ���ô��ֱ��ɾ���ýڵ㣬���øýڵ��Ψһ�ӽڵ㶥������λ�á�
		�� ��ɾ���ڵ����������ӡ���ô�����ҳ����ĺ�̽ڵ㣻
		Ȼ��ѡ����ĺ�̽ڵ�����ݡ����Ƹ����ýڵ�����ݡ���
		֮��ɾ�������ĺ�̽ڵ㡱��
		�������̽ڵ��൱�������ڽ���̽ڵ�����ݸ��Ƹ�"��ɾ���ڵ�"֮���ٽ���̽ڵ�ɾ����
		����������Ľ�����ת��Ϊ"ɾ����̽ڵ�"������ˣ�����Ϳ��Ǻ�̽ڵ㡣
		��"��ɾ���ڵ�"�������ǿ��ӽڵ������£����ĺ�̽ڵ㲻������˫�ӷǿա�
		��Ȼ"�ĺ�̽ڵ�"������˫�Ӷ��ǿգ�����ζ��"�ýڵ�ĺ�̽ڵ�"Ҫôû�ж��ӣ�Ҫôֻ��һ�����ӡ�
		��û�ж��ӣ���"����� "���д���
		��ֻ��һ�����ӣ���"����� "���д���(�����Դ�������ĸ��Ӷ�)
		*/
		if (isNull(node->lChild) && isNull(node->rChild)) {//(01)���Ҷ�Ϊ��ֱ��ɾ������
			if (node != root) {
				parent = node->parent;
				direction = getDirection(parent, node);
				if (direction == LEFT) parent->lChild = NULL;
				else parent->rChild = NULL;
			}
			else {//ֻ�и��ڵ�Ļ�ɾ����ֱ���˳�
				delete node;
				return NULL;
			}
			D = node;
		}
		else if (!isNull(node->lChild) && !isNull(node->rChild)) {\
			//(03)���Ҷ���Ϊ��
			//���ҵ���̽ڵ�
			//�������������ģ�������������С�ģ�����
			//ֵ��ע����������ҵ��ĺ�̽ڵ�һ�����ֻ��һ���ӽڵ㣬�����ӽڵ�ʱһ��������
			successor = Find_Successor(node->lChild);
			parent = successor->parent;
			direction = getDirection(parent, successor);
			//Ȼ��ֱ����successor�ڵ��滻��node�ڵ㣨ֻ�����ݣ���Ҫ���Ǹ����뱻ɾ���ڵ���ص��ӽڵ�͸��ڵ�
			node->value = successor->value;

			if (successor->parent != node) parent->rChild = successor->lChild;
			else parent->lChild = successor->lChild;

			if (!isNull(successor->lChild)) successor->lChild->parent = parent;

			C = successor->c;//��¼�±�ɾ���ڵ����ɫ�Ա���֮���ж��ǲ�����Ҫ����
			D = successor;//������ͷŵ�successor�ڵ�
		}
		else if (!isNull(node->lChild)) {
			//(02)һ��Ϊ��һ����Ϊ�գ���Ϊ�գ�
			parent = node;
			direction = LEFT;
			C = node->lChild->c;

			node->value = node->lChild->value;//�������ӵ�ֵ������'
			D = node->lChild;

			node->lChild = NULL;
		}
		else if (!isNull(node->rChild)) {//��Ϊ��
			parent = node;
			direction = RIGHT;
			C = node->rChild->c;

			node->value = node->rChild->value;//�����Һ��ӵ�ֵ������
			D = node->rChild;

			node->rChild = NULL;
		}
		if (C == BLACK) //ֻ��ɾ���Ľڵ��Ǻ�ɫ�Ҳ��Ǹ��ڵ����Ҫ������������������Ӱ����������,��Ϊɾ����ɫ�ڵ��Ӱ�����ʣ�5��
			root = delete_FixUp(root, D, C, direction);
		delete D;
		curSize -= 1;//�ߴ��һ
		return root;
	}
	bool search(RBT *root, T val) {
		RBT *node = root;
		while (node){
			if (node->value < val) {
				node = node->right;
			}
			else if (node->value > val) {
				node = node->left;
			}
			else return true;
		}
		return false;
	}
	void dfs_del(RBT * node) {
		if (node == NULL) return;
		dfs_del(node->lChild);//��������
		dfs_del(node->rChild);//��������
		delete node;
	}
	void dfs(RBT * node) {
		if (node == NULL) return;
		if (node->lChild == NULL&&node->rChild == NULL) {//���Ҷ�Ϊ��
			cout << node->value << ' ';
			return;
		}
		dfs(node->lChild);//��������
		cout << node->value << ' ';
		dfs(node->rChild);//��������
	}

public:
	friend prt;
	MySet() {
		root = NULL;
		curSize = 0;
	}
	~MySet() {
		dfs_del(root);
		root = NULL;
		curSize = 0;
	}
	RBT *getMinmum(RBT *node) {//��С�ڵ�
		if (node == NULL) return NULL;
		while (node->lchild) {
			node = node->lchild;
		}
		return node;
	}
	int size() {
		return curSize;
	}
	void insert(T val) {
		root = RB_insert(root, val);
	}
	void erase(T val) {
		root = RB_delete(root, val);
	}
	bool count(T val) {
		return search(root, val);
	}
	void travel() {
		cout << "����˳�����£�";
		dfs(root);
		cout << '\n';
	}
};
template<class T>
class prt {
public:
	friend MySet;
	MySet::RBT *it = MySet::root;
	it = MySet:: ;
	void operator ++(T)const {//��׺����
		if(it)it
	}
	
};