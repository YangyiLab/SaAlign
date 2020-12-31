#include<iostream>
#include<string>
#include<vector> 
#define mmax 100000
using namespace std;

vector<string> comp(string s1, string s2);

/*
����һ����Ծ��Ϊ���ڵ�ʱ��Ĳ��룩��
o����Ҷ��֮�󣬻�Ծ�ڵ�����Ϊ���ڵ㣻
o��Ծ�߸���Ϊ���ǽ�����Ҫ���µĺ�׺������ĸ�� 
o��Ծ�뾶��1;
���������׺������
oÿ���׶Σ������ǽ����µ��ڲ��ڵ㲢�Ҳ��Ǹý׶ε�һ�ν����ڲ��ڵ��ʱ��
 ������Ҫ��ָ��ӵ�ǰ�ڲ��ڵ�ָ�򱾽׶����һ�ν������ڲ��ڵ㡣 
����������Ծ�㲻Ϊ���ڵ�ʱ��Ĳ��룩��
o�����ǰ��Ծ�㲻�Ǹ��ڵ㣬��ô����ÿ�δӻ�Ծ���½�һ��Ҷ��֮��
 ��Ҫ���ź�׺�������µĽڵ㣬�����»�Ծ�ڵ㣬��������ں�׺����
 ���Ǿ�ת�Ƶ����ڵ㣬����Ծ�ڵ����Ϊ���ڵ㵫��Ծ�뾶�Լ���Ծ�߲��䡣 

  
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
	int count;//���ӵıߵĸ��������±ߵı�ָ������洢 
	Edge *child[mmax];
	Edge *parent;
	Node *next;//��׺���ӱ�ʶ 
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
	Node *node;//��ڵ� 
	Edge *edge;//��� 
	int length;//����� 
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
	~SuffixTree() { delall(root); } //�������� 
		//void delall(Node *p);//ʵ���ͷſռ亯�����ͷŽڵ�p�����к��� (�Ӻ���ǰ)
		//
		//int getlength(Node *p);//��p�ڵ����ϵ����ڵ㾭�����ıߵ��ַ����� 
		//string getstr(Node *node);//�Ӹ��ڵ����µ�p�ڵ�ƴ���ַ��� 
		string getallstr(){return helpstr;}//���ظ������ַ��� 
		//
		//bool search(Node *p,string str,Node *&cur);//��p�ڵ�����Ѱ�����ַ���strƥ��ģ��ҵ�����true 
		//bool findstr(string str);//�����ַ����Ƿ���� 
		//string findlongeststr();//Ѱ����ظ��ַ��� 
		//void finddeepestr(Node *a[],Node *p,int &cal);//Ѱ��ÿ����֧����ظ��ַ��� 
		//
		//int count(string str);//�����ַ���str���ֵĴ��� 
		//int countleaf(Node *p);//����p�ڵ��µ�Ҷ�ڵ���� 
		//bool judgeleaf(Node *p);//�ж�p�ڵ����Ƿ�ȫΪҶ�ڵ� 
		//
		//int find(char x);//����ָ���ĺ�׺�Ƿ���� 
		//void build(string str);//������׺�� 
		//void deal(string str,int currentindex);//�����׺���� 
		//
		//void showtree(){show(root,0,0);}//��ӡ��׺�� 
		//void show(Node *p,int repeat,int len);//��ӡ��׺��ʵ�ʺ��� 
		/****************************************************************************
		**delall(Node *p)������
		**�ͷŽڵ�p�����к���
		****************************************************************************/

		void delall(Node* p)
		{
			for (int i = p->count - 1; i >= 0; i--)
			{
				if (p->child[i]->below->flag >= 0)
				{
					//cout<<"�ͷű�"<<p->child[i]->str<<"��*�ڵ�ĵ�"<<i+1<<"�����ӽڵ�"<<"\n"; 
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
				//cout<<"�ͷű�"<<q->child[q->count-1]->str<<"��*�ڵ�ĵ�"<<q->count<<"������"<<"\n"; 
				delete q->child[q->count - 1]->below;
				delete q->child[q->count - 1];
				q->count--;
				//del(q,q->count-1);
			}
			else
			{
				delete p;
				//cout<<"�ͷŸ��ڵ�"<<endl;
			}
		}
		
		/****************************************************************************
		**getlength(Node *p)������
		**���ش�p�ڵ㵽���ڵ����������ַ���ƴ�Ӷ��ɵ��ַ����ĳ���
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
		**getst(Node *p)������
		**ƴ�ӴӸ��ڵ㵽p�ڵ�������ߵ��ַ���������
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
		**search(Node *p,string str,Node *&cur)������
		**�ڽڵ�p�ĺ�������Ѱ���ַ���strƥ��ģ��������ȫƥ��ģ��ͽ�
		**�˽ڵ�ĵ�ַ����cur
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
		**findstr(string str)������
		**�����ַ���str�Ƿ����
		**���ڷ���true
		****************************************************************************/
		bool findstr(string str)
		{
			Node* cur = NULL;
			return search(root, str, cur);
		}
		/****************************************************************************
		**findlongest()������
		**�����׺������ظ��ַ��������ظ��ַ���
		****************************************************************************/
		string findlongeststr()
		{
			string s = "";
			int len = 0, cal = 0;
			Node* a[mmax];
			finddeepestr(a, root, cal);//����ÿ����֧�������ڲ��ڵ� 
			for (int i = 0; i < cal; i++)//��ʼ�Ƚ�ÿ�������ڲ��ڵ���ƴ�����ַ����ȣ����س��ȴ�� 
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
		**findeepest(Node *a[],Node *p,int &cal)������
		**��¼p�ڵ���ÿ����֧�������ظ��ַ������������¼������a�� �������ñ���cal������
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
		**count(string str)������
		**�����׺����str���ֵĴ���
		****************************************************************************/
		int count(string str)
		{
			Node* cur = NULL;
			bool signal = search(root, str, cur);//�ж��Ƿ��ҵ������ҵ����򽫵�ַ���ظ�cur 
			if (signal == false)
				return 0;
			else//�ҵ���������countleaf����curָ��ڵ��Ҷ���� 
			{
				if (cur->flag >= 0)
					return 1;
				return countleaf(cur);
			}

		}
		/****************************************************************************
		**countleaf(Node *p)������
		**����ڵ�p�ĺ�����Ҷ�ڵ����
		****************************************************************************/
		int countleaf(Node* p)
		{
			int counts = 0;
			if (p->flag >= 0)//�����ǰ�ڵ���Ҷ�ڵ㣬ֱ�ӷ���0 
				return 0;
			else
			{
				for (int i = 0; i < p->count; i++)//�����жϽڵ��µĽڵ��Ƿ�ΪҶ�ڵ㣬������counts��һ������ݹ���� 
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
		**judgeleaf(Node *p)������
		**�ж�p�ڵ�ĺ����Ƿ�ȫΪҶ�ڵ㣬���򷵻�true�����򷵻�false
		****************************************************************************/
		bool judgeleaf(Node* p)
		{
			for (int i = 0; i < p->count; i++)
			{
				Node* t = p->child[i]->below;
				if (t->flag < 0)// �����ǰ�ڵ㲻��Ҷ�ڵ� 
					return false;
			}
			return true;
		}

		/****************************************************************************
		**int find(char x)������
		**���ҵ�ǰ��׺�Ƿ���ڣ������ڷ���-1
		****************************************************************************/
		int find(char x)
		{
			Edge*& apedge = activepoint->edge;
			Node*& apnode = activepoint->node;
			int& aplen = activepoint->length;
			if (apedge == NULL)
			{//�޻�ߣ���ӻ�ڵ���ӽڵ㿪ʼ����
				for (int i = 0; i < apnode->count; i++)
				{
					//cout<<i;
					Edge* tempedge = apnode->child[i];
					if (tempedge->str[0] == x)
					{
						aplen++;
						apedge = apnode->child[i];
						if (aplen == apedge->str.length())
						{//���������һ���޸Ļ��Ĺ��򣺼��������ϵ������ַ�ȫ������ƥ������
						 //(������ϵ��ַ���==�����)���򽫻�߽���Ϊ��㣬ͬʱ���û����Ϊ0��
						 //�����´β���ʱ�͵ôӸýڵ㿪ʼ�ˣ������Ǹ��ڵ��ˡ�

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
			{// �л�ߣ����ڻ���ϲ���
				if (apedge->str[aplen] == x)
				{
					aplen++;
					if (aplen == apedge->str.length())
					{//���������һ���޸Ļ��Ĺ��򣺼��������ϵ������ַ�ȫ������ƥ������
					 //(������ϵ��ַ���==�����)���򽫻�߽���Ϊ��㣬ͬʱ���û����Ϊ0��
					 //�����´β���ʱ�͵ôӸýڵ㿪ʼ�ˣ������Ǹ��ڵ��ˡ�

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
		**build(string str)������
		**��str�����׺��
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
				//cout<<"\n��ǰ�����: "<<index<<","<<str[index]<<"\n";
				//test();cout<<"\n" ;
				int currentindex = index++;
				char w = str[currentindex];

				//�����Ƿ���ڱ����е�ǰ��׺�ַ��Ľڵ� 
				if (find(w) != -1)//����ҵ��� 
				{
					suffixarray += w;
					reminder++;
					continue;
				}
				else //���δ�ҵ� 
				{
					suffixarray += w;
					reminder++;
				}
				active = NULL;
				deal(str, currentindex);

			}
		}

		/****************************************************************************
		**deal(string str,int currentindex,int number)������
		**�����׺�ַ���str��������ַ���currentindex�Ǵ�����λ�ã�number��ʾ���β���
		**ʹ���˼��κ�׺����
		****************************************************************************/
		void deal(string str, int currentindex)
		{
			//cout<<"\n----------------------------------------------\n";
			//cout<<"deal������ڣ�\n";
		   // test();show(root,0,0);


			Edge*& apedge = activepoint->edge;
			Node*& apnode = activepoint->node;
			int& aplen = activepoint->length;
			if (reminder == 1)//���ʣ���׺��Ϊ1 //pay attention to//�Ƿ�һ��Ϊ������reminderΪ1��ʱ�� 
			{
				if (apnode == root)//�����ڵ��Ǹ��ڵ� 
				{//�½��ڵ� 
					Node* tempnode1 = new Node(currentindex - suffixarray.length() + 1);
					Edge* tempedge1 = new Edge(apnode, tempnode1, str.substr(currentindex));
					suffixarray.erase(0, 1);
					reminder--;
					apedge = NULL;

					//cout<<"deal�������ڣ�\n";
					//test();show(root,0,0);
					//cout<<"\n----------------------------------------------\n";
					return;
				}
				else//�����ڵ㲻�Ǹ��ڵ�,apnode!=root 
				{

				}
			}
			else//ʣ���׺������1
			{
				if (apnode == root)
				{
					//����һ����Ծ��Ϊ���ڵ�ʱ��Ĳ��룩��
					//o����Ҷ��֮�󣬻�Ծ�ڵ�����Ϊ���ڵ㣻
					//o��Ծ�߸���Ϊ���ǽ�����Ҫ���µĺ�׺������ĸ�� 
					//o��Ծ�뾶��1;
					if (apedge == NULL)//�����߲����ڣ���˵����ڵ�����Ҫ�´����ڵ�
					{
						Node* tempnode1 = new Node(currentindex);
						Edge* tempedge1 = new Edge(apnode, tempnode1, str.substr(currentindex));
						//�����������Ϊ�� 		
					}
					else
					{
						Edge* edge = apedge;//���浱ǰ��ߣ�Ҳ���ں���ͷž��еĻ�� 
						apedge = NULL;
						aplen--;//��Ϊһ�����ҵ������Ѱ�ҹ����л�ʹ��aplen++���˴�����
						int m = find(edge->str[0]);//Ѱ�ұ�ţ�����½��ڵ���õ� 


						Node* tempnode1 = new Node();
						Edge* tempedge1 = new Edge(tempnode1, apedge->below, apedge->str.substr(aplen));
						Edge* tempedge2 = new Edge(apnode, m, tempnode1, apedge->str.substr(0, aplen));

						Node* tempnode2 = new Node(currentindex - suffixarray.length() + 1);
						Edge* tempedge3 = new Edge(tempnode1, tempnode2, str.substr(currentindex));

						apedge = apnode->child[m];
						delete edge;//�ͷž��еĻ�� 




					}



					//���������׺������
					//oÿ���׶Σ������ǽ����µ��ڲ��ڵ㲢�Ҳ��Ǹý׶ε�һ�ν����ڲ��ڵ��ʱ��
					//������Ҫ��ָ��ӵ�ǰ�ڲ��ڵ�ָ�򱾽׶����һ�ν������ڲ��ڵ㡣 

					//�����ǰ�½��ڵ����ڲ��ڵ㣬����º�׺���� 
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


					apedge = NULL;//apnode�Ѿ�Ϊ�� 
					aplen = 0;
					int flag;
					for (int i = 0; i < reminder; i++)
					{
						flag = find(suffixarray[i]);
					}


					if (flag == -1)
					{
						//cout<<"deal�������ڣ�\n";
						//test();show(root,0,0);
						//cout<<"\n----------------------------------------------\n";
						deal(str, currentindex);
						return;
					}
					else
					{
						//cout<<"deal�������ڣ�\n";
						//test();show(root,0,0);
						//cout<<"\n----------------------------------------------\n";
						return;
					}



				}
				else//apnode!=root
				{
					//����������Ծ�㲻Ϊ���ڵ�ʱ��Ĳ��룩��
					//o�����ǰ��Ծ�㲻�Ǹ��ڵ㣬��ô����ÿ�δӻ�Ծ���½�һ��Ҷ��֮��
					//��Ҫ���ź�׺�������µĽڵ㣬�����»�Ծ�ڵ㣬��������ں�׺����
					//���Ǿ�ת�Ƶ����ڵ㣬����Ծ�ڵ����Ϊ���ڵ㵫��Ծ�뾶�Լ���Ծ�߲��䡣

					char temp;
					if (apedge == NULL)//�����߲����ڣ���˵����ڵ�����Ҫ�´����ڵ�
					{
						Node* tempnode1 = new Node(currentindex - suffixarray.length() + 1);
						Edge* tempedge1 = new Edge(apnode, tempnode1, str.substr(currentindex));
						//���ʱ���ڵ���ô���壿������ //���ɵ����½����ڲ��ڵ㴦�� 	
					}
					else
					{


						Edge* edge = apedge;
						temp = edge->str[0];
						apedge = NULL;
						aplen--;
						int m = find(edge->str[0]);


						Node* tempnode1 = new Node();
						//cout<<"what happened?\n";//������������һ���������ǰ���"aplen--"��"int m=find(edge->str[0])"˳���������������� 
						//cout<<apedge->str<<" "<<aplen;//��˳�򷴺󣬻����ܻ��������������ⲻ������Ҫ�� 
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

					//�����ǰ�½��ڵ����ڲ��ڵ㣬����º�׺���� 
					if (apedge != NULL && apedge->below->count > 1)
					{
						if (active == NULL)//ע����ж����ж��Ƿ�Ϊ�ڲ��ڵ㣡���� 
							active = apedge->below;
						else
						{
							active->next = apedge->below;
							active = apedge->below;
						}
					}
					else
					{
						if (active == NULL)//ע����ж����ж��Ƿ�Ϊ�ڲ��ڵ㣡���� 
							active = apnode;
						else
						{
							active->next = apnode;
							active = apnode;
						}

					}


					//��ʼ���ź�׺����Ѱ��,�������û�� 
					if (apnode->next != NULL)//��������ӣ��ͽ��� 
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
							//cout<<"deal�������ڣ�\n";
							//test();show(root,0,0);
							//cout<<"\n----------------------------------------------\n";
							deal(str, currentindex);
							return;
						}
						else
						{
							//cout<<"deal�������ڣ�\n";
							//test();show(root,0,0);
							//cout<<"\n----------------------------------------------\n";
							return;
						}


					}
					else//�����ǰ�ڵ������ӣ����û�ڵ�Ϊ���ڵ� 
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
							//cout<<"deal�������ڣ�\n";
							//test();show(root,0,0);
							//cout<<"\n----------------------------------------------\n";			        	
							deal(str, currentindex);
							return;
						}
						else
						{
							//cout<<"deal�������ڣ�\n";
						   //test();show(root,0,0);
							//cout<<"\n----------------------------------------------\n";			        	
							return;
						}


					}
				}//apnode!=root�ս� 

			}//reminder>1�ս� 

		}
		/****************************************************************************
		**show(Node *p,int repeat,int len)������
		**��ӡp�ڵ�֮�µĺ�׺��
		**repeat��ʾ�������ȣ�len��ʾ���뵱ǰ��׼�ľ���
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
				} *///���ڲ���judge������ 
				if (edge->below->next != NULL)
				{
					cout << "~~~~" << getstr(edge->below->next) << "~~~~~~*";
				}
				if (edge->below->count == 0)//������������Ľڵ�û�����������ı�ʱ 
				{

				}
				else//������������Ľڵ����ӱ� 
				{
					show(edge->below, ++repeat, templen + len);
				}
			}
			cout << "\n";
		}

		void showtree() { show(root, 0, 0); }

		void test()//�����ú�����չʾ��ǰ��㣬��׺�ַ���ʣ���׺������Ϣ 
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
