#include <iostream>
#include <fstream>
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
	return to_string((n*n)+(m*m)+m+((i-1)*m)+j) ;	//if match_numb is the index
}

string has_mail(int j){
	return to_string((n*n)+(m*m)+j);
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


string only_one_clause_n(int ind){
	string clause = "";
	for(int i=1; i<=m; i++){
		for(int j=i+1; j<=m; j++){
			clause = clause + "-" + get_ind_match(ind,i) + " -" + get_ind_match(ind,j) + " 0\n";
		}
	}
	return clause;
}

string only_one_clause_m(int ind){
	string clause = "";
	for(int i=1; i<=n; i++){
		for(int j=i+1; j<=n; j++){
			clause = clause + "-" + get_ind_match(i,ind) + " -" + get_ind_match(j,ind) + " 0\n";
		}
	}
	return clause;
}



string has_mail_clause(int ind){
	string clause = "";
	for(int i=1; i<=n; i++){
		clause = clause + "-" + get_ind_match(i,ind) + " " + has_mail(ind) + " 0\n";
	}

	clause = clause + "-" + has_mail(ind);
	for(int i=1; i<=n; i++){
		clause = clause + " " + get_ind_match(i,ind);
	}
	clause = clause + " 0\n";
	return clause;
}


string edge_clauses(vector<vector<bool> > g_mail, vector<vector<bool> > g_phone, vector<int> deg_mail, vector<int> deg_phone){

	string clause = "";
	
	int nextj = 0;
	int p = 1;
	for(int i=1; i<=n; i++){
		if (deg_mail[i-1]==0)
		{
			if(p>m){
				return "";
			}
			for(int j=p; j<=m; j++){
				if (deg_phone[j-1]==0)
				{
					clause = clause + get_ind_match(i,j)+ " 0\n";
					p=j+1;
					break;	
				}
			}
		}
	}

	for(int i=1; i<=n; i++){
		if(deg_mail[i-1]==0){
				continue;
			}
		for(int j=1; j<=m; j++){
			if(deg_mail[i-1]>deg_phone[j-1])
				clause = clause + "-"+get_ind_match(i,j)+ " 0\n";
			else{
				bool unsat = false;
				for(int k=1; k<=n; k++){
					if(g_mail[i-1][k-1]){
						bool flag = false;
						for(int l=1; l<=m; l++){
							if(g_phone[j-1][l-1] && deg_mail[k-1]<=deg_phone[l-1]){
								flag = true;
								break;
							}
						}
						if(!flag){
							unsat = true;
							break;
						}
					}
				}

				if(!unsat){
					for(int k=1; k<=n; k++){
						if(g_mail[i-1][k-1]){
							// clause = clause + "-"+get_ind_match(i,j);
							for(int l=1; l<=m; l++){
								if(g_phone[j-1][l-1] && deg_mail[k-1]<=deg_phone[l-1])
									// cout<<i<<"->"<<j<<" then "<<k<<"->"<<l<<endl;
									clause = clause +  "-"+get_ind_match(i,j) + " -" + get_ind_match(k,l) + " -"+get_ind_edge(1,i,k)+" "+get_ind_edge(2,j,l) " 0\n";
							}
						}
					}
					for(int l=1; l<=m; l++){
						if(g_phone[j-1][l-1]){
							clause = clause + "-"+get_ind_match(i,j);
							clause = clause + " -" + has_mail(l-1);
							for(int k=1; k<=n; k++){
								if(g_mail[i-1][k-1] && deg_mail[k-1]<=deg_phone[l-1]){
									clause = clause + " " + get_ind_match(k,l);
								}
							}
							clause = clause + " 0\n";
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
	cout<<clause<<endl;
	// cout << "main Conditions" << endl;
	for(int i=1; i<=n; i++){
		clause = clause + atleast_one_clause(i);		//O(n_m)
	}

	cout<<"atleast one done"<<endl;
	for(int i=1; i<=n; i++){											
		clause = clause + only_one_clause_n(i);			//O(n*m^2)
	}


	// cout<<"atleast one done"<<endl;
	for(int i=1; i<=m; i++){											
		clause = clause + only_one_clause_m(i);			//O(m*n^2)
	}

	for(int i=1; i<=m; i++){
		clause = clause + has_mail_clause(i);	
	}

	cout<<"only one done"<<endl;
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

	GINFO gboth;
	gboth = readFile(filename);
	vector<vector<bool> > g_mail;
	vector<vector<bool> > g_phone;
	vector<int> deg_mail;
	vector<int> deg_phone;
	cout<<"here \n"<<g_mail.size()<<endl<<g_phone.size();

	print_matrix(g_phone);
	print_matrix(g_mail);

	g_phone = gboth.first.first;
	g_mail = gboth.first.second;

	deg_phone = gboth.second.first;
	deg_mail = gboth.second.second;

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

	auto start = std::chrono::system_clock::now();
	//get clauses
	string ans = get_all_clauses(g_mail, g_phone, deg_mail, deg_phone);

	auto end = std::chrono::system_clock::now();
 
	cout<<ans;

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";


	int noOfClauses = 0;
	for (int i = 0; i < ans.length(); i++)
	{
		if(ans[i]=='\n')
		{
			noOfClauses++;
		}
	}
	
	//printing in a file
	int total_vars = n*n + m*m + n*m;
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


