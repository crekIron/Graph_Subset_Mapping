#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

#define GRAPHS pair<vector<vector<bool>>, vector<vector<bool>>> 

GRAPHS readFile(string filename)
{
    ifstream inFile;
    inFile.open(filename);

    if (!inFile)
    {
        cerr << "Unable to open file datafile.txt" << endl;
        exit(1); // call system to stop
    }

    int x;
    int num = -1;                   //the key
    vector<int> edgs;
    bool nextGraph = false;
    int tvers1 = 0;
    int tvers2 = 0;
    while (inFile >> x)
    {
        if(!nextGraph)
        {
            if(tvers1<x) tvers1 = x;
        }
        else
        {
            if(tvers2<x) tvers2 = x;
        }
        num = int(x)*10;
        inFile>>x;
        num = num +x;
        if (num == 0)
            nextGraph = true;
        if(!nextGraph)
        {
            if(tvers1<x) tvers1 = x;
        }
        else
        {
            if(tvers2<x) tvers2 = x;
        }
        edgs.push_back(num);
    }
    inFile.close();

    vector<vector<bool>> Gemail(tvers2, vector<bool>(tvers2,false)); //if there is an edge from 1->2 then Gemail[12]=1
    vector<vector<bool>> Gphone(tvers1, vector<bool>(tvers1,false));
    int i=0;
    for (i = 0; i < edgs.size() && edgs[i]!=0; i++)
    {
        Gphone[edgs[i]/10-1][edgs[i]%10-1] = true;
    }
    i++;
    for (;i < edgs.size() && edgs[i]!=0; i++)
    {
        Gemail[edgs[i]/10-1][edgs[i]%10-1] = true;
    }

    return make_pair(Gphone, Gemail);
}


void printMap(vector<vector<bool>> graph)
{
    for (int i = 0; i < graph.size(); i++)
    {
        for (int j = 0; j < graph[0].size(); j++)
        {
            if(graph[i][j])
            {
                cout << i+1 << " " << j+1 << endl;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    GRAPHS infos;

    infos = readFile("testcase.txt");
    printMap(infos.second);

    return 0;
}
