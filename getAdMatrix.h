#ifndef GETADMATRIX_H
#define GETADMATRIX_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

#define GRAPHS pair<vector<vector<bool>>, vector<vector<bool>>> 

struct Edge
{   
    int parent;
    int child;
};

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
    Edge num;                   //the key
    vector<Edge> edgs;
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

        num.parent = x;
        inFile>>x;
        num.child = x;

        if (num.parent == 0 && num.child == 0)
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
    for (i = 0; i < edgs.size() && edgs[i].parent!=0; i++)
    {
        Gphone[edgs[i].parent-1][edgs[i].child-1] = true;
    }
    i++;
    for (;i < edgs.size() && edgs[i].parent!=0; i++)
    {
        Gemail[edgs[i].parent-1][edgs[i].child-1] = true;
    }

    return make_pair(Gphone, Gemail);
}


void printMatrix(vector<vector<bool>> graph)
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


#endif