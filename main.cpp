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

        ofstream fout("output.txt");

        for(std::list<TagBoard::Move>::iterator it = res.begin();
            it != res.end(); it++)
        {
            switch(*it)
            {
            case TagBoard::left:
                fout << "Left" << endl;
                break;
            case TagBoard::right:
                fout << "Right" << endl;
                break;
            case TagBoard::top:
                fout << "Top" << endl;
                break;
            case TagBoard::bottom:
                fout << "Bottom" << endl;
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

