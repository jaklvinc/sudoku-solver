#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>

using namespace std;

class CSudoku
{
    public:
    CSudoku(){}
    CSudoku( vector<vector<int>> sudoku );
    
    bool Solve ( void );
    void Print ( void ) const;
    void Print ( fstream & output ) const;
    bool Load ( istream & in );
    bool CheckOnCoordinates ( const int x , const int y , const int guess = 0 ) const;
    bool CheckAndInsert ( const CSudoku & toCheck , const int x , const int y , const int guess );
    void ChangeCoordinates ( const int x , const int y , const int guess );

    bool operator != ( const CSudoku & cmp );

    private:
    vector<vector<int>> m_Grid;
    queue<pair<int,int>> m_EmptySpaces;
    bool m_EmptyFound=false;

    void FindEmptySpaces ( void );
    bool IsSafeInRow ( int row , int num ) const;
    bool IsSafeInColumn ( int col , int num  ) const;
    bool IsSafeInSqare ( int row , int col , int num ) const;
    bool CanPlace ( int row , int col , int num ) const;
};