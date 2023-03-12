#include "scoreboard.h"

// #include <iostream>

// using namespace std;

// int main(){
//     char * item = setScore(0,0);
//     for(int i = 0; i <16; i++){
//         cout << "char" << *(item + i) << endl;
//     }
//     return 0;
// }   

char* setScore(int scoreOne, int scoreTwo){
    // TODO add a catch for if either score is 0
    bool scoreOneIsZero = false;
    if (scoreOne == 0){
        scoreOneIsZero = true;
    }
    bool scoreTwoIsZero = false;
    if (scoreTwo == 0){
        scoreTwoIsZero = true;
    }
    int len = 15;
    int copyScoreOne = scoreOne;
    int lenScoreOne = 0;
    while(copyScoreOne != 0){
        copyScoreOne = copyScoreOne / 10;
        lenScoreOne++;
    }
    int copyScoreTwo = scoreTwo;
    int lenScoreTwo = 0;
    while(copyScoreTwo != 0){
        copyScoreTwo = copyScoreTwo / 10;
        lenScoreTwo++;
    }

    char* message = new char[16];
    
    char* first = new char[lenScoreOne];
    int counter = lenScoreOne - 1;
    while(scoreOne != 0){
        char charToAdd;
        int digit = scoreOne % 10; //if number if 76, gets 6, then 7
        charToAdd = '0' + digit;
        scoreOne = scoreOne / 10;
        first[counter] = charToAdd;
        counter--;
    }
    counter = lenScoreTwo - 1;
    char* second = new char[lenScoreTwo];
    while(scoreTwo != 0){
        char charToAdd;
        int digit = scoreTwo % 10; //if number if 
        charToAdd = '0' + digit;
        scoreTwo = scoreTwo / 10;
        second[counter] = charToAdd;
        counter--;
    }
    // both char number arrays are now populated
    if (scoreTwoIsZero) {
        lenScoreTwo = 1;
    }
    // first number added:
    if (!scoreOneIsZero){
        for (int i = 0; i < lenScoreOne; i++){
            message[i] = first[i];
            
        }
    } else {
        message[0] = '0';
        lenScoreOne = 1;
    }
    delete[] first;

    for (int i = lenScoreOne; i < (16 - lenScoreTwo); i++){
        message[i] = '-';
    }
    if (!scoreTwoIsZero){
        for (int i = (16 - lenScoreTwo); i < 16; i++){
            message[i] = second[i - (16 - lenScoreTwo)];
        }
    } else {
        message[15] = '0';
    }
    delete[] second;
    return message;
}
