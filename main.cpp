#include "sudoku.hpp"

/**
 * Reads and checks if inputted number is in specified range. Repeats until user inputs right number.
 * 
 * @param[in] input Input string the function reads numbers from.
 * @param[out] importOption The option that the user chooeses.
 * @param[in] numberOfOptions Number of options the user can have.
 *      The range of options is: 1 -> numberOfOptions.
 */
void ReadOptions( string & input , int & importOption , const int numberOfOptions )
{
    while ( getline(cin,input) ) 
    {
        if ( input.find_first_not_of("0123456789") != string::npos )
        {
            cout << "Please enter a valid number." << endl;
            continue;
        }
        importOption = stoi(input);
        if ( importOption < 1 || importOption > numberOfOptions )
        {
            cout << "Please enter a valid number." << endl;
        }
        else 
        {
            break;
        }
    }
}

/**
 * Opens file specified by the fileStream.
 * 
 * @param[out] fileStream The filestream, the file should be opened to.
 * @param[in] option Whether the file should be opened to read or write to.
 * 
 * @return if the file could bbe opened.
 */
bool OpenFile ( fstream & fileStream , const string & option )
{
    string fileName;
    /// Reads a line from input
    getline(cin,fileName);
    if ( option == "in" ) 
        fileStream.open(fileName);
    else if ( option == "out" )         
        fileStream.open(fileName,fstream::out);
    if ( !fileStream )
    {
        cout << "Wrong file name or file cannot be opened." << endl;
        return false;
    }
    
    return true;
}

int main( void )
{
    CSudoku sudoku;

    cout << "Welcome to sudoku solver!" << endl;
    cout << "ENTER \"1\" : If you wish to import your sudoku through command line." << endl;
    cout << "ENTER \"2\" : If you wish to import your sudoku from a file." << endl;

    string input;
    int importOption;

    ReadOptions(input,importOption,2);


    if ( importOption == 1 )
    {
        cout << "Input sudoku by rows with any character between the numbers:" << endl;
        if ( ! sudoku.Load(cin) )
        {
            cout << "Sudoku not imported correctly." << endl;
            return 1;
        }
        cout << endl << "Your inputted sudoku:" << endl;
        sudoku.Print();
    }
    else 
    {
        cout << "Input the exact name of the file, you want to read from:" << endl;
        fstream fileStream;
        if ( !OpenFile(fileStream,"in"))
        {
            return 1;
        }
        if ( ! sudoku.Load(fileStream) )
        {
            cout << "Sudoku not imported correctly." << endl;
            return 1;
        }
        cout << endl << "Your inputted sudoku:" << endl;
        sudoku.Print();
    }

    CSudoku toSolve = sudoku;
    
    cout << "Solving now!" << endl << endl;
    
    if ( !sudoku.Solve() )
    {
        cout << "Your sudoku doesn't have a solution." << endl;
        return 1;
    }
    cout << "Sudoku solved!" << endl << endl;

    cout << "ENTER \"1\" : If you wish to only show the solution." << endl;
    cout << "ENTER \"2\" : If you wish to export your solved sudoku to a file." << endl;
    cout << "ENTER \"3\" : If you wish to try and solve the sudoku." << endl;

    ReadOptions(input,importOption,3);

    if ( importOption == 1 )
    {
        cout << "Your solved sudoku:" << endl ;
        sudoku.Print();
        return 0;
    }

    if ( importOption == 2 )
    {
        cout << "Input the exact name of the file, you want to output to:" << endl;
        fstream fileStream;
        if ( !OpenFile(fileStream,"out"))
        {
            return false;
        }
        sudoku.Print(fileStream);
        fileStream.close();
        return 0;
    }
    if ( importOption == 3)
    {
        cout << "Input your guesses in this format, where coordinates start from the top left corner:" << endl;
        cout << "\"X-coordinate Y-coordinate YourGuess\"" << endl;
        cout << "For example: \"1 2 5\" is a guess that there is a 5 in first column , second row." << endl;
        
        int livesLeft = 5 ;
        while ( toSolve != sudoku )
        {
            toSolve.Print();
            cout << "Lives left: " << livesLeft << endl;
            string inputGuess;
            getline( cin , inputGuess );
            int x , y , guess = 0;
            y = inputGuess[0] - '0' -1;
            x = inputGuess[2] - '0' -1;
            guess = inputGuess[4] - '0';
            if ( x < 0 || x > 8 || y < 0 || y > 8 || guess < 0 || guess > 9 )
            {
                cout << "Coordinates or guess out of range. Try again." << endl << endl;
                continue;
            }
            if ( !toSolve.CheckOnCoordinates(x,y) )
            {
                cout << "The number on these coordinates is already solved." << endl << endl ;
                continue;
            }

            if ( !toSolve.CheckAndInsert(sudoku,x,y,guess) )
            {
                if ( --livesLeft == 0 )
                {
                    cout << "Wrong answer, you ran out of lives. Here is the right solution:" << endl;
                    sudoku.Print();
                    return 0;
                }
                cout << "Wrong answer, try again." << endl << endl;
                continue;
            }
            else 
            {
                cout << "Good job! Try another one!" << endl << endl ;
            }
        }
        
        cout << "Well done, you have solved the sudoku!" << endl;
        
    }

    return 0;
}
