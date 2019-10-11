#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <unordered_map>
#include <utility>
#include "getAdMatrixAndDegree.h"

using namespace std;

int n=0;//size of g_mail    value_set
int m=0;//size of g_phone  value_set

// vector<vector<int> > all_matches;   //size mPn * n

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

string get_ind_edge(int graph, int i, int j){
	if(graph==1){
		return to_string(((i-1)*n)+j);
	}
	else{
		return to_string((n*n) + ((i-1)*m)+j);
	}
}

string get_ind_match(int i, int j){
	return to_string(m+((i-1)*m)+j) ;	//if match_numb is the index
	// return to_string((n*n)+(m*m)+m+((i-1)*m)+j) ;
}

string has_mail(int j){
	return to_string(j);
	// return to_string((n*n)+(m*m)+j);

}

unordered_map<int, int> get_hashmap(int numb){
	unordered_map<int, int> mapping;
	int index = n;

	while(numb>0 && index>0){
		mapping.insert({index, (numb%10)});
		numb = numb/10;
		index--;
	}

	return mapping;

}

string atleast_one_clause(int ind, vector<int> v){
	string clause = "";
	for(int i=1; i<=v.size(); i++){
		clause = clause + get_ind_match(ind,v[i-1]) + " ";
	}
	clause = clause + "0\n";
	return clause;

}


string only_one_clause_n(int ind, vector<int> v){
	string clause = "";
	for(int i=1; i<=v.size(); i++){
		for(int j=i+1; j<=v.size(); j++){
			clause = clause + "-" + get_ind_match(ind,v[i-1]) + " -" + get_ind_match(ind,v[j-1]) + " 0\n";
		}
	}
	return clause;
}

string only_one_clause_m(int ind, vector<int> p){
	string clause = "";
	for(int i=1; i<=p.size(); i++){
		for(int j=i+1; j<=p.size(); j++){
			clause = clause + "-" + get_ind_match(p[i-1],ind) + " -" + get_ind_match(p[j-1],ind) + " 0\n";
		}
	}
	return clause;
}



string has_mail_clause(int ind, vector<int> p){
	string hasMail = has_mail(ind);
	string clause1 = "";
	string clause2 = "-" + hasMail;
	string s="";
	for(int i=1; i<=p.size(); i++){
		s = get_ind_match(p[i-1],ind);
		clause1 = clause1 + "-" + s + " " + hasMail + " 0\n";
		clause2 = clause2 + " " + s;
		
	}

	clause2 = clause2 + " 0\n";
	return clause1 + clause2;
}

bool no_incoming(int ind, int graph,vector<vector<bool> > g_mail, vector<vector<bool> > g_phone){
	if(graph==1){
		for(int i=0; i<n; i++){
			if(g_mail[i][ind-1])
				return false;
		}
	}
	else{
		for(int i=0; i<m; i++){
			if(g_phone[i][ind-1])
				return false;
			}
	}
	
	return true;
}


string edge_clauses(vector<vector<bool> > g_mail, vector<vector<bool> > g_phone, vector<int> deg_mail, vector<int> deg_phone, vector<vector<int> > v, vector<vector<int> > w){

	string clause = "";

	int nextj = 0;
	int p = 1;

	for(int i=1; i<=n; i++){
		if((deg_mail[i-1]==0) && no_incoming(i,1, g_mail, g_phone)){
			if(p>m){
				return "";
			}
			for(int j=p; j<=m; j++){
				if ((deg_phone[j-1]==0) && no_incoming(j,2, g_mail, g_phone))
				{
					// cout<<"isolated match: "<<i<<" "<<j<<endl;
					clause = clause + get_ind_match(i,j)+ " 0\n";
					p=j+1;
					break;	
				}
			}
			continue;
		}
		for(int j=0; j<v[i-1].size(); j++){
			for(int k=1; k<=n; k++){
				if(g_mail[i-1][k-1]){
					clause = clause + "-"+get_ind_match(i,v[i-1][j]);
					for(int l=0; l<v[k-1].size(); l++){
						if(g_phone[v[i-1][j]-1][v[k-1][l]-1])
							// cout<<i<<"->"<<j<<" then "<<k<<"->"<<l<<endl;
							clause = clause + " " + get_ind_match(k,v[k-1][l]);
					}
					clause = clause + " 0\n";
				}
			}
			for(int l=1; l<=m; l++){
				if(g_phone[v[i-1][j]-1][l-1]){
					clause = clause + "-"+get_ind_match(i,v[i-1][j]) + " -" + has_mail(l);
					for(int k=0; k<w[l-1].size(); k++){
						if(g_mail[i-1][w[l-1][k]-1]){
							clause = clause + " " + get_ind_match(w[l-1][k],l);
						}
					}
					clause = clause + " 0\n";
				}
			}
				
		}
	}

	return clause;
}


string get_all_clauses(vector<vector<bool> > g_mail, vector<vector<bool> > g_phone, vector<int> deg_mail, vector<int> deg_phone){
	string clause = "";

	vector<vector<int> > possible_matches;
	vector<int> temp;
	for(int i=1; i<=n; i++){
		temp.clear();
		for(int j=1; j<=m; j++){
			if(deg_mail[i-1]<=deg_phone[j-1])
				temp.push_back(j);
			else
				clause = clause + "-"+get_ind_match(i,j)+ " 0\n";
		}
		possible_matches.push_back(temp);
	}

	vector<vector<int> > possible2;
	for(int i=1; i<=m; i++){
		temp.clear();
		for(int j=1; j<=n; j++){
			if(deg_mail[j-1]<=deg_phone[i-1])
				temp.push_back(j);
		}
		possible2.push_back(temp);
	} 


	for(int i=1; i<=n; i++){
		clause = clause + atleast_one_clause(i,possible_matches[i-1]);		//O(n_m)
	}

	// cout<<"atleast one done"<<endl;
	for(int i=1; i<=n; i++){											
		clause = clause + only_one_clause_n(i, possible_matches[i-1]);			//O(n*m^2)
	}


	// cout<<"atleast one done"<<endl;
	for(int i=1; i<=m; i++){											
		clause = clause + only_one_clause_m(i, possible2[i-1]);			//O(m*n^2)
	}

	for(int i=1; i<=m; i++){
		clause = clause + has_mail_clause(i, possible2[i-1]);	
	}

	// cout<<"only one done"<<endl;
	string edgeClauses = edge_clauses(g_mail, g_phone, deg_mail, deg_phone, possible_matches, possible2);
	if(edgeClauses.length()==0)
		clause = "0";
	else clause = clause + edgeClauses;


	return clause;
}

void print_matrix(vector<vector<bool> > a){
	for(int i=0; i<a.size(); i++){
		for(int j=0; j<a[i].size(); j++)
			cout<<a[i][j]<<" " ;
		cout<<endl;
	}
}

void mainFunction(string filename)
{

	GINFO gboth;
	gboth = readFile(filename + ".graphs");
	vector<vector<bool> > g_mail;
	vector<vector<bool> > g_phone;
	vector<int> deg_mail;
	vector<int> deg_phone;
	// cout<<"here \n"<<g_mail.size()<<endl<<g_phone.size();

	g_phone = gboth.first.first;
	g_mail = gboth.first.second;

	deg_phone = gboth.second.first;
	deg_mail = gboth.second.second;

	n = g_mail.size();
	m = g_phone.size();

	vector<int> input;
	vector<int> all_matches;

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = start - start; 
	//get clauses
	string ans = get_all_clauses(g_mail, g_phone, deg_mail, deg_phone);

	elapsed_seconds = elapsed_seconds + std::chrono::system_clock::now()-start;  
	cout << "elapsed time for reading the output of MiniSat: " << elapsed_seconds.count() << "s\n"; 
	int noOfClauses = 0;
	for (int i = 0; i < ans.length(); i++)
	{
		if(ans[i]=='\n')
		{
			noOfClauses++;
		}
	}
	
	//printing in a file
	int total_vars = m + n*m;
	ofstream outfile;
	outfile.open(filename + ".satinput");

	outfile << "c Here is a comment." << endl;
	outfile << "p cnf " << total_vars << " " << noOfClauses << endl;
	outfile << ans << endl;

	outfile.close();
}
int main(int argc, char const *argv[]){
	// string filename = "";
	// cin>>filename;
	// cout<<string(argv[1])<<endl;
	mainFunction(string(argv[1]));
	
	return 0;
}



