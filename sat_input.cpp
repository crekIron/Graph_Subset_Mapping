#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>

using namespace std;

vector<vector<bool> > g_mail;  //size n_mail square
vector<vector<bool> > g_phone;	//size m_phone square

int n=3;  //size of g_mail    value_set
int m=4;	//size of g_phone  value_set

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
		return to_string((n*n) + ((i-1)*n)+j);
	}
}

string get_ind_match(int match_numb){
	return to_string((n*n)+(m*m)+(match_numb)+1);	//if match_numb is the index
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

string match_clauses(int match_numb, vector<vector<bool> > g_mail, vector<int> all_matches){
	string clause = "";
	unordered_map<int, int> mapping;
	int index = n;
	int numb = all_matches[match_numb];
	while(numb>0 && index>0){
		mapping.insert({index, (numb%10)});
		numb = numb/10;
		index--;
	}
	// unordered_map<int, int> mapping = get_hashmap(all_matches[match_numb]);
	for(int i=1; i<=n; i++){
		for(int j=1 ;j<=n; j++){
			if(i!=j){
				clause = clause + "-" +  get_ind_match(match_numb) + " " ;
				if(g_mail[i-1][j-1]){
					clause = clause + "-" + get_ind_edge(1,i,j) + " " + get_ind_edge(2, mapping.at(i), mapping.at(j)) + " 0\n";
					// clause = clause + "-" + to_string(i) + "->" + to_string(j) + " " + to_string(mapping.at(i)) + "->" + to_string(mapping.at(j)) + "\n";
				}
				else{
					clause = clause + get_ind_edge(1,i,j) + " -" + get_ind_edge(2, mapping.at(i), mapping.at(j)) + " 0\n";	
					// clause = clause + to_string(i) + "->" + to_string(j) + " -" + to_string(mapping.at(i)) + "->" + to_string(mapping.at(j)) + "\n";
				}
			}
		}
	}

	return clause;
}

string get_all_clauses(vector<vector<bool> > g_mail, vector<vector<bool> > g_phone, vector<int> all_matches){
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
	cout<<clause<<endl;

	for(int i=1; i<=m; i++){
		for(int j=1; j<=m; j++){
			// cout<<get_ind_edge(1,i,j)<<endl;
			if(g_phone[i-1][j-1])
				clause = clause + get_ind_edge(2,i,j) + " 0\n";
			else
				clause = clause + "-" + get_ind_edge(2,i,j) + " 0\n";
		}
	}
	cout<<clause<<endl;
	for(int i=0; i<all_matches.size(); i++){
		clause = clause + match_clauses(i, g_mail, all_matches) +"\n";
	}
	return clause;
}


vector<int> get_all_matches(int n, vector<int> vect){		//initially 
	vector<int> ans;
	if(n==1){
		ans = vect;
		return ans;
	}
	cout<<n<<endl;
	vector<int> temp = vect;
	vector<int> remain;
	int ten = 1;
	for(int i=0; i<n-1; i++){
		ten = ten*10;
	}

	for(int i=0; i<vect.size(); i++){
		temp = vect;
		remove(vect[i], temp);
		remain = get_all_matches(n-1,temp);
		for(int j=0; j<remain.size(); j++){
			ans.push_back((vect[i]*ten) + remain[j]);
			// cout<<(i*ten) + remain[j]<<endl;
		}
	}

	return ans;
}

int main(){

	vector<int> input;
	vector<int> all_matches;
	vector<vector<bool> > temp1{{false, true, false},		//value_set
								{false, false, false},
								{false, true, false}};

	vector<vector<bool> > temp2{{false, true, true, true},		//value_set
								{false, false, false, false},
								{false, true, false, false},
								{false, true, false, false}};

	// g_mail = temp1;
	// g_phone = temp2;

	for(int i=0; i<m; i++){
		input.push_back(i+1);		
	}

	all_matches = get_all_matches(3,input);

	// for(int i=0; i<all_matches.size(); i++){
	// 	cout<<all_matches[i]<<endl;
	// }
	cout<<"size of all_matches: "<<all_matches.size()<<endl;

	string ans = get_all_clauses(temp1, temp2, all_matches);

	cout<<ans;

	return 0;
}



