#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>

using namespace std;

#define GRAPHS pair<unordered_map<int, int>, unordered_map<int, int>> 

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
    unordered_map<int, int> Gemail; //if there is an edge from 1->2 then Gemail[12]=1
    unordered_map<int, int> Gphone;
    bool nextGraph = false;
    while (inFile >> x)
    {
        num = int(x)*10;
        inFile>>x;
        num = num +x;
        if (num == 0)
            nextGraph = true;
        if (!nextGraph) //first fill larger graph
        {
            Gphone[num] = 1;
        }
        else
        {
            Gemail[num] = 1;
        }
    }
    inFile.close();

    return make_pair(Gphone, Gemail);
}


void printMap(unordered_map<int, int> graph)
{
    for(auto x:graph)
    {
        cout << x.first << " " << x.second << endl;
    }
}

int main(int argc, char const *argv[])
{
    GRAPHS infos;

    infos = readFile("testcase.txt");
    printMap(infos.first);

    return 0;
}
