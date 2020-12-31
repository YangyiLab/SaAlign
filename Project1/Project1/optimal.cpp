#include"optimal.h"
#include"dynamic.h"
#include <sys/utsname.h>
#include <iostream>
#include <cstdio>
#include<string.h>
#include "suffixtree.h"
//#include "main.cpp"
#define timeTaken float(endt - begt)/CLOCKS_PER_SEC
#define STRINGNUMBER 5
#define NUMBEROFDNA 4
class state;

using namespace std;

float ttime;

time_t begt, endt;

bool isSubOptimal;

// name justifies
float mcAvg = 0;

//this factor decides the heuristic value
float divideFactor = 2.0;

// final cost
long minCost = 99999;

// they will be used by center star method where edit distance will return them as the output strings
// purely temporary
string s3, s4;

//20 is the max number of strings
string storeStrings[2][2][20][20][2];
long storeCosts[2][2][20][20];

// for analysis
long statesProcessed = 0;
long statesEncountered = 0;
long nodeSaved = 0;

// number of characters
int vocabNumber = NUMBEROFDNA;

// vector containing all the characters in the vocabulary
vector<char> vocab = { 'A','T','C','G' };

// number of strings
int stringNumber;

// vector of strings
vector<string> strings;

// each character in strings has been replaced by its index in vocab vector
vector< vector<int> > stringInts;


state* minState;

// CC*no. of dashes is the conversion cost
int CC;

vector< vector<int> > MC = { {0,2,2,2,2},{2,0,2,2,2},{2,2,0,2,2},{2,2,2,0,2},{
	1,1,1,1,0} };

// true for ith string if that string is the largest
vector<bool> isMaxLength;

// this is the index in MC for hyphen
int hyphen;

typedef vector<int>::iterator intIterator;
typedef string::iterator stringIterator;
typedef vector<string>::iterator vecStringIterator;
typedef vector<state>::iterator stateIterator;


// finds the editDistance between the strings so far and returns the number of matches
// apply DP here
long editDistance(string s1, string s2, vector<int> v1, vector<int> v2, int depth) {

	s3.clear();
	s4.clear();

	//    if( depth == 1 )
	//        cout << "Strings are " << s1 << " " << s2;

	int rows = s1.size() + 1;
	int cols = s2.size() + 1;

	int** A = new int* [rows];
	char** B = new char* [rows];
	for (int i = 0; i < rows; i++) {
		A[i] = new int[cols];
		B[i] = new char[cols];
	}

	//    initialize

	A[0][0] = 0;
	B[0][0] = 'O';

	for (int i = 1; i < rows; i++) {
		A[i][0] = A[i - 1][0] + CC + MC[v1.at(i - 1)][vocabNumber];
		B[i][0] = '|';
	}
	for (int i = 1; i < cols; i++) {
		A[0][i] = A[0][i - 1] + CC + MC[v2.at(i - 1)][vocabNumber];
		B[0][i] = '-';
	}

	//    do it
	for (int i = 1; i < rows; i++) {
		for (int j = 1; j < cols; j++) {
			int choice1, choice2, choice3;
			choice1 = A[i - 1][j - 1] + MC[v1[i - 1]][v2[j - 1]]; // '\'
			choice2 = A[i - 1][j] + MC[v1[i - 1]][vocabNumber] + CC; // '|'
			choice3 = A[i][j - 1] + MC[v2[j - 1]][vocabNumber] + CC; // '-'

//            cout << "\ni j choice1,2,3 " << i << " " << j << " " << choice1 << " " << choice2 << " " << choice3;

			if (choice1 <= choice2 && choice1 <= choice3) {
				A[i][j] = choice1;
				B[i][j] = '\\';
			}
			else if (choice2 <= choice1 && choice2 <= choice3) {
				A[i][j] = choice2;
				B[i][j] = '|';
			}
			else {
				A[i][j] = choice3;
				B[i][j] = '-';
			}

		}
	}

	//    for( int i = 0; i < rows; i++ ) {

	//        cout << "\n";
	//        for( int j = 0; j < cols; j++ ) {
	//            cout << A[i][j] << " ";
	//        }
	//    }

	//    for( int i = 0; i < rows; i++ ) {
	//        cout << "\n";

	//        for( int j = 0; j < cols; j++ ) {
	//            cout << B[i][j] << " ";
	//        }
	//    }

	for (int i = rows - 1, j = cols - 1; (i > 0) || (j > 0);) {
		if (B[i][j] == '\\') {
			s3.push_back(s1[i - 1]);
			s4.push_back(s2[j - 1]);
			i--;
			j--;
		}
		else if (B[i][j] == '|') {
			s3.push_back(s1[i - 1]);
			s4.push_back('-');
			i--;
		}
		else {
			s3.push_back('-');
			s4.push_back(s2[j - 1]);
			j--;
		}
	}

	reverse(s3.begin(), s3.end());
	reverse(s4.begin(), s4.end());

	long result = A[rows - 1][cols - 1];

	//    if( depth == 1 ) {
	//        cout << " - values returned -> " << s3 << " " << s4;
	//        cout << " " << result;
	//    }

	for (int i = 0; i < rows; i++) {
		delete[] A[i];
		delete[] B[i];
	}
	delete[] A;
	delete[] B;

	s1.clear();
	s2.clear();

	return result;

}

vector<int> getStringInts(string s) {
	vector<int> temp;
	for_each(s.begin(), s.end(), [&](char c) {
		for (int i = 0; i < vocabNumber; i++) {
			if (c == vocab.at(i)) {
				temp.push_back(i);
				break;
			}
		}
		if (c == '-')
			temp.push_back(vocabNumber);
		});
	return temp;
}

// a state is composed of the starting index of each string
// goal states is having the last index for each string
// we have to reach the goal state with the minimum possible cost
class state {

public:

	//    this will be 1 when largest string will introduced hyphen
	int extraHyphens = 0;

	//    time when this state was discovered
	float discoveredAt;

	//    depth of state in the tree
	int depth = 0;

	//    estimated cost that will be incurred from this state to goal
	float heuristicMinCost = 0.0;

	//    pointer to the previous state
	state* previousState;

	//    we came to this state after giving this vector to findCost()
	vector<int> previousCostInput;

	//    this is the cost of coming to this state from the previous state i.e. cost incurred by this state
	int costIncurred;

	//    this is the cost so far to come to this state including this state's cost
	int costSoFar;

	//    starting indices for each string
	vector<int> startingIndex;

	//    starting index for previous state
	vector<int> previousStartingIndex;

	//    this is to be fed to findCost to find costIncurred, this will include hyphens
	vector<int> currentConcern;

	//    vector of strings so far encountered upto this state including hyphen
	vector<string> stringsSoFar;

	//    number of hyphens added in this state
	int hyphens = 0;

	//    only used by Center Star Method
	vector<string> globalAlignment;

	int findCost(vector<int> s) {

		int cost = 0;

		while (s.size() > 1) {
			//            start it from the next element in the list
			for (intIterator it = s.begin() + 1; it != s.end(); it++) {
				cost += (MC.at(*(s.begin()))).at(*it);
			}
			if (*(s.begin()) == vocabNumber)
				cost += CC;
			s.erase(s.begin());
		}

		if (*(s.begin()) == vocabNumber)
			cost += CC;

		return cost;

	}

	state() {}

	//    1st argument is the startingIndex of this state
	//    2nd argument is the startingIndex of the previous state
	//    3rd argument is the stringsSoFar of the previous state
	//    4th argument is costSoFar of the previous state
	//    5th argument is depth of the previous state
	state(vector<int> startingIndex, vector<int> previousStartingIndex, vector<string> previousStringsSoFar, int previousCostSoFar, int previousDepth) {

		endt = clock();

		this->startingIndex = startingIndex;
		this->previousStartingIndex = previousStartingIndex;

		//        for_each( startingIndex.begin(), startingIndex.end(), [](int i){
		//            cout << i;
		//        });
		//        cout << endl;
		//        for_each( previousStartingIndex.begin(), previousStartingIndex.end(), [](int i){
		//            cout << i;
		//        });
		//        cout << endl;

		this->depth = previousDepth + 1;
		this->costIncurred = 0;

		this->discoveredAt = timeTaken;

		for (int i = 0; i < stringNumber; i++) {
			string temp = previousStringsSoFar.at(i);

			if (startingIndex.at(i) > previousStartingIndex.at(i)) {
				temp.push_back((strings.at(i)).at(previousStartingIndex.at(i)));
				currentConcern.push_back((stringInts.at(i)).at(previousStartingIndex.at(i)));
			}
			else {
				temp.push_back('-');
				currentConcern.push_back(vocabNumber);
				hyphens++;
			}

			stringsSoFar.push_back(temp);
			temp.clear();
		}

		//        find cost incurred after matching
		this->costIncurred = findCost(currentConcern);
		this->costSoFar = costIncurred + previousCostSoFar;
		//        if( depth == 1 ) {
		//            viewStateInfo();
		//            cout << "GONNA check heuristic\n";
		//        }

		heuristic();

		//        if( depth == 1 ) {
		//            cout << "Heuristic returned is " << this->heuristicMinCost;
		//        }
	}

	~state() {
		this->currentConcern.clear();
		this->previousCostInput.clear();
		this->startingIndex.clear();
		this->stringsSoFar.clear();
		this->tempInts.clear();
		this->tempStates.clear();
	}

	//    only used by exploreStates() and getState()
	vector<state> tempStates;
	vector<int> tempInts;
	state* tempState;

	void getState(int i) {

		endt = clock();

		if (ttime - timeTaken < 10)
			return;

		//        if the whole tempInts has been created and ready
		if (i == strings.size()) {

			tempState = new state(tempInts, this->startingIndex, this->stringsSoFar, this->costSoFar, this->depth);

			//            if the new state can be better than the solution obtained yet, use it
			if (tempState->costSoFar + tempState->heuristicMinCost < minCost) {
				//                for_each( tempState->startingIndex.begin(), tempState->startingIndex.end(), [](int i) {cout<<i;} );
				//                cout << " ";
				tempStates.push_back(*tempState);
			}
			else {
				//                for_each( tempState->startingIndex.begin(), tempState->startingIndex.end(), [](int i) {cout<<i;} );
				//                cout << "(ignored[" << tempState->costSoFar << "]) ";
			}

			delete tempState;

			return;
		}
		//        i is not referring to the last element

		if (strings.at(i).size() > startingIndex.at(i)) {
			tempInts.push_back(startingIndex.at(i) + 1);
			getState(i + 1);
			tempInts.erase(tempInts.begin() + i, tempInts.end());
		}

		tempInts.push_back(startingIndex.at(i));

		getState(i + 1);

	}

	//    only used by center star method
	void addHyphen(int k) {
		transform(globalAlignment.begin(), globalAlignment.end(), globalAlignment.begin(), [&](string s) {
			s.insert(s.begin() + k, '-');
			return s;
			//            cout << s << endl;
			});
	}


	// desirability of coming to this states is determined
	void heuristic() {

		//        cout << "Finding h";fflush(stdout);

		heuristicMinCost = 0;

		//        going to implement Center star approximation method
		//        create a matrix of size nXn where n is the number of strings

		long* stringCostSum = new long[stringNumber];

		for (int i = 0; i < stringNumber; i++) {
			stringCostSum[i] = 0;
		}

		//        for all nC2 combination of strings, find edit distance cost
		for (int k = 0; k < stringNumber; k++) {

			for (int l = k + 1; l < stringNumber; l++) {

				//                l is always index ahead than k
				int i = this->startingIndex.at(k) - this->previousStartingIndex.at(k);
				int j = this->startingIndex.at(l) - this->previousStartingIndex.at(l);

				float temp = storeCosts[i][j][k][l];

				stringCostSum[k] += temp;
				stringCostSum[l] += temp;

			}
		}

		long tempMinCost = 99999999;
		int tempMinIndex;

		for (int i = 0; i < stringNumber; i++) {
			if (tempMinCost > stringCostSum[i]) {
				tempMinCost = stringCostSum[i];
				tempMinIndex = i;
			}
		}

		//        if( depth == 1 )
		//            cout << endl;

		//        these will hold the aligned strings, Sc[i] being center string aligned with Si[i] string
		//        if tempMinIndex is 3, Sc[3] and Si[3] will be useless
		vector<string> Sc;
		vector<string> Si;

		//        find all the aligned strings aligned with the most similar string
		for (int l = 0; l < stringNumber; l++) {
			if (l != tempMinIndex) {

				int i = this->startingIndex.at(tempMinIndex) - this->previousStartingIndex.at(tempMinIndex);
				int j = this->startingIndex.at(l) - this->previousStartingIndex.at(l);
				//                cout << "ij - " << i << " " << j << endl;
				if (tempMinIndex < l) {
					Sc.push_back(storeStrings[i][j][tempMinIndex][l][0]);
					Si.push_back(storeStrings[i][j][tempMinIndex][l][1]);
				}
				else {
					Sc.push_back(storeStrings[j][i][l][tempMinIndex][1]);
					Si.push_back(storeStrings[j][i][l][tempMinIndex][0]);
				}

			}
			else {
				Sc.push_back("");
				Si.push_back("");
			}
			//            if( depth == 1 ) {
			//                cout << "Sc and Si are - " << Sc.at(l) << " " << Si.at(l) << endl;
			//            }
		}

		//        now I have Sc and Si, now I have to find the global alignment

		if (tempMinIndex != 0) {
			//            cout << "\nPushing Sc0 " << Sc[0] << endl;
			globalAlignment.push_back(Sc[0]);
		}
		else {
			//            cout << "\nPushing Sc1 " << Sc[1] << endl;
			globalAlignment.push_back(Sc[1]);
		}

		for (int i = 0; i < stringNumber; i++) {
			if (tempMinIndex != i) {
				int max = ((globalAlignment[0]).size() > (Sc[i]).size() ? (globalAlignment[0]).size() : (Sc[i]).size());
				int j = 0;
				while (j != max) {

					max = ((globalAlignment[0]).size() > (Sc[i]).size() ? (globalAlignment[0]).size() : (Sc[i]).size());

					if (j >= (globalAlignment[0]).size()) {
						//                        cout << "1.Adding hyphen\n";
						addHyphen(j);
					}
					else if (j >= (Sc[i]).size()) {
						(Sc[i]).insert(Sc[i].begin() + j, '-');
						(Si[i]).insert(Si[i].begin() + j, '-');
					}

					if ((globalAlignment[0]).at(j) != (Sc[i]).at(j)) {
						if ((globalAlignment[0]).at(j) == '-') {
							(Sc[i]).insert(Sc[i].begin() + j, '-');
							(Si[i]).insert(Si[i].begin() + j, '-');
						}
						else if ((Sc[i]).at(j) == '-') {
							//                            cout << "2.Adding hyphen\n";
							addHyphen(j);
						}
					}
					j++;
				}
				//                cout << "Pushing Si " << Si[i] << endl;
				globalAlignment.push_back(Si[i]);
			}
		}

		//        if( depth == 3 ) {
		//            cout << "Final alignment by Center Star\n";
		//            for_each( globalAlignment.begin(), globalAlignment.end(), [](string s) {
		//                cout << s << endl;
		//            });
		//        }

		//        now I have a full alignment, I have to calculate the cost now which will be my estimate

		string temp;
		for (int i = 0; i < (globalAlignment[0]).size(); i++) {
			temp.clear();
			for (int j = 0; j < globalAlignment.size(); j++) {
				temp.push_back(globalAlignment[j][i]);
			}

			//            cout << "\nSending this to findCost " << temp << " ";
			//            vector<int> temp1 = getStringInts(temp);
			//            for_each( temp1.begin(), temp1.end(), [](int i){cout << i;});

			heuristicMinCost += findCost(getStringInts(temp));
		}

		if (minCost > heuristicMinCost + costSoFar) {

			cout << "String so far\n";
			for_each(stringsSoFar.begin(), stringsSoFar.end(), [](string s) {
				cout << s << endl;
				});

			cout << "Final alignment by Center Star\n";
			for_each(globalAlignment.begin(), globalAlignment.end(), [](string s) {
				cout << s << endl;
				});

			minCost = heuristicMinCost + costSoFar;
			cout << "\n\nCenter Star found minimum cost goal - " << minCost;
			state tempGoal;
			string temp;
			for (int i = 0; i < stringNumber; i++) {

				if (i == tempMinIndex) {
					temp = *(new string(stringsSoFar[i]));
					(tempGoal.stringsSoFar).push_back(temp.append(globalAlignment[0]));
				}
				else if (i < tempMinIndex) {
					temp = *(new string(stringsSoFar[i]));
					(tempGoal.stringsSoFar).push_back(temp.append(globalAlignment[i + 1]));
				}
				else {
					temp = *(new string(stringsSoFar[i]));
					(tempGoal.stringsSoFar).push_back(temp.append(globalAlignment[i]));
				}

			}
			tempGoal.costSoFar = minCost;
			tempGoal.discoveredAt = timeTaken;
			temp.clear();
			tempGoal.viewStateInfo();
			minState = new state(tempGoal);
		}
		//        ideally, it should be divided by 2
		heuristicMinCost /= divideFactor;

		//        if( depth == 3 )
		//            cout << "\nHeuristic min cost - " << heuristicMinCost << endl;

		delete[] stringCostSum;

		Sc.clear();
		Si.clear();
		globalAlignment.clear();
		temp.clear();
		//cout << "Finding h done\n";fflush(stdout);
		//        if( depth == 1 )
		//            cout << "heursitc value - " << heuristicMinCost;
	}


	//    will return a vector of all next states
	vector<state> exploreStates() {
		tempStates.clear();

		//        setup all the result of edit distances so that it will be used by its children

		//        the string who comes before the other will be at index 0
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < stringNumber; k++) {
					if ((strings.at(k).size()) < this->startingIndex.at(k) + i)
						continue;

					for (int l = k + 1; l < stringNumber; l++) {
						if ((strings.at(l).size()) < this->startingIndex.at(l) + j)
							continue;

						//                        cout << "Setting iterators\n";fflush(stdout);
						stringIterator s1Start = strings.at(k).begin() + this->startingIndex.at(k) + i;
						stringIterator s2Start = strings.at(l).begin() + this->startingIndex.at(l) + j;
						stringIterator s1End = strings.at(k).end();
						stringIterator s2End = strings.at(l).end();

						intIterator i1Start = stringInts.at(k).begin() + this->startingIndex.at(k) + i;
						intIterator i2Start = stringInts.at(l).begin() + this->startingIndex.at(l) + j;
						intIterator i1End = stringInts.at(k).end();
						intIterator i2End = stringInts.at(l).end();

						string* s1 = new string(s1Start, s1End);
						string* s2 = new string(s2Start, s2End);
						vector<int> v1(i1Start, i1End);
						vector<int> v2(i2Start, i2End);

						//                        if( depth == 1 )
						//                            cout << "\nStrings for editDistance " << *s1 << " " << *s2;


						//                        cout << "About to call edit distance\n";fflush(stdout);
						float temp = editDistance(*s1, *s2, v1, v2, depth);

						storeStrings[i][j][k][l][0] = s3;
						storeStrings[i][j][k][l][1] = s4;
						storeCosts[i][j][k][l] = temp;

						//                        cout << "Pushing at ijkl0 - " << i << " " << j << " " << k << " " << l << " 0 - " << s3 << endl;
						//                        cout << "Pushing at ijkl1 - " << i << " " << j << " " << k << " " << l << " 1 - " << s4 << endl;

						delete s1;
						delete s2;
						v1.clear();
						v2.clear();
						//                        cout << "Doing";
					}
				}

			}
		}


		getState(0);

		//        to remove the last entry which indicates inserting hyphens for each string which is completely useless
		//        remove the last entry only if the number of hyphens there are equal to number of strings
		if (tempStates.size() > 1 && tempStates.back().hyphens == stringNumber)
			tempStates.pop_back();

		//        apply heuristic and sort appropriately and return appropriate vector

		//        sort according to heuristic
		sort(tempStates.begin(), tempStates.end(), [&](state s1, state s2) {
			//            higher the heuristic value returned, lower the desirability of the state
			//            sooner the state will be enstacked and later will be popped
			if (s1.costIncurred == 0)
				return false;
			else if (s2.costIncurred == 0)
				return true;
			else if (s1.heuristicMinCost + s1.costIncurred > s2.heuristicMinCost + s2.costIncurred)
				return true;
			else
				return false;
			});

		float minEstimate = 0;
		if (tempStates.size() > 1) {
			minEstimate = (tempStates.back()).heuristicMinCost + (tempStates.back()).costIncurred;

			//            attempt to prune worse nodes
			//            while( true ) {
			//                if( minEstimate + 10 < (tempStates.at(0)).costIncurred + (tempStates.at(0)).heuristicMinCost ) {
			////                    cout << " Pruning nodes";
			//                    tempStates.erase( tempStates.begin() );
			//                } else
			//                    break;
			//            }
		}
		return tempStates;
	}

	bool isGoal() {

		//        if the startingIndex of any string in the vector startingIndex is not its end
		//        then this state is not a goal state
		for (int i = 0; i < stringNumber; i++) {
			if (startingIndex.at(i) < strings.at(i).size())
				return false;
		}
		return true;
	}

	void viewStateInfo() {
		endt = clock();
		cout << "\n\nStarting index of each string - ";
		for_each(startingIndex.begin(), startingIndex.end(), [](int i) {cout << i; });
		cout << "\nCost to come to this state from the previous state - " << costIncurred;
		cout << "\nTotal cost                                         - " << costSoFar;
		cout << "\nDepth - " << depth;
		cout << "\nDivide factor - " << divideFactor;
		cout << "\nHeuristic min estimated cost of this state - " << heuristicMinCost;
		cout << "\nGoal cost till now - " << minCost;
		cout << "\nNodes processed till now - " << statesProcessed;
		cout << "\nTime this node was discovered - " << discoveredAt;
		cout << "\nCurrent time - " << timeTaken;
		cout << "\nMaximum time - " << ttime;
		cout << "\nString upto this state including this state - \n";
		for_each(stringsSoFar.begin(), stringsSoFar.end(), [](string s) {cout << s << endl; });

	}

}startState, currentState;

//计算分数


int calculate_score(int valNum, string s1, string s2) {
	int l = s1.length();
	int r = s2.length();
	if (l > r)
		for (int i = 0; i < l - r; i++)
			s2.append("-");
	else if (r > l)
		for (int i = 0; i < r - l; i++)
			s1.append("-");

	vector<int> string_int_1 = getStringInts(s1);
	vector<int> string_int_2 = getStringInts(s2);
	int score = 0;
	int length = string_int_1.size();
	for (int i = 0; i < length; i++)
		score += MC[string_int_1[i]][string_int_2[i]];

	return score;
}
// input is the set of indices of vocab which represents character of string


void read(string fstring, int stringNumber) {
	ifstream ifs;
	ifs.open(fstring, ios::in);

	if (!ifs.is_open())
	{
		cout << "文件打开失败" << endl;
		exit(0);
	}
	//第三种
	for (int i = 0; i < stringNumber; i++) {
		string buf;
		if (getline(ifs, buf))
		{
			transform(buf.begin(), buf.end(), buf.begin(), ::toupper);
			strings.push_back(buf);
			currentState.startingIndex.push_back(0);
			currentState.stringsSoFar.push_back(*(new string()));
		}
		else
		{
			cout << "读取错误";
			exit(0);
		}

	}


}
void inputMC(int vocabNumber) {
	cout << "矩阵赋值" << endl;
	for (int i = 0; i <= vocabNumber; i++) {
		vector<int> temp1;
		for (int j = 0; j <= vocabNumber; j++) {
			int temp;
			cin >> temp;
			temp1.push_back(temp);
		}
		MC.push_back(temp1);
		temp1.clear();
	}
}

extern "C"{
	string findCentre(string filename,int stringNumber) {
		//    check file input format
		read(filename, stringNumber);
		//int matrix[STRINGNUMBER][STRINGNUMBER];
		int max = 0;
		string center=strings[0];
		//inputMC(4);
		/*for_each(strings.begin(), strings.end(), [&](string s) {
			int temp = 0;
			for_each(strings.begin(), strings.end(), [&](string ss) {
				temp += calculate_score(4, s, ss);
				});
			if (temp > max) {
				max = temp, center.assign(s);
			}
			});*/
		return center;
	}
}


extern "C"{
	string alignSequ(vector<string> seqs, string originalSequence) {

		vector<int> gap;
		int gap_count = originalSequence.length() + 1;
		for (size_t i = 0; i < gap_count; i++)
		{
			gap.push_back(0);
		}
		for_each(seqs.begin(), seqs.end(), [&](string s) {
			string target_string = s;
			int original = 0, target = 0, gap_num = 0, gap_code = 0;
			int target_lenth = target_string.length();
			while (target < target_lenth) {
				if (gap_code == gap_count - 1) {
					if (target != target_lenth)
					{
						gap_num++; target++;
					}

					else
					{
						if (gap.at(gap_code) < gap_num)
							gap[gap_code] = gap_num;
					}
				}
				else {

					if (target_string.at(target) == originalSequence.at(original)) {
						if (gap.at(gap_code) < gap_num)
							gap[gap_code] = gap_num;
						original++;
						target++;
						gap_code++;
						gap_num = 0;
					}
					else
					{
						gap_num++;
						target++;
					}

				}
			}
			});
		for (int i = gap.size() - 1; i >= 0; i--) {
			int len = gap.back();
			//if (len>100)
			originalSequence.insert(i,len , '-');
			gap.pop_back();
		}
		return originalSequence;
	}
}







extern "C" {
	double* main_prog(double* distances,int stringNumber) {
		string center = findCentre("DNA.txt", stringNumber);
		int length = strings.size();
		vector<string> centers;
		vector<string> sequences;
		vector<string> sequences_2;
		int centre_idx = 0;
		for (int i = 0; i < length; i++) {
			if (strings[i] != center)
			{
				vector <string>result = comp(center, strings[i]);
				centers.push_back(result.front());
				sequences.push_back(result.back());
			}
			else
				centre_idx = i;//找出中心序列的下标
		}
		center = alignSequ(centers, center);
		//cout << center + '\n';
		length = sequences.size();

		for (int i = 0; i < length; i++) {
			if (sequences[i] != center)
			{
				vector <string>result = comp(center, sequences[i]);
				sequences_2.push_back(result.back());
			}
		}
		sequences_2.insert(sequences_2.begin() + centre_idx, center );

		/*cout << "对比后DNA序列" << endl;
		for_each(sequences_2.begin(), sequences_2.end(), [](string s) {cout << s << endl; });*/
		length = sequences_2.size();
		cout << length << endl;
		int temp = 0;
		for (int i = 0; i < length; i++) {
			cout << "序列" << i << ":\t";
			for (int j = 0; j < i; j++) {
				cout << calculate_score(4, sequences_2[i], sequences_2[j]) << "\t";
				distances[temp] = calculate_score(4, sequences_2[i], sequences_2[j]);
				temp++;
			}
			cout << endl;
		}
		//for(int i=0;i<10;i++) cout<<distances[i]<<"\t";
		return distances;
	}
}

extern "C" {
	double* main_prog_2(double* distances,int stringNumber) {
		string center = findCentre("DNA.txt",stringNumber);
		int length = strings.size();
		vector<string> centers;
		vector<string> sequences;
		vector<string> sequences_2;
		int centre_idx = 0;
		for (int i = 0; i < length; i++) {
			if (strings[i] != center)
			{
				vector <string>result = compare_two(center, strings[i]);
				centers.push_back(result.front());
				sequences.push_back(result.back());
			}
			else
				centre_idx = i;//找出中心序列的下标
		}
		center = alignSequ(centers, center);
		cout << center + '\n';
		length = sequences.size();

		for (int i = 0; i < length; i++) {
			if (sequences[i] != center)
			{
				vector <string>result = compare_two(center, sequences[i]);
				sequences_2.push_back(result.back());
			}
		}
		sequences_2.insert(sequences_2.begin() + centre_idx, center);

		//cout << "对比后DNA序列" << endl;
		//for_each(sequences_2.begin(), sequences_2.end(), [](string s) {cout << s << endl; });
		length = sequences_2.size();
		cout << length << endl;
		int temp = 0;
		for (int i = 0; i < length; i++) {
			cout << "序列" << i << ":\t";
			for (int j = 0; j < i; j++) {
				cout << calculate_score(4, sequences_2[i], sequences_2[j]) << "\t";
				distances[temp] = calculate_score(4, sequences_2[i], sequences_2[j]);
				temp++;
			}
			cout << endl;
		}
		//for(int i=0;i<10;i++) cout<<distances[i]<<"\t";
		return distances;
	}
}

extern "C" {
	double* calculate_distance_list(double* distances,string sequences_2[]) {
		int length = sequences_2->length();
		cout << length << endl;
		int temp = 0;
		for (int i = 0; i < length; i++) {
			//cout << "序列" << i << ":\t";
			for (int j = 0; j < i; j++) {
				//cout << calculate_score(4, sequences_2[i], sequences_2[j]) << "\t";
				distances[temp] = calculate_score(4, sequences_2[i], sequences_2[j]);
				temp++;
			}
			cout << endl;
		}
		//for(int i=0;i<10;i++) cout<<distances[i]<<"\t";
		return distances;
	}
}

extern "C" {
	int func(int a)
	{
		return a * a;
	}
}
//int main()
//{
//	//auto start = chrono::high_resolution_clock::now();
//	//utsname names;
//	//if (uname(&names) != 0) {
//	//	std::perror("cannot get unames");
//	//}
//
//	//std::cout << "Linux kernel version: " << names.release << std::endl;
//	for (int i = 0; i < 10; i++)
//		std::cout << "HW" << endl;
//}
extern "C" {
	void calculate(char* s1,char* s2, char* result_seq,char* result_centre) {
		vector<string>  result = comp(s1, s2);
		strcpy(result_seq, result[0].data());
		strcpy(result_centre, result[1].data());
	}
}


extern "C" {
	int findResult(char* filename, char* centre,int stringNumber) {
		strcpy(centre, findCentre(filename, stringNumber).data());
		for (int i = 0; i < stringNumber; i++) {
			if (strings[i] == centre)
			{
				return i;
			}

		}
	}
}
extern "C" {
	int main1() {
		string center = findCentre("DNA.txt", 3);
		compare_two(strings.at(0), strings.at(1));
	}
}

extern "C" {
	void align_py(char* seqs[], char* original,char* result,int stringNum) {
		vector<string> input;
		for (int i = 0; i < stringNum; i++) {
			input.push_back(seqs[i]);
		}
		strcat(result, alignSequ(input, original).data());
	}
}

extern "C" {
	void construct_matrix(char* seqs[],double* distances,int stringNum) {
		vector<string> sequences_2;
		for (int i = 0; i < stringNum; i++) {
			sequences_2.push_back(seqs[i]);
		}
		int length = sequences_2.size();
		cout << length << endl;
		int temp = 0;
		for (int i = 0; i < length; i++) {
			cout << "序列" << i << ":\t";
			for (int j = 0; j < i; j++) {
				//cout << calculate_score(4, sequences_2[i], sequences_2[j]) << "\t";
				distances[temp] = (double)calculate_score(4, sequences_2[i], sequences_2[j]);
				temp++;
			}
			cout << endl;
		}
		//for(int i=0;i<10;i++) cout<<distances[i]<<"\t";
	}
}



int main() {
	/*char a[5000000];
	char b[5000000];
	
	cout << a << endl;
	cout << b;*/
	double res[2000];
	main_prog(res,3);
}