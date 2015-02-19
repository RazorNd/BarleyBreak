#include <stdexcept>
#include <iostream>
#include <fstream>
#include "tagboard.h"
#include "findertaggame.h"

using namespace std;

int main()
{   
    try
    {
        ifstream fin("input.txt");

        if(!fin.is_open())
        {
            cout << "File not open!";
            return 3;
        }

        FinderTagGame finder(fin);        
        FinderTagGame::TagBoardList boards = finder.getTagBoardList();

        for(FinderTagGame::TagBoardList::iterator it = boards.begin();
            it != boards.end(); it++)
        {
            cout << *it <<endl;
        }
    }
    catch (const std::logic_error& e)
    {
        cout << e.what();
    }
    return 0;
}

