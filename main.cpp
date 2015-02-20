#include <stdexcept>
#include <iostream>
#include <fstream>
#include <ctime>
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

        TagBoard tag;

        fin >> tag;

        std::time_t beginTime = std::time(0);

        FinderTagGame finder(tag);
        FinderTagGame::TagMoveList moves = finder.getMoveList();

        std::time_t endTime = std::time(0);

        for(FinderTagGame::TagMoveList::iterator it = moves.begin();
            it != moves.end(); it++)
        {
            cout << *it <<endl;
        }

        cout << "Moves count: " << moves.size() <<endl;

        cout << "elapsed time: " << endTime - beginTime;
    }
    catch (const std::logic_error& e)
    {
        cout << e.what();
    }
    return 0;
}

