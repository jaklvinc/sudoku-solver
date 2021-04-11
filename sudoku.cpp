#include "sudoku.hpp"

#include <queue>
#include <vector>
#include <iostream>
#include <fstream>

#define SIZE 9

using namespace std;


CSudoku::CSudoku ( vector<vector<int>> sudoku )
{
    m_Grid=sudoku;
}

bool CSudoku::operator != ( const CSudoku & cmp)
{
    return this->m_Grid != cmp.m_Grid;
}

///Finds all the empty spaces that the Solve method needs to calculate.
void CSudoku::FindEmptySpaces ( void )
{
    ///Goes through the grid and finds all spaces that have zero
    for ( int x = 0 ; x < SIZE ; x++ )
    {
        for ( int y = 0 ; y < SIZE ; y++ )
        {
            if (this->m_Grid[x][y] == 0)
            {
                ///Pushes x ,y coordinates to our queue of coordinates, that needs to be solved
                m_EmptySpaces.push(make_pair(x,y));
            }
        }
    }
    m_EmptyFound = true;
    return;
}

/**
 * Checks if the number is safe in the specified row. 
 * 
 * @param row The number of the row that needs to be searched.
 * @param num The number to check.
 * @return if the number is safe in the row.
 */
bool CSudoku::IsSafeInRow ( int row , int num ) const
{
    for ( int x = 0 ; x < SIZE ; x++)
    {
        if ( this->m_Grid[row][x] == num )
            return false;
    }
    return true;
}

/**
 * Checks if the number is safe in the specified Column. 
 * 
 * @param col The number of the column that needs to be searched.
 * @param num The number to check.
 * @return if the number is safe in the column.
 */
bool CSudoku::IsSafeInColumn ( int col , int num  ) const
{
    for ( int y = 0 ; y < SIZE ; y++)
    {
        if ( this->m_Grid[y][col] == num )
            return false;
    }
    return true;
}

/**
 * Checks if the number is safe in the specified 3x3 square. 
 * 
 * @param row The number of the starting row of the square to search.
 * @param col The number of the starting column of the square to search.
 * @param num The number to check.
 * @return if the number is safe in the square.
 */
bool CSudoku::IsSafeInSqare ( int row , int col , int num ) const
{
    for ( int x = 0 ; x < SIZE/3 ; x++ )
    {
        for ( int y = 0 ; y < SIZE/3 ; y++ )
        {
            if ( this->m_Grid[row+x][col+y] == num )
                return false;
        }
    }
    return true;
}

/**
 * Wrapper method that check if the number can be placed in the place we want.
 * 
 * @param row The number of the row to place the number in.
 * @param col The number of the column to place te number in. 
 * @param num The number to check.
 * @return if the number can be placed in specified coordinates.
 */
bool CSudoku::CanPlace ( int row , int col , int num ) const
{
    return  IsSafeInRow(row,num) && 
            IsSafeInColumn(col,num) &&
            ///Passes the starting row and column of the right square.
            IsSafeInSqare( ( (row / 3 ) * 3 ) , ((col / 3 ) * 3) , num );
}

/**
 * Recursive bactracking algorithm that solves the sudoku.
 * 
 * @return whether the tried number fits in the right place
 */
bool CSudoku::Solve ( void )
{
    int row , col = 0;
    ///Only gets called once at the beggining of the algorithm
    if ( !m_EmptyFound )
        FindEmptySpaces();
    
    ///Returns true if there are no other spaces to fill
    if(m_EmptySpaces.empty())
        return true;
    else
    {
        pair<int,int> Index = m_EmptySpaces.front();
        m_EmptySpaces.pop();
        ///Row coordinate to try new number in
        row = Index.first;
        ///Column coordinate to try the new number in
        col = Index.second; 
    }
    
    ///Tries all numbers from 1 to 9
    for ( int i = 1 ; i <= SIZE ; i++ )
    {
        if ( CanPlace( row , col , i ) )
        {
            m_Grid[row][col] = i;

            ///If the number doesnt return right values in the full recursion, set the coordinates to 0
            if ( Solve () )
                return true;
            else
            {
                m_Grid[row][col] = 0;
            }
        }
        
    }
    //if neither number can be put in because there was a mistake higher in the recursion, the coordinates get pushed back
    m_EmptySpaces.push(make_pair(row,col));
    return false;
}


///Prints the grid to std::cout with approrpiate formatting
void CSudoku::Print ( void ) const
{
    for ( int x = 0 ; x < SIZE ; x++ )
    {
        /// Prints lines at every 3 rows for better visibility.
        if ( x % 3 == 0 && x != 0)
            cout << "—————+—————+—————" << endl;
        
        for ( int y = 0 ; y < SIZE ; y++ )
        {
            cout << m_Grid[x][y];
            /// Prints lines at every 3 columns for better visibility.
            if ( y % 3 -2 == 0 && y != 8)
                cout << "|";
            else 
                cout  << " ";
        }
        cout << endl;
    }
    cout << endl;
}

/**
 * Sends the sudoku to the specified stream.
 * 
 * @param output The stream to output the grid to.
 * 
 * @note Outputs in format that can be inputted back in the program.
 * @overload
 */
void CSudoku::Print ( fstream & output ) const
{
    for ( int x = 0 ; x < SIZE ; x++ )
    {        
        for ( int y = 0 ; y < SIZE ; y++ )
        {
            output << m_Grid[x][y];
            output  << " ";
        }
        output << endl;
    }
    output << endl;
}

/**
 * Loads the grid from specified stream.
 * 
 * @param in The stream to read from.
 * @return if the grid could be loaded.
 */
bool CSudoku::Load ( istream & in)
{
    string sIn;
    for ( int y = 0 ; y < SIZE ; y++ )
    {
        if ( !getline(in,sIn))
        {
            return false;
        }
        /// vector of numbers on the "x" coordinates on the specified y coordinate
        vector<int> Xs;
        for ( int x = 0 ; x < SIZE*2 ; x+=2 )
        {
            int atPlace = sIn[x] - '0';
            if ( atPlace < 0 || atPlace > SIZE )
            {
                return false;
            }
            Xs.push_back(atPlace);
        }
        m_Grid.push_back(Xs);
    }
    return true;
}

/**
 * Checks if the number on specified coordinates equals inputted number.
 * 
 * @param x X coordinate of the positon to check.
 * @param y Y coordinate of the position to check.
 * @param guess The number to check.
 * @return if the number on our coordinates matches our guess.
 */
bool CSudoku::CheckOnCoordinates ( const int x , const int y , const int guess ) const
{
    return this->m_Grid[x][y] == guess;
}

/**
 * Changes the value on specified coordinates.
 * 
 * @param x X coordinate of the position to set the new value.
 * @param y Y coordinate of the position to set the new value.
 * @param guess The number to set.
 */
void CSudoku::ChangeCoordinates ( const int x , const int y , const int guess )
{
    this->m_Grid[x][y] = guess;
    return;
}

/**
 * Checks if the specified guess is on x,y coordinates of anoter sudoku and inserts if true.
 * 
 * @param toCheck Instance of CSudoku to check with.
 * @param x X coordinate of the positon to check.
 * @param y Y coordinate of the position to check.
 * @param guess The number to check.
 * @return if the guess corresponds with coordinates of toCheck.
 */
bool CSudoku::CheckAndInsert ( const CSudoku & toCheck , const int x , const int y , const int guess )
{
    if ( !toCheck.CheckOnCoordinates(x,y,guess) )
    {
        return false;
    }
    else
    {
        this->ChangeCoordinates(x,y,guess);
        return true;
    }
    return false;
}

