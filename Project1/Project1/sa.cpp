//#include<stdio.h>
//#include<iostream>
//#include<string.h>
//#include<algorithm>
//using namespace std;
//int suffixArrayQsort_longestCommonSubstring(char* text, char* pattern, char* commen)
//{
//	if (!text || !pattern)  return 0;     //nullptr
//	int tlen = strlen(text), plen = strlen(pattern), i, j;
//	if (0 == tlen || 0 == plen) return 0; //empty string
//
//	enum ATTRIB { TEXT, PATTERN };
//	struct absInfo
//	{
//		char* head;
//		ATTRIB attr;  //tag
//		int len;
//		absInfo() :head(NULL), attr(TEXT), len(0) {}
//		absInfo(char* phead, ATTRIB attrib, int length) :head(phead), attr(attrib), len(length) {}
//		bool operator < (const absInfo& b)
//		{
//			return  strcmp(head, b.head) < 0;
//		}
//		static void display(const absInfo& a)
//		{
//			printf("size:%d type:%-7s    ", a.len, (a.attr == TEXT ? "TEXT" : "PATTERN"));
//			printf("%s\n", a.head);
//		}
//	}*sa;
//
//	//step 2:build the suffix array
//	sa = new absInfo[tlen + plen];
//	for (i = 0; i < tlen; ++i)
//	{
//		sa[i].head = text + i;
//		sa[i].attr = TEXT;
//		sa[i].len = tlen - i;
//	}
//	for (j = 0; j < plen; ++j)
//	{
//		sa[j + tlen].head = pattern + j;
//		sa[j + tlen].attr = PATTERN;
//		sa[j + tlen].len = plen - j;
//	}
//
//	//step 3:use sort() to sort the sa
//	puts("before sort, the sa is:"); for_each(sa, sa + tlen + plen, absInfo::display);
//	sort(sa, sa + tlen + plen);
//	puts("after sort, the sa is:"); for_each(sa, sa + tlen + plen, absInfo::display);
//
//	//step 4:compare
//	int maxLEN = 0, rec = 0,pos=0;
//	for (i = 0; i < tlen + plen - 1; i++)
//	{
//		if (sa[i].attr == sa[i + 1].attr) continue;
//		if (sa[i].len <= maxLEN || sa[i + 1].len <= maxLEN) continue;
//		rec = 0;
//		while (rec < sa[i].len && rec < sa[i + 1].len && *(sa[i].head + rec) == *(sa[i + 1].head + rec))
//			++rec;
//		if (rec > maxLEN)  maxLEN = rec; pos = i; //update
//	}
//	strncpy(sa[pos].head,commen,maxLEN);
//	//release memory resource and return
//	delete[] sa; sa = NULL;
//	return maxLEN;
//}
//
//int main() {
//	char str[100];
//	suffixArrayQsort_longestCommonSubstring("attcgcg", "tactcgcg",str);
//
//}