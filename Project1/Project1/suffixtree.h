#include<iostream>
#include<string>
#include<vector> 
#define mmax 100000
using namespace std;

vector<string> comp(string s1, string s2);

/*
规则一（活跃点为根节点时候的插入）：
o插入叶子之后，活跃节点依旧为根节点；
o活跃边更新为我们接下来要更新的后缀的首字母； 
o活跃半径减1;
规则二（后缀链表）：
o每个阶段，当我们建立新的内部节点并且不是该阶段第一次建立内部节点的时候，
 我们需要用指针从当前内部节点指向本阶段最近一次建立的内部节点。 
规则三（活跃点不为根节点时候的插入）：
o如果当前活跃点不是根节点，那么我们每次从活跃点新建一个叶子之后，
 就要沿着后缀链表到达新的节点，并更新活跃节点，如果不存在后缀链表，
 我们就转移到根节点，将活跃节点更新为根节点但活跃半径以及活跃边不变。 

  
*/ 


#ifndef  SUFFIXTREE_H
#define  SUFFIXTREE_H
void printspace(int m);
int compare(string s1, string s2);
#endif

struct Node;
struct Edge;




struct Node
{
	int flag;
	int count;//链接的边的个数，用下边的边指针数组存储 
	Edge *child[mmax];
	Edge *parent;
	Node *next;//后缀链接标识 
	Node(){flag=-1;parent=NULL;count=0;next=NULL;}
	Node(int f){flag=f;parent=NULL;count=0;next=NULL;}
};
struct Edge
{
	string str;
	Node *above,*below;//head-->above   back--->below 
	Edge(){str="";above=NULL;below=NULL;}
	Edge(Node *above,Node *below,string str)
	{
		this->str=str;
		this->above=above;
	    this->below=below;
		this->above->child[above->count++]=this;
		this->below->parent=this;
	}
	Edge(Node *above,int i,Node *below,string str)
	{
		this->str=str;
		this->above=above;
		this->below=below;
		this->above->child[i]=this;
		this->below->parent=this;
	}
};

struct ActivePoint
{
	Node *node;//活动节点 
	Edge *edge;//活动边 
	int length;//活动长度 
	ActivePoint(){node=NULL;edge=NULL;length=0;}
	ActivePoint(Node*n,Edge*e,int len){node=n;edge=e;length=len;}
};
class SuffixTree
{
	public:
	SuffixTree()
	{
		root = new Node();
		activepoint = new ActivePoint(root, NULL, 0);
		reminder = 0;
		helpstr = "";
		suffixarray = "";
		active = NULL;
	}
	~SuffixTree() { delall(root); } //析构函数 
		//void delall(Node *p);//实际释放空间函数，释放节点p的所有孩子 (从后往前)
		//
		//int getlength(Node *p);//从p节点向上到根节点经历过的边的字符个数 
		//string getstr(Node *node);//从根节点向下到p节点拼出字符串 
		string getallstr(){return helpstr;}//返回该树的字符串 
		//
		//bool search(Node *p,string str,Node *&cur);//从p节点向下寻找与字符串str匹配的，找到返回true 
		//bool findstr(string str);//查找字符串是否存在 
		//string findlongeststr();//寻找最长重复字符串 
		//void finddeepestr(Node *a[],Node *p,int &cal);//寻找每个分支的最长重复字符串 
		//
		//int count(string str);//计算字符串str出现的次数 
		//int countleaf(Node *p);//计算p节点下的叶节点个数 
		//bool judgeleaf(Node *p);//判断p节点先是否全为叶节点 
		//
		//int find(char x);//查找指定的后缀是否存在 
		//void build(string str);//构建后缀树 
		//void deal(string str,int currentindex);//处理后缀函数 
		//
		//void showtree(){show(root,0,0);}//打印后缀树 
		//void show(Node *p,int repeat,int len);//打印后缀树实际函数 
		/****************************************************************************
		**delall(Node *p)方法：
		**释放节点p的所有孩子
		****************************************************************************/

		void delall(Node* p)
		{
			for (int i = p->count - 1; i >= 0; i--)
			{
				if (p->child[i]->below->flag >= 0)
				{
					//cout<<"释放边"<<p->child[i]->str<<"及*节点的第"<<i+1<<"个孩子节点"<<"\n"; 
					delete p->child[i]->below;
					delete p->child[i];
					p->count--;
				}
				else
					delall(p->child[i]->below);
			}
			if (p != root)
			{
				Node* q = p->parent->above;
				//cout<<"释放边"<<q->child[q->count-1]->str<<"及*节点的第"<<q->count<<"个孩子"<<"\n"; 
				delete q->child[q->count - 1]->below;
				delete q->child[q->count - 1];
				q->count--;
				//del(q,q->count-1);
			}
			else
			{
				delete p;
				//cout<<"释放根节点"<<endl;
			}
		}
		
		/****************************************************************************
		**getlength(Node *p)方法：
		**返回从p节点到根节点所经边上字符串拼接而成的字符串的长度
		****************************************************************************/
		int getlength(Node* p)
		{
			int len = 0;
			if (p == root)
				return len;
			len = p->parent->str.length();
			if (p->parent->above != root)
				return len + getlength(p->parent->above);
			else
				return len;
		}
		/****************************************************************************
		**getst(Node *p)方法：
		**拼接从根节点到p节点的所经边的字符串并返回
		****************************************************************************/
		string getstr(Node* node)
		{
			string str;
			if (node == root)
				return "";
			Edge* edge = node->parent;
			while (node != root)
			{
				edge = node->parent;
				str = edge->str + str;
				node = edge->above;
			}
			return str;
		}
		/****************************************************************************
		**search(Node *p,string str,Node *&cur)方法：
		**在节点p的孩子中搜寻与字符串str匹配的，如果是完全匹配的，就将
		**此节点的地址传给cur
		****************************************************************************/
		bool search(Node* p, string str, Node*& cur)
		{
			bool signal;
			for (int i = 0; i < p->count; i++)
			{
				int s = compare(str, p->child[i]->str);
				if (s == 0)
				{
					cur = p->child[i]->below;
					return true;
				}
				else if (s == -1)
					signal = false;
				else
				{
					str.erase(0, s);
					signal = search(p->child[i]->below, str, cur);
					break;
				}
			}
			return signal;

		}

		/****************************************************************************
		**findstr(string str)方法：
		**查找字符串str是否存在
		**存在返回true
		****************************************************************************/
		bool findstr(string str)
		{
			Node* cur = NULL;
			return search(root, str, cur);
		}
		/****************************************************************************
		**findlongest()方法：
		**计算后缀树中最长重复字符串并返回该字符串
		****************************************************************************/
		string findlongeststr()
		{
			string s = "";
			int len = 0, cal = 0;
			Node* a[mmax];
			finddeepestr(a, root, cal);//储存每个分支的最深内部节点 
			for (int i = 0; i < cal; i++)//开始比较每个最深内部节点所拼出的字符长度，返回长度大的 
			{
				Node* t = a[i];
				Node* p = t;
				for (int i = 0; i < p->count; i++)
				{
					string str = p->child[i]->str;
					string temp = getstr(t);
					
					if (str.find("#")!=-1&& (int)(str.find(temp))> (int)(str.find("#")))
					{
						int templen = getlength(t);
						if (templen > len != -1)
						{
							len = templen;
							s = getstr(t);
							break;
						}
					}
				}

			}
			return s;

		}
		/****************************************************************************
		**findeepest(Node *a[],Node *p,int &cal)方法：
		**记录p节点下每个分支的最深重复字符串，并将其记录在数组a中 ，用引用变量cal来计数
		****************************************************************************/
		void finddeepestr(Node* a[], Node* p, int& cal)
		{
			for (int i = 0; i < p->count; i++)
			{
				Node* t = p->child[i]->below;
				if (t->flag >= 0)
					continue;
				else if (judgeleaf(t))
				{
					a[cal++] = t;
				}
				else
					finddeepestr(a, t, cal);
			}

		}
		/****************************************************************************
		**count(string str)方法：
		**计算后缀树中str出现的次数
		****************************************************************************/
		int count(string str)
		{
			Node* cur = NULL;
			bool signal = search(root, str, cur);//判断是否找到，若找到，则将地址返回给cur 
			if (signal == false)
				return 0;
			else//找到，则利用countleaf计算cur指向节点的叶子数 
			{
				if (cur->flag >= 0)
					return 1;
				return countleaf(cur);
			}

		}
		/****************************************************************************
		**countleaf(Node *p)方法：
		**计算节点p的孩子中叶节点个数
		****************************************************************************/
		int countleaf(Node* p)
		{
			int counts = 0;
			if (p->flag >= 0)//如果当前节点是叶节点，直接返回0 
				return 0;
			else
			{
				for (int i = 0; i < p->count; i++)//依次判断节点下的节点是否为叶节点，是则令counts加一，否则递归进入 
				{
					if (p->child[i]->below->flag >= 0)
						counts++;
					else
						counts += countleaf(p->child[i]->below);
				}
			}
			return counts;
		}
		/****************************************************************************
		**judgeleaf(Node *p)方法：
		**判断p节点的孩子是否全为叶节点，是则返回true，否则返回false
		****************************************************************************/
		bool judgeleaf(Node* p)
		{
			for (int i = 0; i < p->count; i++)
			{
				Node* t = p->child[i]->below;
				if (t->flag < 0)// 如果当前节点不是叶节点 
					return false;
			}
			return true;
		}

		/****************************************************************************
		**int find(char x)方法：
		**查找当前后缀是否存在，不存在返回-1
		****************************************************************************/
		int find(char x)
		{
			Edge*& apedge = activepoint->edge;
			Node*& apnode = activepoint->node;
			int& aplen = activepoint->length;
			if (apedge == NULL)
			{//无活动边，则从活动节点的子节点开始查找
				for (int i = 0; i < apnode->count; i++)
				{
					//cout<<i;
					Edge* tempedge = apnode->child[i];
					if (tempedge->str[0] == x)
					{
						aplen++;
						apedge = apnode->child[i];
						if (aplen == apedge->str.length())
						{//这里出现了一个修改活动点的规则：即如果活动边上的所有字符全部都被匹配完了
						 //(级活动边上的字符数==活动长度)，则将活动边晋升为活动点，同时重置活动长度为0。
						 //所以下次查找时就得从该节点开始了，而不是根节点了。

							apnode = apedge->below;
							aplen = 0;
							apedge = NULL;
							//return 1;
						}
						return i;
					}

				}
				return -1;
			}
			else
			{// 有活动边，则在活动边上查找
				if (apedge->str[aplen] == x)
				{
					aplen++;
					if (aplen == apedge->str.length())
					{//这里出现了一个修改活动点的规则：即如果活动边上的所有字符全部都被匹配完了
					 //(级活动边上的字符数==活动长度)，则将活动边晋升为活动点，同时重置活动长度为0。
					 //所以下次查找时就得从该节点开始了，而不是根节点了。

						apnode = apedge->below;
						aplen = 0;
						apedge = NULL;
					}
					return 1;
				}
				else
					return -1;

			}
			return -1;
		}
		/****************************************************************************
		**build(string str)方法：
		**以str构造后缀树
		****************************************************************************/
		void build(string str)
		{
			helpstr = str;
			int index = 0;
			Edge*& apedge = activepoint->edge;
			Node*& apnode = activepoint->node;
			int& aplen = activepoint->length;

			while (index < str.length())
			{
				//cout<<"\n当前处理的: "<<index<<","<<str[index]<<"\n";
				//test();cout<<"\n" ;
				int currentindex = index++;
				char w = str[currentindex];

				//查找是否存在保存有当前后缀字符的节点 
				if (find(w) != -1)//如果找到了 
				{
					suffixarray += w;
					reminder++;
					continue;
				}
				else //如果未找到 
				{
					suffixarray += w;
					reminder++;
				}
				active = NULL;
				deal(str, currentindex);

			}
		}

		/****************************************************************************
		**deal(string str,int currentindex,int number)方法：
		**处理后缀字符，str是输入的字符，currentindex是处理到的位置，number表示本次操作
		**使用了几次后缀链表
		****************************************************************************/
		void deal(string str, int currentindex)
		{
			//cout<<"\n----------------------------------------------\n";
			//cout<<"deal函数入口：\n";
		   // test();show(root,0,0);


			Edge*& apedge = activepoint->edge;
			Node*& apnode = activepoint->node;
			int& aplen = activepoint->length;
			if (reminder == 1)//如果剩余后缀数为1 //pay attention to//是否一定为根，当reminder为1的时候 
			{
				if (apnode == root)//如果活动节点是根节点 
				{//新建节点 
					Node* tempnode1 = new Node(currentindex - suffixarray.length() + 1);
					Edge* tempedge1 = new Edge(apnode, tempnode1, str.substr(currentindex));
					suffixarray.erase(0, 1);
					reminder--;
					apedge = NULL;

					//cout<<"deal函数出口：\n";
					//test();show(root,0,0);
					//cout<<"\n----------------------------------------------\n";
					return;
				}
				else//如果活动节点不是根节点,apnode!=root 
				{

				}
			}
			else//剩余后缀数大于1
			{
				if (apnode == root)
				{
					//规则一（活跃点为根节点时候的插入）：
					//o插入叶子之后，活跃节点依旧为根节点；
					//o活跃边更新为我们接下来要更新的后缀的首字母； 
					//o活跃半径减1;
					if (apedge == NULL)//如果活动边不存在，即说明活动节点下需要新创建节点
					{
						Node* tempnode1 = new Node(currentindex);
						Edge* tempedge1 = new Edge(apnode, tempnode1, str.substr(currentindex));
						//活动边依旧设置为空 		
					}
					else
					{
						Edge* edge = apedge;//保存当前活动边，也便于后边释放旧有的活动边 
						apedge = NULL;
						aplen--;//因为一定能找到，因此寻找过程中会使得aplen++，此处修正
						int m = find(edge->str[0]);//寻找标号，后边新建节点会用到 


						Node* tempnode1 = new Node();
						Edge* tempedge1 = new Edge(tempnode1, apedge->below, apedge->str.substr(aplen));
						Edge* tempedge2 = new Edge(apnode, m, tempnode1, apedge->str.substr(0, aplen));

						Node* tempnode2 = new Node(currentindex - suffixarray.length() + 1);
						Edge* tempedge3 = new Edge(tempnode1, tempnode2, str.substr(currentindex));

						apedge = apnode->child[m];
						delete edge;//释放旧有的活动边 




					}



					//规则二（后缀链表）：
					//o每个阶段，当我们建立新的内部节点并且不是该阶段第一次建立内部节点的时候，
					//我们需要用指针从当前内部节点指向本阶段最近一次建立的内部节点。 

					//如果当前新建节点是内部节点，则更新后缀链表 
					if (apedge != NULL && apedge->below->count > 1)
					{
						if (active == NULL)
							active = apedge->below;
						else
						{
							active->next = apedge->below;
							active = apedge->below;
						}
					}
					else if (apedge == NULL)
					{
						if (active == NULL)
							active = apnode;
						else
						{
							active->next = apnode;
							active = apnode;
						}
					}



					suffixarray.erase(0, 1);
					reminder--;
					aplen--;


					apedge = NULL;//apnode已经为空 
					aplen = 0;
					int flag;
					for (int i = 0; i < reminder; i++)
					{
						flag = find(suffixarray[i]);
					}


					if (flag == -1)
					{
						//cout<<"deal函数出口：\n";
						//test();show(root,0,0);
						//cout<<"\n----------------------------------------------\n";
						deal(str, currentindex);
						return;
					}
					else
					{
						//cout<<"deal函数出口：\n";
						//test();show(root,0,0);
						//cout<<"\n----------------------------------------------\n";
						return;
					}



				}
				else//apnode!=root
				{
					//规则三（活跃点不为根节点时候的插入）：
					//o如果当前活跃点不是根节点，那么我们每次从活跃点新建一个叶子之后，
					//就要沿着后缀链表到达新的节点，并更新活跃节点，如果不存在后缀链表，
					//我们就转移到根节点，将活跃节点更新为根节点但活跃半径以及活跃边不变。

					char temp;
					if (apedge == NULL)//如果活动边不存在，即说明活动节点下需要新创建节点
					{
						Node* tempnode1 = new Node(currentindex - suffixarray.length() + 1);
						Edge* tempedge1 = new Edge(apnode, tempnode1, str.substr(currentindex));
						//这个时候活动节点怎么定义？？？？ //依旧当做新建的内部节点处理 	
					}
					else
					{


						Edge* edge = apedge;
						temp = edge->str[0];
						apedge = NULL;
						aplen--;
						int m = find(edge->str[0]);


						Node* tempnode1 = new Node();
						//cout<<"what happened?\n";//这里曾经出现一个问题就是前面的"aplen--"与"int m=find(edge->str[0])"顺序错误而产生的问题 
						//cout<<apedge->str<<" "<<aplen;//当顺序反后，活动点可能会错误的升级，而这不是我想要的 
						//cout<<apedge->str.substr(aplen)<<"\n";
						Edge* tempedge1 = new Edge(tempnode1, apedge->below, apedge->str.substr(aplen));
						Edge* tempedge2 = new Edge(apnode, m, tempnode1, apedge->str.substr(0, aplen));

						Node* tempnode2 = new Node(currentindex - suffixarray.length() + 1);
						Edge* tempedge3 = new Edge(tempnode1, tempnode2, str.substr(currentindex));

						apedge = apnode->child[m];
						delete edge;
					}
					reminder--;
					suffixarray.erase(0, 1);

					//如果当前新建节点是内部节点，则更新后缀链表 
					if (apedge != NULL && apedge->below->count > 1)
					{
						if (active == NULL)//注意加判定以判断是否为内部节点！！！ 
							active = apedge->below;
						else
						{
							active->next = apedge->below;
							active = apedge->below;
						}
					}
					else
					{
						if (active == NULL)//注意加判定以判断是否为内部节点！！！ 
							active = apnode;
						else
						{
							active->next = apnode;
							active = apnode;
						}

					}


					//开始沿着后缀链表寻找,并且重置活动点 
					if (apnode->next != NULL)//如果有连接，就进入 
					{

						apnode = apnode->next;
						apedge = NULL;
						int tempaplen = aplen;
						aplen = 0;


						int flag;
						for (int i = reminder - tempaplen - 1; i < reminder; i++)
						{
							flag = find(suffixarray[i]);
						}



						if (flag == -1)
						{
							//cout<<"deal函数出口：\n";
							//test();show(root,0,0);
							//cout<<"\n----------------------------------------------\n";
							deal(str, currentindex);
							return;
						}
						else
						{
							//cout<<"deal函数出口：\n";
							//test();show(root,0,0);
							//cout<<"\n----------------------------------------------\n";
							return;
						}


					}
					else//如果当前节点无连接，就置活动节点为根节点 
					{
						apnode = root;
						apedge = NULL;
						aplen = 0;
						int flag;
						for (int i = 0; i < reminder; i++)
						{
							flag = find(suffixarray[i]);
						}
						if (flag == -1)
						{
							//cout<<"deal函数出口：\n";
							//test();show(root,0,0);
							//cout<<"\n----------------------------------------------\n";			        	
							deal(str, currentindex);
							return;
						}
						else
						{
							//cout<<"deal函数出口：\n";
						   //test();show(root,0,0);
							//cout<<"\n----------------------------------------------\n";			        	
							return;
						}


					}
				}//apnode!=root终结 

			}//reminder>1终结 

		}
		/****************************************************************************
		**show(Node *p,int repeat,int len)方法：
		**打印p节点之下的后缀树
		**repeat表示进入的深度，len表示距离当前标准的距离
		****************************************************************************/
		void show(Node* p, int repeat, int len)
		{
			int templen = 0;//int judge=0;
			for (int i = 0; i < p->count; i++)
			{

				if (repeat != 0)
				{
					cout << "\n"; printspace(len); cout << "|";
				}
				else
				{

				}

				Edge* edge = p->child[i];
				if (p == root)
				{
					cout << "\n*--" << edge->str << "--";
					if (edge->below->flag == -1)
						cout << "*";
					else
						cout << edge->below->flag;
					templen = 3 + edge->str.length() + 2;
				}
				else
				{
					cout << "\n";
					printspace(len);
					cout << "*--" << edge->str << "--";
					if (edge->below->flag == -1)
						cout << "*";
					else
						cout << edge->below->flag;
					templen = 3 + edge->str.length() + 2;
				}
				/*if(judge==0&&edge->below->next!=NULL)
				{
					cout<<"~~~~"<<getstr(edge->below->next)<<"~~~~~~*";
					judge=1;
				} *///现在不懂judge的作用 
				if (edge->below->next != NULL)
				{
					cout << "~~~~" << getstr(edge->below->next) << "~~~~~~*";
				}
				if (edge->below->count == 0)//如果与边相关联的节点没有其他相连的边时 
				{

				}
				else//如果与边相关联的节点有子边 
				{
					show(edge->below, ++repeat, templen + len);
				}
			}
			cout << "\n";
		}

		void showtree() { show(root, 0, 0); }

		void test()//测试用函数，展示当前活动点，后缀字符，剩余后缀数等信息 
		{
			this->showtree();
			if(activepoint->edge!=NULL)
			{
				cout<<"\n apnode="<<getstr(activepoint->node)<<",apedge="<<activepoint->edge->str<<",aplen="<<activepoint->length;
		    	cout<<",reminder="<<reminder<<",suffixarray="<<suffixarray<<"\n";
			}
			else
			{
				cout<<"\n apnode="<<getstr(activepoint->node)<<",apedge=NULL,aplen="<<activepoint->length;
		    	cout<<",reminder="<<reminder<<",suffixarray="<<suffixarray<<"\n";
			}
		}


	private:
		Node *root;
		ActivePoint *activepoint;
		int reminder;
		string suffixarray;	
		Node *active;
		string helpstr;	
		
		




		
};
