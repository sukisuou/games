#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <chrono>

//players
const char plyX = 'X';
const char plyO = 'O';
char currentply = plyX;
int plyXWins = 0;
int plyOWins = 0;
int ties = 0;

//boards
static char board[3][3] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
};
static char testboard[3][3];
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
bool checkTestWin(){
    //line
    for(int i = 0; i < 3; ++i){
        if(testboard[i][0] == currentply && testboard[i][1] == currentply && testboard[i][2] == currentply) return true;
        if(testboard[0][i] == currentply && testboard[1][i] == currentply && testboard[2][i] == currentply) return true;
    }
    
    //diagonal
    if((testboard[0][0] == currentply && testboard[1][1] == currentply && testboard[2][2] == currentply) 
    || (testboard[0][2] == currentply && testboard[1][1] == currentply && testboard[2][0] == currentply)) return true;

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
void markTestPoint(int row, int col){
    testboard[row][col] = currentply;
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
inline void compMsg(){
    std::cout << Color::blue << plyO << Color::reset << " Computer's thinking";
    for(int i = 0; i < 3; ++i){
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << ".";
    }
}

//computer ai
void alg(int& maxScore, int& pickRow, int& pickCol){
    char rows[3] = {'a', 'b', 'c'};
    char cols[3] = {'1', '2', '3'};
    int cellScore[3][3] = {};

    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 3; ++j){
            //copy board to test
            for(int a = 0; a < 3; ++a){
                for(int b = 0; b < 3; ++b){
                    testboard[a][b] = board[a][b];
                }
            }

            int row = rows[i] - 'a';
            int col = cols[j] - '0' - 1;

            if(checkAvailable(row, col)){
                markTestPoint(row, col);
                //check win on test board
                if(checkTestWin()) cellScore[i][j] = 1;
                else cellScore[i][j] = 0;

                //find max score
                if(cellScore[i][j] > maxScore){
                    maxScore = cellScore[i][j];
                    pickRow = i;
                    pickCol = j;
                }
            }else{
                cellScore[i][j] = -1;
            }
        }
    }
}
void aiRandomMove(){
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> gen(0, 2);

    char rows[3] = {'a', 'b', 'c'};
    char cols[3] = {'1', '2', '3'};

    while(true){
        if(checkAvailable(1, 1)){   //prioritize getting middle spot
            markPoint(1, 1);
            break;
        }

        int row = rows[gen(rng)] - 'a';
        int col = cols[gen(rng)] - '0' - 1;
        if(checkAvailable(row, col)){
            markPoint(row, col);
            break;
        }
    }
}
void aiMove(){  
    int maxScore = -1;
    int pickRow = 0;
    int pickCol = 0;
    
    //check possibility for winning
    alg(maxScore, pickRow, pickCol);

    //check possibility for blocking
    if(maxScore == 0){
        maxScore = -1;
        pickRow = 0;
        pickCol = 0;
        currentply = plyX;
        alg(maxScore, pickRow, pickCol);
        currentply = plyO;
        if(maxScore == 0) aiRandomMove();
        else markPoint(pickRow, pickCol);
    }else{
        markPoint(pickRow, pickCol);
    }
}

int main(){
    system("cls");
    drawBoard();
    char loop = 'y';
    while(loop == 'y'){
        bool win;
        while(true){
            //moves
            if(currentply == plyX){
                //input a move
                std::string move;
                std::cout << "Your move, " << currentcolor << currentply << Color::reset << " (eg: a2/2a)\n\n";
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
            }else{
                system("cls");
                aiMove();
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

            if(currentply == plyO) compMsg();
        }

        //wins and ties
        if(win){
            if(currentply == plyX) std::cout << "Congrats, you win!\n";
            else std::cout << "Computer wins!\n";
        }
        else std::cout << "Eh, its a tie.\n";

        //leaderboard counts
        if(win){
            if(currentply == plyX) ++plyXWins;
            else ++plyOWins;
        }else{
            ++ties;
        }
        std::cout << "Wins [" << Color::red << "X" << Color::reset << ": " << plyXWins << 
                    ", " << Color::blue << "O" << Color::reset << ": " << plyOWins << 
                    ", Ties: " << ties << "]\n\n";

        //prompt replay
        std::cout << "Play again? {y/n} : ";
        std::cin >> loop;
        
        //cleanups
        clearBoard();
        if(loop == 'y'){
            system("cls");
            drawBoard();
        }
    }

    return 0;
}
