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
**compare(string s1,string s2)������
**�Ƚ�s1��s2�����s1����s2��ǰ׺������-1�����s1��s2��ǰ׺������0��
**���s2��s1��ǰ׺��������ƥ��ĳ���
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
**printspace(int m)������
**��ӡ����Ϊm�Ŀո�
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
		//���ν���i��Ԫ�ط���vs��
		sa[i] = str + i;
	}
	//���򲢷���
	sort(sa, sa + s.length(),myfunction);
	int maxLen = 0;
	string ret;
	int n = s.length();
	char** vs = sa;
	for (int i = 0; i < n - 1; i++) {
		string cur = vs[i];
		string suf = vs[i + 1];

		int tlen = 0;
		//ѭ����������δ��������һ����ĩβ
		for (int j = 0; j < min(cur.find("$"), suf.length()); j++) {
			//��������в�ƥ�䣬�����˳�ѭ����
			//����ǵ�һ��Ԫ�ؾͲ�ƥ�䣬��ôû�б�Ҫƥ����ȥ
			//����Ǻ����Ԫ�ز�ƥ�䣬��ô����һ��ѭ�����Ѿ�������maxLen
			if (cur.find("$") == suf.find("$"))break;
			if ((int)cur.find("$")>0 && (int)suf.find("$") > 0)break;
			if (cur[j] != suf[j]) {
				tlen = 0;
				break;
			}
			else if (cur[j] == suf[j]) {
				tlen++;
			}
			//����maxLen
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


