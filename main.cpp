#include <stdexcept>
#include <iostream>
#include <fstream>
#include <tagboard.h>
#include <set>
#include "findertaggame.h"

using namespace std;

int main()
{
    multiset<int> intSet();
    try
    {
        ifstream fin("input.txt");

        if(!fin.is_open())
        {
            cout << "File not open!";
            return 3;
        }

        FinderTagGame finder(fin);
        std::list<TagBoard::Move> res = finder.getMoveList();

        for(std::list<TagBoard::Move>::iterator it = res.begin();
            it != res.end(); it++)
        {
            switch(*it)
            {
            case TagBoard::left:
                cout << "Left" << endl;
                break;
            case TagBoard::right:
                cout << "Right" << endl;
                break;
            case TagBoard::top:
                cout << "Top" << endl;
                break;
            case TagBoard::bottom:
                cout << "Bottom" << endl;
                break;
            }
        }
    }
    catch (const std::logic_error& e)
    {
        cout << e.what();
    }
    return 0;
}

