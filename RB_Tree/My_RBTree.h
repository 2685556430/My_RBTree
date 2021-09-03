#pragma once
typedef bool COLOR;//便于表示节点颜色
typedef int DIR;
#define NULL 0
#define LEFT -1
#define RIGHT 1
#define ROOT 0
#define RED 0
#define BLACK 1
#define isRed(x) (x != NULL && x->c == RED) //空节点默认是黑节点（判断是否为红色节点）
#define isNull(x) (x == NULL)				//看看是不是空节点
#define setColor(x, COL) (x = COL)		//染色
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
		setColor(node->c, RED);//新节点着色为红色
		node->lChild = NULL;
		node->rChild = NULL;
		node->parent = p;
		node->value = val;
	}
	RBT *LRotate(RBT *root, RBT *node) {//以node为中心左旋
		RBT *parent = node->parent;
		RBT *rChild = node->rChild;
		node->rChild = rChild->lChild;
		if (!isNull(rChild->lChild)) {//如果右孩子的左孩子不是空节点再将其父节点更改
			rChild->lChild->parent = node;
		}

		rChild->parent = parent;//将 parent 作为 rChild 新的的父节点
		if (isNull(parent)) {//如果parent是空说明当前节点只有两层
			root = rChild;
		}
		else if (parent->lChild == node) {//分两种情况parent节点的左孩子的左旋 和 右孩子的左旋
			parent->lChild = rChild;
		}
		else parent->rChild = rChild;

		node->parent = rChild;
		rChild->lChild = node;
		return root;
	}

	RBT* RRotate(RBT *root, RBT *node) {//以node为中心左旋 情况类似只要交换 左 右 即可
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
		RBT *grandparent = NULL;	//祖父节点
		RBT *parent = NULL;			//父节点
		RBT *uncle = NULL;			//叔叔节点
									//插入位置带来的影响会由下到上传递开来所以要进行调整
									//while循环也可以使用递归来理解：由下（插入位置）向上（根节点）不断调整颜色和旋转

		while (isRed(node)) {

			parent = node->parent;
			if (!isRed(parent)) break;//插入节点后该节点的父亲节点如果是黑色就不用（如果父节点是空节点那么也退出）

			grandparent = parent->parent;
			if (parent == grandparent->lChild) {  //父节点是祖父节点的左孩子
				uncle = grandparent->rChild;      //说明叔叔节点是祖父节点的右孩子
				if (isRed(uncle)) {
					/*
					当前节点的父节点是红色，且当前节点的祖父节点的另一个子节点（叔叔节点）也是红色。
					(01) 将“父节点”设为黑色。
					(02) 将“叔叔节点”设为黑色。
					(03) 将“祖父节点”设为“红色”。
					(04) 将“祖父节点”设为“当前节点”(红色节点)；即，之后继续对“当前节点”进行操作。
					*/
					setColor(parent->c, BLACK);
					setColor(uncle->c, BLACK);
					setColor(grandparent->c, RED);
					node = grandparent;//节点上移到其祖父节点
				}
				else if (parent->rChild == node) {
					/*
					当前节点的父节点是红色，叔叔节点是黑色，且当前节点是其父节点的右孩子
					(01) 将“父节点”作为“新的当前节点”。
					(02) 以“新的当前节点”为支点进行左旋。
					*/
					root = LRotate(root, parent);
					node = parent;//节点上移到其父节点的位置
				}
				else if (parent->lChild == node) {
					/*
					当前节点的父节点是红色，叔叔节点是黑色，且当前节点是其父节点的左孩子
					(01) 将“父节点”设为“黑色”。
					(02) 将“祖父节点”设为“红色”。
					(03) 以“祖父节点”为支点进行右旋。
					*/
					setColor(parent->c, BLACK);
					setColor(grandparent->c, RED);
					root = RRotate(root, grandparent);
				}
			}//if
			else {//和前面一样只是左右调换即可
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
		}//while:如果当前节点是黑色就不进入while循环即什么也不用做。

		root->c = BLACK;//根节点染成黑色
		return root;
	}
	RBT* RB_insert(RBT *root, T val) {
		RBT *nowNode = root;//当前节点：用于寻找要插入的位置
		RBT *newNode = NULL;//新建节点设为NULL

		while (nowNode != NULL) {
			newNode = nowNode;
			if (nowNode->value < val) nowNode = nowNode->rChild;
			else nowNode = nowNode->lChild;
		}

		//RBT *Node = (RBT *)malloc(sizeof(struct RB_Tree));//要插入的节点
		RBT *Node = new RBT;
		if (!Node) exit(0);
		Node->parent = newNode;//将刚刚找到的位置节点作为要插入节点的父节点

		setNode(Node, newNode, val);//节点赋值
		if (newNode == NULL) {//如果新建节点为空说明这棵树本来就是空树
			root = Node;      //那么将插入的节点作为根节点
		}
		else if (Node->value < newNode->value) {//新节点的值小于它父节点的值将它作为父节点的左孩子
			newNode->lChild = Node;
		}
		else if (Node->value > newNode->value)newNode->rChild = Node;//否则右孩子
		else return root;
		root = insert_FixUp(root, Node);
		curSize += 1;//尺寸增一
		return root;
	}
	RBT* Find_Successor(RBT *node) {//寻找后继节点
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
		if (isNull(node)) return root;//空节点直接退出
		RBT *parent;
		RBT *brother;
		while (node != root && !isRed(node)) {//如果当前节点不是根节点且是黑色（即当前节点是黑色节点且不是根节点才进入循环）
			parent = node->parent;
			if (parent->value > node->value || dir == LEFT) {//因为我先前的删除函数中的操作导致parent与node节点链接丢失所以加入dir判断node节点是左孩子还是右孩子
				dir = ROOT;
				brother = parent->rChild;
				if (isRed(brother)) {//兄弟节点为红色
									 /*
									 (01) 将x的兄弟节点设为“黑色”。
									 (02) 将x的父节点设为“红色”。
									 (03) 对x的父节点进行左旋。
									 (04) 左旋后，重新设置x的兄弟节点。
									 */
					setColor(brother->c, BLACK);
					setColor(parent->c, RED);
					root = LRotate(root, parent);
					brother = parent->rChild;

				}
				//上边条件和下面是并列关系不要用else if
				if (!isRed(brother->lChild) && !isRed(brother->rChild)) {//兄弟节点是黑色且它的子节点也是黑色
					setColor(brother->c, RED);
					node = parent;

				}
				else if (isRed(brother->lChild) && !isRed(brother->rChild)) {//左红右黑
																			 /*
																			 (01) 将x兄弟节点的左孩子设为“黑色”。
																			 (02) 将x兄弟节点设为“红色”。
																			 (03) 对x的兄弟节点进行右旋。
																			 (04) 右旋后，重新设置x的兄弟节点。
																			 */
					setColor(brother->lChild->c, BLACK);
					setColor(brother->c, RED);
					root = RRotate(root, brother);
					brother = parent->rChild;

				}
				else {//右红左任意
					  /*
					  (01) 将x父节点颜色 赋值给 x的兄弟节点。
					  (02) 将x父节点设为“黑色”。
					  (03) 将x兄弟节点的右子节设为“黑色”。
					  (04) 对x的父节点进行左旋。
					  (05) 设置“x”为“根节点”。
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
				if (isRed(brother)) {//兄弟节点为红色
					setColor(brother->c, BLACK);
					setColor(parent->c, RED);
					root = RRotate(root, parent);
					brother = parent->lChild;
				}
				if (!isRed(brother->rChild) && !isRed(brother->lChild)) {//兄弟节点是黑色且它的子节点也是黑色
					setColor(brother->c, RED);
					node = parent;
				}
				else if (isRed(brother->rChild) && !isRed(brother->lChild)) {//左红右黑
					setColor(brother->rChild->c, BLACK);
					setColor(brother->c, RED);
					root = LRotate(root, brother);
					brother = parent->lChild;
				}
				else {//右红左任意
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
	RBT *RB_delete(RBT *root, T val) {//此函数只用于删除不用于修正
		COLOR C;//被删除节点的颜色
		RBT *D = NULL;//指向最终要被释放掉的节点
		RBT *nowNode = root;
		RBT *newNode = nowNode;
		while (!isNull(nowNode) && nowNode->value != val) {//先找到删除的位置
			if (nowNode->value < val) nowNode = nowNode->rChild;
			else nowNode = nowNode->lChild;
			newNode = nowNode;
		}
		//没找到要删的节点直接退出
		if (isNull(nowNode)) return root;

		RBT *node = newNode;//node指向要删除的节点
		RBT *successor = NULL;//后继节点（指的是左子树或者右子树最接近被删除节点数据大小的节点）
		RBT *parent = NULL;//真正被删除节点的父节点
		DIR direction;//删除的节点是其父节点的左孩子还是右孩子或删除的节点为根节点
		C = node->c;

		/*这和"删除常规二叉查找树中删除节点的方法是一样的"。分3种情况：
		① 被删除节点没有儿子，即为叶节点。那么，直接将该节点删除就OK了。
		② 被删除节点只有一个儿子。那么，直接删除该节点，并用该节点的唯一子节点顶替它的位置。
		③ 被删除节点有两个儿子。那么，先找出它的后继节点；
		然后把“它的后继节点的内容”复制给“该节点的内容”；
		之后，删除“它的后继节点”。
		在这里，后继节点相当于替身，在将后继节点的内容复制给"被删除节点"之后，再将后继节点删除。
		这样就巧妙的将问题转换为"删除后继节点"的情况了，下面就考虑后继节点。
		在"被删除节点"有两个非空子节点的情况下，它的后继节点不可能是双子非空。
		既然"的后继节点"不可能双子都非空，就意味着"该节点的后继节点"要么没有儿子，要么只有一个儿子。
		若没有儿子，则按"情况① "进行处理；
		若只有一个儿子，则按"情况② "进行处理。(这点可以大大简化问题的复杂度)
		*/
		if (isNull(node->lChild) && isNull(node->rChild)) {//(01)左右都为空直接删除即可
			if (node != root) {
				parent = node->parent;
				direction = getDirection(parent, node);
				if (direction == LEFT) parent->lChild = NULL;
				else parent->rChild = NULL;
			}
			else {//只有根节点的话删除后直接退出
				delete node;
				return NULL;
			}
			D = node;
		}
		else if (!isNull(node->lChild) && !isNull(node->rChild)) {\
			//(03)左右都不为空
			//先找到后继节点
			//找左子树中最大的（或者右子树最小的）数据
			//值得注意的是这样找到的后继节点一定最多只有一个子节点，且有子节点时一定是左孩子
			successor = Find_Successor(node->lChild);
			parent = successor->parent;
			direction = getDirection(parent, successor);
			//然后直接用successor节点替换掉node节点（只换数据）不要忘记更改与被删掉节点相关的子节点和父节点
			node->value = successor->value;

			if (successor->parent != node) parent->rChild = successor->lChild;
			else parent->lChild = successor->lChild;

			if (!isNull(successor->lChild)) successor->lChild->parent = parent;

			C = successor->c;//记录下被删掉节点的颜色以便于之后判断是不是需要修正
			D = successor;//最后再释放掉successor节点
		}
		else if (!isNull(node->lChild)) {
			//(02)一个为空一个不为空（右为空）
			parent = node;
			direction = LEFT;
			C = node->lChild->c;

			node->value = node->lChild->value;//将其左孩子的值赋给它'
			D = node->lChild;

			node->lChild = NULL;
		}
		else if (!isNull(node->rChild)) {//左为空
			parent = node;
			direction = RIGHT;
			C = node->rChild->c;

			node->value = node->rChild->value;//将其右孩子的值赋给它
			D = node->rChild;

			node->rChild = NULL;
		}
		if (C == BLACK) //只有删除的节点是黑色且不是根节点才需要修正红黑树其他情况不影响红黑树性质,因为删除红色节点会影响性质（5）
			root = delete_FixUp(root, D, C, direction);
		delete D;
		curSize -= 1;//尺寸减一
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
		dfs_del(node->lChild);//遍历左树
		dfs_del(node->rChild);//遍历右树
		delete node;
	}
	void dfs(RBT * node) {
		if (node == NULL) return;
		if (node->lChild == NULL&&node->rChild == NULL) {//左右都为空
			cout << node->value << ' ';
			return;
		}
		dfs(node->lChild);//遍历左树
		cout << node->value << ' ';
		dfs(node->rChild);//遍历右树
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
	RBT *getMinmum(RBT *node) {//最小节点
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
		cout << "遍历顺序如下：";
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
	void operator ++(T)const {//后缀自增
		if(it)it
	}
	
};