//
//#include "suffixtree.h" 
//using namespace std; 
///****************************************************************************
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
//****************************************************************************/
//void initial()
//{
//	cout<<"\n\n\n\n\n\t\t\t 欢迎进入!"<<endl;//欢迎界面 
//    cout<<"\n\n\n\t\t\t 后缀树操作程序！"<<endl;//准备开始
//    //_getch(); 
//    system("cls");
//}
//void inout()
//{
//	SuffixTree* a = new SuffixTree();
//	string b, x;
//	int fl = 1, n = 1;
//
//	do
//	{
//
//		if (fl == 1)
//		{
//			if (n == 1)
//			{
//				cout << "\n请输入原字符串:";
//				cin >> b;//abbbabbbabbbd
//				//b="ababaaabbbabbbba$";
//				cout << "原字符串为: " << b << "\n";//vvhhhvvhhv$hhvh//lkjjlllllj%
//				a->build(b);
//				n = 0;
//				cout << "0.退出  1.重新输入字符串  2.追加字符串  3.显示后缀树  4.字符串查找  5.最长重复字符串查找  6.显示字符串  7.显示要求  8.清屏\n";
//			}
//			else
//			{
//				//buildstep=0;
//				cout << "请重新输入字符串:";
//				cin >> b;
//				cout << "现在字符串为: " << b << endl;
//				a->~SuffixTree();
//				a = new SuffixTree();
//				a->build(b);
//			}
//
//		}
//		else if (fl == 2)
//		{
//
//			cout << "请输入追加字符串:";
//			cin >> b;
//			b = a->getallstr() + b;
//			cout << b << "\n";
//			a->~SuffixTree();
//			a->build(b);
//			cout << "现在字符串为: " << a->getallstr() << endl;
//		}
//		else if (fl == 3)
//		{
//			cout << "此字符串的后缀树为:" << endl;
//			a->showtree();
//			cout << "0.退出  1.重新输入字符串  2.追加字符串  3.显示后缀树  4.字符串查找  5.最长重复字符串查找  6.显示字符串  7.显示要求  8.清屏\n";
//		}
//		else if (fl == 4)
//		{
//			cout << "请输入你想要查找的字符串:";
//			cin >> x;
//			cout << "要查找的字符串为: " << x << endl;
//			bool signal = a->findstr(x);
//			cout << "1.查找结果:";
//			if (signal == true)
//				cout << "找到";
//			else
//				cout << "未找到";
//			cout << "\n";
//			int y = a->count(x);
//			cout << "2.频率查找:";
//			cout << "字符串\"" << x << "\"出现的次数为" << y << "次";
//			cout << endl;
//		}
//		else if (fl == 5)
//		{
//			string temp = a->findlongeststr();
//			cout << "最长重复字符串查找:";
//			if (temp != "")
//				cout << "最长重复字符串(之一)为\"" << temp << "\"\n";
//			else
//				cout << "没有最长重复字符串\n";
//		}
//		else if (fl == 6)
//			cout << "原字符串为: " << a->getallstr() << endl;
//		else if (fl == 7)
//			cout << "0.退出  1.重新输入字符串  2.追加字符串  3.显示后缀树  4.字符串查找  5.最长重复字符串查找  6.显示字符串  7.显示要求  8.清屏\n";
//		else if (fl == 8)
//		{
//			system("cls");
//			cout << "\n字符串为: " << a->getallstr() << endl;
//			cout << "0.退出  1.重新输入字符串  2.追加字符串  3.显示后缀树  4.字符串查找  5.最长重复字符串查找  6.显示字符串  7.显示要求  8.清屏\n";
//		}
//
//		else
//			cout << "输入无效，请重新输入\n";
//		cin >> fl;
//	} while (fl != 0);
//	cout << "已退出\n";
//}
//
//int main() { inout(); }
////fdasjojiofdasjifodajifaeojifadsjio$
////fdjojiofdjifojifajif$
////fdjojiofdjifojifaji$
