#include <stdexcept>
#include <iostream>
#include <fstream>
#include <chrono>
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

        auto beginTime = std::chrono::high_resolution_clock::now();

        FinderTagGame finder(tag);
        FinderTagGame::TagMoveList moves = finder.getMoveList();

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsedTime = endTime - beginTime;

        for(FinderTagGame::TagMoveList::iterator it = moves.begin();
            it != moves.end(); it++)
        {
            cout << *it <<endl;
        }

        cout << "Moves count: " << moves.size() <<endl;

        cout << "elapsed time: " << elapsedTime.count() << "ms" << std::endl;
    }
    catch (const std::logic_error& e)
    {
        cout << e.what();
    }
    return 0;
}

