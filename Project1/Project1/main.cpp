#include "suffixtree.h"
#include"dynamic.h"
#include <iostream>
#include <string.h>
#include <string>
#include <set>
#include <vector>
#include<map>
#include <string.h>
#include <algorithm>
using namespace std;
SuffixTree* a = new SuffixTree();
/****************************************************************************
**compare(string s1,string s2)方法：
**比较s1和s2，如果s1不是s2的前缀，返回-1，如果s1是s2的前缀，返回0，
**如果s2是s1的前缀，返回已匹配的长度
****************************************************************************/
int compare(string s1, string s2)
{
	int len1 = s1.length(), len2 = s2.length();
	bool signal;
	if (len1 <= len2)
	{
		for (int i = 0; i < len1; i++)
		{
			if (s1[i] != s2[i])
				return -1;
		}
		return 0;
	}
	else
	{
		if (len2 == 0)
			return -1;
		for (int i = 0; i < len2; i++)
		{
			if (s1[i] != s2[i])
				return -1;
		}
		return len2;
	}
}
/****************************************************************************
**printspace(int m)方法：
**打印长度为m的空格
****************************************************************************/
void printspace(int m)
{
	for (int i = 0; i < m; i++)
		cout << " ";
};

bool myfunction(char* i, char* j) { return (strcmp(i,j)>0); }

string longcom(string s) {
	char* sa[s.length()];
	char str[5000000];
	strncpy(str,s.c_str(),sizeof(str)-1);
	for (int i = 0; i < s.length(); i++) {
		//依次将后i个元素放入vs中
		sa[i] = str + i;
	}
	//排序并返回
	sort(sa, sa + s.length(),myfunction);
	int maxLen = 0;
	string ret;
	int n = s.length();
	char** vs = sa;
	for (int i = 0; i < n - 1; i++) {
		string cur = vs[i];
		string suf = vs[i + 1];

		int tlen = 0;
		//循环条件是尚未到达任意一个的末尾
		for (int j = 0; j < min(cur.find("$"), suf.length()); j++) {
			//如果发现有不匹配，立刻退出循环，
			//如果是第一个元素就不匹配，那么没有必要匹配下去
			//如果是后面的元素不匹配，那么在上一次循环中已经设置了maxLen
			if (cur.find("$") == suf.find("$"))break;
			if ((int)cur.find("$")>0 && (int)suf.find("$") > 0)break;
			if (cur[j] != suf[j]) {
				tlen = 0;
				break;
			}
			else if (cur[j] == suf[j]) {
				tlen++;
			}
			//更新maxLen
			if (maxLen < tlen ) {
				maxLen = tlen;
				ret = suf.substr(0, maxLen);
				//cout<<ret<<endl;
			}
		}
	}
	return ret;
}




vector<string> comp(string s1,string s2) {
	if (s1.length() <= 1000 && s2.length() <= 1000) {
		if (s1.length() < 100 && s2.length() < 100 && (s1.length() - s2.length())^2<2500)
			return handleShort(s1, s2);
		if (s1.length() < 10 || s2.length() < 10)
			return handleShort(s1, s2);
		return compare_two(s1, s2);
	}
		
	char res[200];
	//suffixArrayQsort_longestCommonSubstring((char*)s1.c_str(),(char*)s2.c_str(),res);
	string a = longcom(s1 + "$" + s2 + "#");
	int pos1 = s1.find(a);
	string s11 =s1.substr(0, pos1);
	string s12 = s1.substr(pos1+a.length(),s1.length());
	int pos2 = s2.find(a);
	string s21 = s2.substr(0, pos2);
	string s22 = s2.substr(pos2 + a.length(), s2.length());
	if ((s1 == s11) || (s1 == s12) || (s2 == s21) || (s2 == s22) || a.length() < 3) 
		return compare_two(s1, s2);
	
		
	vector<string>containerPre=comp(s11, s21); 
	vector<string>containerPost=comp(s12, s22);
	return vector<string>{containerPre[0] +a+ containerPost[0], containerPre[1] +a+ containerPost[1]};
}


