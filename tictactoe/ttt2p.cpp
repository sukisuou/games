#include <iostream>
#include <string>
#include <random>

//players
const char plyX = 'X';
const char plyO = 'O';
char currentply = plyX;
int plyXWins = 0;
int plyOWins = 0;

//board
static char board[3][3] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
};
void clearBoard(){
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            board[i][j] = ' ';
        }
    }
}

//colors
namespace Color{
    const std::string red = "\033[91m";
    const std::string blue = "\033[94m";
    const std::string reset = "\033[0m";
}
std::string currentcolor = Color::red;

//logics
bool checkAvailable(int row, int col){
    if(board[row][col] == ' ') return true;
    return false;
}

bool checkWin(){
    //line
    for(int i = 0; i < 3; ++i){
        if(board[i][0] == currentply && board[i][1] == currentply && board[i][2] == currentply) return true;
        if(board[0][i] == currentply && board[1][i] == currentply && board[2][i] == currentply) return true;
    }
    
    //diagonal
    if((board[0][0] == currentply && board[1][1] == currentply && board[2][2] == currentply) 
    || (board[0][2] == currentply && board[1][1] == currentply && board[2][0] == currentply)) return true;

    return false;
}

bool checkTie(){
    for(auto& row : board){
        for(auto& col : row){
            if(col == ' ') return false;
        }
    }
    return true;
}


//mark position
void markPoint(int row, int col){
    board[row][col] = currentply;
}

//draw board
void drawBoard(){
    int i = 0;
    std::string color;
    std::cout << " " << "   1   2   3\n";
    for(auto& row : board){
        std::cout << (char)('a' + i) << " | ";
        for(auto& col : row){
            color = (col == 'X') ? Color::red : Color::blue;
            std::cout << color << col << Color::reset << " | ";
        }std::cout << '\n';
        ++i;
    }std::cout << '\n';
}

inline void errorMsg(){
    std::cout << "Invalid move, try again.\n\n";
    drawBoard();
}

int main(){
    system("cls");
    drawBoard();
    char loop = 'y';
    while(loop == 'y'){
        bool win;
        while(true){
            //input a move
            std::string move;
            std::cout << currentcolor << currentply << Color::reset << "'s move. (eg: a2/2a)\n\n";
            std::cout << "Position : ";
            std::cin >> move;
            system("cls");
    
            //check input
            if(move.length() != 2){
                errorMsg();
                continue;
            }
    
            //parse input
            int row;
            int col;
            bool repeatDigit = true;
            bool repeatLetter = true;
            for(char c : move){
                if(std::isdigit(c)){
                    col = (c - '0') - 1;
                    repeatDigit = (repeatDigit) ? false : true;
                } 
                else{
                    row = c - 'a';
                    repeatLetter = (repeatLetter) ? false : true;
                }
            }
    
            //check boundary
            if(row > 2 || col > 2 || row < 0 || col < 0 || repeatDigit || repeatLetter){
                errorMsg();
                continue;
            }
    
            //mark if spot is unchecked
            if(checkAvailable(row, col)){
                markPoint(row, col);
            }else{
                errorMsg();
                continue;
            }
    
            //draw the instance
            drawBoard();
            
            //check win
            if(checkWin()){
                win = true;
                break;
            }
            if(checkTie()){
                win = false;
                break;
            }
    
            //swap players
            currentply = (currentply == plyX) ? plyO : plyX;
            currentcolor = (currentcolor == Color::red) ? Color::blue : Color::red;
        }

        //wins and ties
        if(win) std::cout << "Congrats, player " << currentcolor << currentply << Color::reset << " wins!\n";
        else std::cout << "Eh, its a tie.\n";

        //leaderboard counts
        if(currentply == plyX) ++plyXWins;
        else ++plyOWins;

        std::cout << "Wins [" << Color::red << "X" << Color::reset << ": " << plyXWins << 
                    ", " << Color::blue << "O" << Color::reset << ": " << plyOWins << "]\n\n";

        std::cout << "Play again? {y/n} : ";
        std::cin >> loop;

        clearBoard();
        if(loop == 'y'){
            system("cls");
            drawBoard();
        }
    }

    return 0;
}
