#include <iostream>
#include <string>
#include <cstring>
#include <random>
#include "../../Color.h"
#include <Windows.h>

//randomizer
static std::mt19937 rng(std::random_device{}());

//ref
const std::string shape[4] = {
    Color::bg_black + " \u2664",    //spade
    Color::bg_red   + " \u2661",    //heart
    Color::bg_black + " \u2667",    //clover
    Color::bg_red   + " \u2662"     //diamond
};
const std::string num[13] = {
    " A ", " 2 ", " 3 ", " 4 " , " 5 ", " 6 ", " 7 ", " 8 ", " 9 ", "10 ", " J ", " Q ", " K "
};

//create deck
struct Deck{
    std::string deck[52];
    std::string joker[2] = {"Joker Black", "Joker Red"};
    Deck(){
        for(int i = 0; i < 52; ++i){
            int x = i / 13;
            int y = i % 13;
            deck[i] = shape[x] + " " + num[y];
        }
    }
};

//shuffle cards
void shuffle(Deck* cards){
    for(int i = 51; i >= 0; --i){
        std::uniform_int_distribution<int> gen(0, i);
        int j = gen(rng);
        std::string temp = cards->deck[i];
        cards->deck[i] = cards->deck[j];
        cards->deck[j] = temp;
    }
}

//show all cards
void show(Deck cards){
    std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    for(int i = 0; i < 13; ++i){
        if(i != 0 && i % 13 == 0) std::cout << '\n';
        std::cout << cards.deck[i] << '\t' 
                << cards.deck[i + 13] << '\t' 
                << cards.deck[i + 26] << '\t' 
                << cards.deck[i + 39] << '\t' 
                << Color::clear << "\n";
    }
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
}

int main(){
    SetConsoleOutputCP(CP_UTF8);
    
    //initialize a deck
    Deck cards;
    
    //game start
    while(true){
        system("cls");
        std::string input;
        int choice = 0;
        std::cout << "Options:\n"
            << "1) Shuffle\n"
            << "2) Show\n"
            << "3) Reset\n"
            << "\t\t(Press X to exit)\n"
            << "Choice: "
        ;
        std::getline(std::cin, input);
        
        if(input[0] == 'x' || input[0] == 'X') break;
        try{
            choice = std::stoi(input);
        }catch(...){
            std::cerr << "\nInvalid input, try again.\n\n";
            continue;
        }

        switch(choice){
            case 1:     //shuffle
                shuffle(&cards);
                std::cout << "The deck has been reshuffled.\n\n";
                break;
            case 2:     //print out the cards
                show(cards);
                break;
            case 3:     //reset deck
                cards = Deck();
                std::cout << "The deck has been reset.\n\n";
                break;
            default:
                std::cout << "Invalid input.\n\n";
        }

        std::cout << "Click anything to continue...";
        std::getline(std::cin, input);
    }

    std::cout << "Buh bye.";
    
    return 0;
}