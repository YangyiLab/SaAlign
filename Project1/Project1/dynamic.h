#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<time.h> 
#include<iomanip>
#include<sstream>
#include<stdlib.h>
//#include <dynamic.cpp>

using namespace std;

vector<string> trackBack(vector < vector<int> > operMatrix, const string& s1,
	const string& s2);
vector<string> compare_two(string s1, string s2);
//vector<string> compare_two_sequ(string s1, string s2);
vector<string> handleShort(string s1, string s2);

//pair<pair<string, string>, int> Hirscheberg(string s1, string s2, Mismatch mismatch);