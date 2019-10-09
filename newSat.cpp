#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>
#include "getAdMatrix.h"

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
	return to_string((n*n)+(m*m)+((i-1)*m)+j) ;	//if match_numb is the index
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

// string match_clauses(int match_numb, vector<vector<bool> > g_mail, vector<int> all_matches){
// 	string clause = "";
// 	unordered_map<int, int> mapping;
// 	int index = n;
// 	int numb = all_matches[match_numb];
// 	while(numb>0 && index>0){
// 		mapping.insert({index, (numb%10)});
// 		numb = numb/10;
// 		index--;
// 	}
// 	// unordered_map<int, int> mapping = get_hashmap(all_matches[match_numb]);
// 	for(int i=1; i<=n; i++){
// 		for(int j=1 ;j<=n; j++){
// 			if(i!=j){
// 				clause = clause + "-" +  get_ind_match(match_numb) + " " ;
// 				if(g_mail[i-1][j-1]){
// 					clause = clause + "-" + get_ind_edge(1,i,j) + " " + get_ind_edge(2, mapping.at(i), mapping.at(j)) + " 0\n";
// 					// clause = clause + "-" + to_string(i) + "->" + to_string(j) + " " + to_string(mapping.at(i)) + "->" + to_string(mapping.at(j)) + "\n";
// 				}
// 				else{
// 					clause = clause + get_ind_edge(1,i,j) + " -" + get_ind_edge(2, mapping.at(i), mapping.at(j)) + " 0\n";	
// 					// clause = clause + to_string(i) + "->" + to_string(j) + " -" + to_string(mapping.at(i)) + "->" + to_string(mapping.at(j)) + "\n";
// 				}
// 			}
// 		}
// 	}

// 	return clause;
// }


string atleast_one_clause(int ind){
	string clause = "";
	for(int i=0; i<m; i++){
		clause = clause + get_ind_match(ind,i+1) + " ";
	}
	clause = clause + "0\n";
	return clause;

}


string only_one_clause(int ind){
	string clause = "";
	for(int i=1; i<=m; i++){
		for(int j=i+1; j<=m; j++){
			clause = clause + "-" + get_ind_match(ind,i) + " -" + get_ind_match(ind,j) + " 0\n";
		}
	}
	return clause;
}

string edge_clauses(vector<vector<bool> > g_mail, vector<vector<bool> > g_phone, vector<int> deg_mail, vector<int> deg_phone){

	string clause = "";
	for(int i=1; i<=n; i++){
		for(int j=1; j<=n; j++){
			for(int a=1; a<=m; a++){
				for(int b=1; b<=m; b++){
					if(i!=j){
						if(a!=b){
							if(deg_mail[i-1]>deg_mail[a-1]){
								clause = clause + "-" + get_ind_match(i,a) + " 0\n";
								if(deg_mail[j-1] > deg_mail[b-1])
									clause = clause + "-" + get_ind_match(j,b) + " 0\n";
							}
							else if(deg_mail[j-1] > deg_mail[b-1])
								clause = clause + "-" + get_ind_match(j,b) + " 0\n";
							else if(g_mail[i-1][j-1]){
								// if(g_phone(a-1,b-1)){
								clause = clause + "-" + get_ind_edge(1,i,j) + " -" + get_ind_match(i,a) + " -" + get_ind_match(j,b) + get_ind_edge(2,a,b) +" 0\n";
								// }
							}
							else{
								clause = clause + get_ind_edge(1,i,j) + " -" + get_ind_match(i,a) + " -" + get_ind_match(j,b) + " -" + get_ind_edge(2,a,b) +" 0\n";
							}
						}
					}
				}
			}
		}
	}

	return clause;
}


string get_all_clauses(vector<vector<bool> > g_mail, vector<vector<bool> > g_phone, vector<int> deg_mail, vector<int> deg_phone){
	string clause = "";

	for(int i=1; i<=n; i++){
		for(int j=1; j<=n; j++){
			// cout<<get_ind_edge(1,i,j)<<endl;
			if(g_mail[i-1][j-1])
				clause = clause + get_ind_edge(1,i,j) + " 0\n";
			else
				clause = clause + "-" + get_ind_edge(1,i,j) + " 0\n";
		}
	}
	// cout << "first Graph condition" << endl;
	// cout<<clause<<endl;

	for(int i=1; i<=m; i++){
		for(int j=1; j<=m; j++){
			// cout<<get_ind_edge(1,i,j)<<endl;
			if(g_phone[i-1][j-1])
				clause = clause + get_ind_edge(2,i,j) + " 0\n";
			else
				clause = clause + "-" + get_ind_edge(2,i,j) + " 0\n";
		}
	}
	// cout << "second Graph condition" << endl;
	// cout<<clause<<endl;
	// cout << "main Conditions" << endl;
	for(int i=0; i<n; i++){
		clause = clause + atleast_one_clause(i);		//O(n_m)
	}

	for(int i=0; i<n; i++){											
		clause = clause + only_one_clause(i);			//O(n*m^2)
	}

	clause = clause + edge_clauses(g_mail, g_phone, deg_mail, deg_phone);


	return clause;
}


// vector<int> get_all_matches(int n, vector<int> vect){		//initially 
// 	vector<int> ans;
// 	if(n==1){
// 		ans = vect;
// 		return ans;
// 	}
// 	cout<<n<<endl;
// 	vector<int> temp = vect;
// 	vector<int> remain;
// 	int ten = 1;
// 	for(int i=0; i<n-1; i++){
// 		ten = ten*10;
// 	}

// 	for(int i=0; i<vect.size(); i++){
// 		temp = vect;
// 		remove(vect[i], temp);
// 		remain = get_all_matches(n-1,temp);
// 		for(int j=0; j<remain.size(); j++){
// 			ans.push_back((vect[i]*ten) + remain[j]);
// 			// cout<<(i*ten) + remain[j]<<endl;
// 		}
// 	}

// 	return ans;
// }

void print_matrix(vector<vector<bool> > a){
	for(int i=0; i<a.size(); i++){
		for(int j=0; j<a[i].size(); j++)
			cout<<a[i][j]<<" next";
		cout<<endl;
	}
}

void mainFunction(string filename)
{
	// vector<vector<bool> > temp1{{false, true, false},		//value_set
	// 							{false, false, false},
	// 							{false, true, false}};

	// vector<vector<bool> > temp2{{false, true, true, true},		//value_set
	// 							{false, false, false, false},
	// 							{false, true, false, false},
	// 							{false, true, false, false}};

	GRAPHS gboth;
	gboth = readFile(filename);
	vector<vector<bool> > g_mail;
	vector<vector<bool> > g_phone;
	vector<int> deg_mail;
	vector<int> deg_phone;
	cout<<"here \n"<<g_mail.size()<<endl<<g_phone.size();

	print_matrix(g_phone);
	print_matrix(g_mail);

	g_phone = gboth.first;
	g_mail = gboth.second;

	//assign deg_mail and deg_phone change

	n = g_mail.size();
	m = g_phone.size();

	vector<int> input;
	vector<int> all_matches;

	// for(int i=0; i<m; i++){
	// 	input.push_back(i+1);		
	// }

	// all_matches = get_all_matches(n,input);

	// cout<<"size of all_matches: "<<all_matches.size()<<endl;

	//get clauses
	string ans = get_all_clauses(g_mail, g_phone, deg_mail, deg_phone);

	cout<<ans;

	int noOfClauses = 0;
	for (int i = 0; i < ans.length(); i++)
	{
		if(ans[i]=='\n')
		{
			noOfClauses++;
		}
	}
	
	//printing in a file
	int total_vars = n*n + m*m + all_matches.size();
	ofstream outfile;
	outfile.open("test.satinput");

	outfile << "c Here is a comment." << endl;
	outfile << "p cnf " << total_vars << " " << noOfClauses << endl;
	outfile << ans << endl;

	outfile.close();
}
int main(){

	mainFunction("test.graphs");
	return 0;
}



