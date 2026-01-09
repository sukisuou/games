#include <iostream>
#include <fstream>
#include <string>
#include <random>

namespace Color{
    const std::string reset     = "\033[0m";
    const std::string green     = "\033[42m";
    const std::string yellow    = "\033[43m";
    const std::string red       = "\033[41m";
    const std::string white     = "\033[47m";
}

//game board
static char board[6][5] = {
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' '}
};

//keyboard hints
static char keystate[26] = {' '};

//big title
void title(){
    std::cout << Color::green << " W " 
            << Color::yellow << " O " 
            << Color::white << " R " 
            << Color::green << " D " 
            << Color::yellow << " L " 
            << Color::white << " E "
            << Color::reset << "\t(Type 'x' to exit)\n\n";
}

//checking the positions
void check(std::string& guess, std::string& word, int& n){
    bool used[5] = {false};
    
    //first pass
    for(int i = 0; i < 5; ++i){
        int val = guess[i] - 'a';
        if(guess[i] == word[i]){
            board[n][i] = keystate[val] = 'O';
            used[i] = true;
        }else{
            board[n][i] = 'X';
            if(keystate[val] != 'O' && keystate[val] != '-'){
                keystate[val] = 'X';
            }
        }
    }

    //second pass (check repetition)
    for(int i = 0; i < 5; ++i){
        if(board[n][i] == 'O') continue;
        
        int val = guess[i] - 'a';
        for(int j = 0; j < 5; ++j){
            if(!used[j] && guess[i] == word[j]){
                board[n][i] = '-'; 
                if(keystate[val] != 'O'){
                    keystate[val] = '-';
                }
                used[j] = true;
                break;
            }
        }
    }
}

//drawing 
void drawslot(char letter, int i, int j){
    std::cout << Color::reset << '|';
    switch(board[i][j]){
        case 'O' : 
            std::cout << Color::green << ' ' << letter << ' ';
            break;
        case '-' : 
            std::cout << Color::yellow << ' ' << letter << ' ';
            break;
        case 'X' : 
            std::cout << Color::red << ' ' << letter << ' ';
            break;
        case ' ' : 
            std::cout << "   ";
            break;
    }

    std::cout << Color::reset << '|';
}
void drawboard(std::string guess[]){
    //draw board
    for(int i = 0; i < 6; ++i){
        for(int j = 0; j < 5; ++j){
            drawslot(guess[i][j], i, j);
        }std::cout << '\n';
    }std::cout << '\n';
}
void drawkey(){
    std::string rows[3] = {"qwertyuiop", "asdfghjkl", "zxcvbnm"};

    for(int i = 0; i < 3; ++i){
        std::cout << std::string(i, ' ');
        for(char c : rows[i]){
            int val = c - 'a';
            char state = keystate[val];
            
            switch(state){
                case 'O' : 
                    std::cout << Color::green << ' ' << c << ' ' << Color::reset;
                    break;
                case '-' : 
                    std::cout << Color::yellow << ' ' << c << ' ' << Color::reset;
                    break;
                case 'X' : 
                    std::cout << Color::red << ' ' << c << ' ' << Color::reset;
                    break;
                default  : 
                    std::cout << ' ' << c << ' ';
            }
        }
        std::cout << '\n';
    }std::cout << '\n';
}

void reset(){
    for(int i = 0; i < 6; ++i){
        for(int j = 0; j < 5; ++j){
            board[i][j] = ' ';
        }
    }
    for(int i = 0; i < 26; ++i) keystate[i] = ' ';
}

int main(){
    system("cls");

    //word library
    std::ifstream inputFile("words.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }
    int size = 3103;
    std::string words[size];
    for(int i = 0; i < size && inputFile >> words[i]; ++i){};

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<> gen(0, size - 1);

    //loop
    char conf = 'y';
    while(conf == 'y'){        
        //big title
        title();
        
        int tries = 0;
        std::string guess[6];
        
        //fetch a random word
        std::string word = words[gen(rng)];
        bool win = false;
        
        //game start
        drawboard(guess);
        drawkey();
        while(tries < 6){
            std::cout << "\nYour guess: ";
            std::cin >> guess[tries];

            //exit
            if(guess[tries] == "x") return 1;
            
            //number error
            bool number = false;
            for(char c : guess[tries]){
                if(std::isdigit(c)){
                    std::cout << "Never recalled numbers being in a WORD before.\n\n";
                    number = true;
                }
            }
            if(number) continue;
            
            //more than 5 letters error
            if(guess[tries].length() != 5){
                std::cout << "Please enter a 5 letter word only, you idiot.\n\n";
                continue;
            }
            
            system("cls");
            title();
            
            //check
            check(guess[tries], word, tries);
            
            //draw board
            drawboard(guess);
            
            //draw keyboard
            drawkey();
            
            //win check
            for(int i = 0; i < 5; ++i){
                if(board[tries][i] == 'O'){
                    win = true;
                }else{
                    win = false;
                    break;
                }
            }

            if(win){
                std::cout << "You got the word : " << word << "!";
                if(tries == 0) printf(" (first try)");
                else printf(" (in %d tries)", tries + 1);
                break;
            }

            ++tries;
        }

        if(!win) std::cout << "You failed. The word was " << word;

        std::cout << "\n\nPlay again? {y/n}: ";
        std::cin >> conf;

        reset();
        system("cls");
    }

    return 0;
}