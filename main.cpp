#include <iostream>
#include <ctime> //random seed
#include <cstdlib> //rand
#include <windows.h> //clear screen
#include <iomanip> // I/O formatting
#include <string> // string search for rollInterpretor
#include <stdlib.h> // string to int conversion


void ClearScreen()
  {
  HANDLE                     hStdOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      count;
  DWORD                      cellCount;
  COORD                      homeCoords = { 0, 0 };

  hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
  if (hStdOut == INVALID_HANDLE_VALUE) return;

  /* Get the number of cells in the current buffer */
  if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
  cellCount = csbi.dwSize.X *csbi.dwSize.Y;

  /* Fill the entire buffer with spaces */
  if (!FillConsoleOutputCharacter(
    hStdOut,
    (TCHAR) ' ',
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Fill the entire buffer with the current colors and attributes */
  if (!FillConsoleOutputAttribute(
    hStdOut,
    csbi.wAttributes,
    cellCount,
    homeCoords,
    &count
    )) return;

  /* Move the cursor home */
  SetConsoleCursorPosition( hStdOut, homeCoords );
  }

using namespace std;

int stringToInt(string s){
    int value = atoi(s.c_str());
    return value;
}

void characterCreationDiceRoll(){
    bool scoreToLowHigh = true;
    bool scoreToLowBonus = true;
    while(scoreToLowHigh || scoreToLowBonus){
        scoreToLowHigh = true;
        scoreToLowBonus = true;

        int dice[5];
        int abilityScore[7];
        int abilityBonus[7];
        int abilityBonusTotal=0;
        cout << "Rolling 6*4d6b3 after enter" << endl;
        cin.get();
        cin.clear();
        srand(time(0));
        for(int j=0; j<6; j++){
            int rollMinimum = 6;
            for (int i=0; i<4; i++){
                dice[i] = rand()%6+1;
                cout << setw(2) << dice[i] << ", ";
                if (dice[i] < rollMinimum){
                    rollMinimum = dice[i];
                }
            }
            abilityScore[j] = (dice[0]+dice[1]+dice[2]+dice[3]-rollMinimum);
            abilityBonus[j] = abilityScore[j]/2-5;
            if (abilityScore[j] > 13){
                scoreToLowHigh = false;
            }
            abilityBonusTotal += abilityBonus[j];
            cout << "abilityScore: " << setw(2) << abilityScore[j] << ", abilityBonus: " << setw(2) << abilityBonus[j] << endl;
        }
        if(abilityBonusTotal>0){
            scoreToLowBonus = false;
            if(scoreToLowHigh == false){
                cout << "A good roll!" << endl << endl;
            }else{
                cout << "Your highest score was not high enough, rerolling." << endl << endl;
            }
        }else{
            cout << "Your average abilityBonus is too low, rerolling."<< endl << endl;
        }
    }
}

void rollInterpretor(string choice){
    //seed random generator with time
    srand(time(0));
    int rolls = 1;
    int dice = 1;
    int sides = 0;
    int best = 0;
    int add = 0;
    int index = 0;
    string inputBackup = choice;

    //search amount of rolls and remove it
    index = choice.find('*');
    if(index!=string::npos){
        rolls = stringToInt(choice.substr(0,index));
        choice = choice.substr(index+1);
    }

    //search amount of dice and remove it
    index = choice.find('d');
    if (index >0){
        dice = stringToInt(choice.substr(0,index));
        choice = choice.substr(index+1);
    }else if (index == 0){
        choice = choice.substr(index+1);
    }

    //search for add and remove it
    index = choice.find('+');
    if(index!=string::npos){
        add = stringToInt(choice.substr(index+1));
        choice = choice.substr(0,index);
    }else{
        index = choice.find('-');
        if(index!=string::npos){
            add = -1*stringToInt(choice.substr(index+1));
            choice = choice.substr(0,index);
        }
    }

    //search for best and remove it
    index = choice.find('b');
    if(index!=string::npos){
        best = stringToInt(choice.substr(index+1));
        choice = choice.substr(0,index);
    }else{
        // if no amount of best selected, keep all dice
        best = dice;
    }

    //leftover = sides
    sides = stringToInt(choice);

    //do the rolling
    ClearScreen();
    cout << "Rolling " << inputBackup << ":" << endl;
    // #rolls

    for (int i = 0; i<rolls; i++){
        // #dice
        int value[dice+1]; //maybe values[] and .push is cleaner?
        cout << "roll #" << i+1 << ": ";
        for (int j = 0; j<dice; j++){
            value[j] = rand()%sides+1+add;
            cout << value[j] << ", ";
        }
        cout << endl;
    }
    cout << endl << endl;
    cout << "Press enter to return to the main menu" << endl;
    cin.get();
    cin.clear();
    ClearScreen();
}

/* char create only main (old)
int main(){
    char choice;
    srand(time(0));
    cout << "Hello fantasy world!" << endl;
    do{
        characterCreationDiceRoll();
        cout << "Roll again? [y]" << endl;
        cin >> choice;
    }while(choice=='y' or choice == 'Y');
    cout << endl << "Press enter to exit"<<endl;
    cin.get();
    cin.get();
    return 0;
}
*/

//dice roll interpretor  only main
int main(){
    string choice; // your choice
    string sExit = "exit"; //exit compare string
    string sChar = "char"; //characterCreationDiceRoll string
    cout << "Hello fantasy world!" << endl;
    do{
        cout << "Type a roll or \"help\" for options or \"exit\" for exit" << endl;
        cout << "Type \"char\" to roll for a new character (old option)" << endl;
        cin >> choice;
        string sHelp = "help";
        if (choice == sHelp){
            ClearScreen();
            cout << "Roll options: (x1*)(x2)dx3(bx4)(+x5)" << endl;
            cout << "x1 to x5 are numbers, greater then 0" << endl;
            cout << "all options between () are optional" << endl;
            cout << "x1 is how many times the roll is rolled" << endl;
            cout << "x2 is the amount of dice, default when omitted is 1" << endl;
            cout << "x3 is the amount of sides on the dice" << endl;
            cout << "x4 is best x of, 7d6b3 rolls 7 dice and keeps the best 3" << endl;
            cout << "x5 is added to the roll at the end, but before x1" << endl;
            cout << "the \"+\" can be changed to \"-\"" << endl;
        }
        else if (choice == sChar){
            ClearScreen();
            characterCreationDiceRoll();
        }
        else if (choice != sExit and choice.find('d') != string::npos){
            ClearScreen();
            rollInterpretor(choice);
        }
        else if (choice != sExit){
            ClearScreen();
            cout << "\"" << choice << "\" is unknown, please try again." << endl << endl;
        }
    }while(choice != sExit);
    ClearScreen();
    cout << "Press enter to exit"<<endl;
    cin.get();
    cin.get();
    return 0;
}
