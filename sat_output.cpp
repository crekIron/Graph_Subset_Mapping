#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <chrono> 
#include <ctime> 

using namespace std;

struct Edge
{   
    int parent;
    int child;
};

void remove(int val, vector<int> &v ){
	vector<int>::iterator ptr; 
	for (ptr = v.begin(); ptr < v.end(); ptr++){
		if((*ptr)==val){
			v.erase(ptr);
			break;
		}
	} 
	return ;
}
int get_match(int *match, int n, vector<int> vect){		//initially 
	int ans;
	if(n==1){
		// cout << "leaf ";
		for (int i = 0; i < vect.size(); i++)
		{
			ans = vect[i];
			// cout << ans << " ";
			*match = *match-1;
			if(*match==0)
			{
				return ans;
			}
		}
		// cout << "match " << *match << endl;
		return ans;
	}
	// cout<<n<<endl;
	vector<int> temp = vect;
	int remain;
	int ten = 1;
	for(int i=0; i<n-1; i++){
		ten = ten*10;
	}

	for(int i=0; i<vect.size(); i++){
		temp = vect;
		remove(vect[i], temp);
		remain = get_match(match, n-1,temp);
		ans = (vect[i]*ten) + remain;
		if(*match==0)
		{
			return ans;
		}
	}

	return ans;
}

void readSatOutput(string filename)
{
	ifstream inFile;
	inFile.open(filename);

	if (!inFile)
	{
		cerr << "Unable to open file datafile.txt" << endl;
		exit(1); // call system to stop
	}

	string success;
	inFile >> success;
	if (success == "UNSAT")
	{
		cout << "0" << endl;
		ofstream outfile;
		outfile.open("test.mapping");

		outfile << "0" << endl;

		outfile.close();
		inFile.close();
	}
	else
	{
		//print the mapping in here
		vector<int> assignments;
		int vars;

		while (inFile>>vars)
		{
			assignments.push_back(int(vars));
		}
		inFile.close();

		// to find n and m
		ifstream GraphInputFile;
		GraphInputFile.open("test.graphs");

		if (!GraphInputFile)
		{
			cerr << "Unable to open file datafile.txt" << endl;
			exit(1); // call system to stop
		}

		int x;
		Edge num;                   //the key
		vector<Edge> edgs;
		bool nextGraph = false;
		int m = 0;
		int n = 0;
		while (GraphInputFile >> x)
		{
			if(!nextGraph)
			{
				if(m<x) m = x;
			}
			else
			{
				if(n<x) n = x;
			}

			num.parent = x;
			GraphInputFile>>x;
			num.child = x;

			if (num.parent == 0 && num.child == 0)
				nextGraph = true;
			if(!nextGraph)
			{
				if(m<x) m = x;
			}
			else
			{
				if(n<x) n = x;
			}
		}
		GraphInputFile.close();
		
		//got n and m in here
		/* 
		now required to find which variable it correspond
		1. get first variable after n^2 + m^2 variables which is true
		2. check assignments for that
		3. and then all_matches alog to find that particular match
		 */
		int correctAns = 0;
		for (int i = n*n+m*m; i < assignments.size(); i++)
		{
			if(assignments[i]>0)
			{
				correctAns = assignments[i];
				break;
			}
		}
		cout << n << " " << m << endl;
		//I got that variable in correct ans
		if (correctAns == 0)
		{
			cout << "some problem in here" << endl;
		}
		
		correctAns = correctAns - n*n - m*m;
		int temp = correctAns;
		int match = 0;
		vector<int> input;

		for(int i=0; i<m; i++){
			input.push_back(i+1);
		}


		match = get_match(&temp, n, input);


		cout << "got the match" << endl;
		cout << match << endl;
		int ten = 1;
		for(int i=0; i<n-1; i++){
			ten = ten*10;
		}
		string ans;
		for (int i = 1; i <=n; i++)
		{
			ans = ans + to_string(i) + " " + to_string(match/ten) + "\n";
			match = match%ten;
			ten = ten/10;
		}

		cout << ans;
		ofstream outfile;
		outfile.open("test.mapping");

		outfile << ans << endl;

		outfile.close();
	}
}

int main(int argc, char const *argv[])
{
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = start - start; 
	readSatOutput("test.satoutput");
	elapsed_seconds = elapsed_seconds + std::chrono::system_clock::now()-start;  
	cout << "elapsed time for reading the output of MiniSat: " << elapsed_seconds.count() << "s\n"; 
	return 0;
}
