/******************************************************
   PF PROJECT - CANDY CRUSH GAME
    Faisal Shahzad 25F-0619
    Asmad Nadeem 25F-0537
*******************************************************/

#include <iostream>
#include <conio.h>
#include <string>
#include <Windows.h>
#include <fstream>
#include <chrono>
#include <ctime>
using namespace std;

//GLOBAL VARIABLES

const int MAX = 10;

char board[MAX][MAX];
char markBoard[MAX][MAX];

int boardSize = 0;
int score = 0;
int timeLeft = 0;
bool hardMode = false;
bool gameRunning = false;

char easyCandies[5] = {'@', '#', '&', '$', '%'};
char hardCandies[7] = {'@', '#', '&', '$', '%', '!', '*'};

// FUNCTION PROTOTYPEs

void showMenu();
void instructions();
void setC(int color);
void startGame(bool mode);
void initializeBoard();
void displayBoard();
void getSelection(int &r, int &c);
int getArrow();
void swapCandies(int r,int c,int dr,int dc);
int checkMatches();
void removeMatches();
void applyGravity();
void refillBoard();
void cascade();
void saveGame();
bool loadGame();
void saveHighScore();
void showHighScores();
char randomCandy();
string formatTime(int seconds);

// ----------------------------------------------
//                      MAIN
// ----------------------------------------------

int main() {
    int choice;
    srand(time(0));

    while (true) {
        system("cls");
        showMenu();
        cin >> choice;

        if (choice == 1) startGame(false);
        else if (choice == 2) startGame(true);
        else if (choice == 3) {
            if (!loadGame()){
                cout << "\nNo saved game found.\n";
                system("pause");
            }
            else {
                gameRunning = true;
                auto start = chrono::steady_clock::now();
                
                while (gameRunning) {
                    displayBoard();
                    
                    int r, c;
                    getSelection(r, c);
                    
                    if (r == -1) {
                        saveGame();
                        break;
                    }
                    
                    int arrow = getArrow();
                    
                    int dr = 0, dc = 0;
                    if (arrow == 72) dr = -1;
                    if (arrow == 80) dr = 1;
                    if (arrow == 75) dc = -1;
                    if (arrow == 77) dc = 1;
                    
                    swapCandies(r, c, dr, dc);
                    
                    if (checkMatches() > 0) {
                        removeMatches();
                        applyGravity();
                        refillBoard();
                        cascade();
                    } else {
                        swapCandies(r, c, dr, dc);
                    }
                    
                    auto now = chrono::steady_clock::now();
                    int elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();
                    if (elapsed >= timeLeft) gameRunning = false;
                }
                
                cout << "\nGame Over! Score: " << score << endl;
                saveHighScore();
                system("pause");
            }
        }
        else if (choice == 4) {
            showHighScores();
            system("pause");
        }
        else if (choice == 5) {
            instructions();
            system("pause");
        }
        else break;
    }

    return 0;
}

// FUNCTION DEFINITIONS

void setC(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void showMenu() {
    setC(10);
    cout << "\n================ CANDY CRUSH =================\n";
    cout << "1. Easy Mode (8x8)\n";
    cout << "2. Hard Mode (10x10)\n";
    cout << "3. Load Saved Game\n";
    cout << "4. High Scores\n";
    cout << "5. Instructions\n";
    cout << "6. Exit\n";
    cout << "Choose: ";
}

void instructions() {
    setC(10);
    cout << "\nHow To Play:\n";
    cout << "Select row and column.\n";
    cout << "Use arrow keys to swap.\n";
    cout << "Match 3 or more candies to score.\n";
    cout << "Press P to pause and save.\n";
}
//main game logic
void startGame(bool mode) {
    hardMode = mode;
    boardSize = hardMode ? 10 : 8;
    timeLeft = hardMode ? 40 : 60;
    score = 0;
    gameRunning = true;

    initializeBoard();

    auto start = chrono::steady_clock::now();

    while (gameRunning) {

        displayBoard();

        int r, c;
        getSelection(r, c);

        if (r == -1) {
            saveGame();
            break;
        }
        else if(r == -2){
            cout << "\tinvalid input try again!";
            Sleep(300);
            continue;
        }

        int arrow = getArrow();

        int dr = 0, dc = 0;
        if (arrow == 72) dr = -1;
        if (arrow == 80) dr = 1;
        if (arrow == 75) dc = -1;
        if (arrow == 77) dc = 1;

        swapCandies(r, c, dr, dc);

        if (checkMatches() > 0) {
            removeMatches();
            applyGravity();
            refillBoard();
            cascade();
        } else {
            cout << "no matches found\n";
            Sleep(500);
            swapCandies(r, c, dr, dc);
        }

        auto now = chrono::steady_clock::now();
        int elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();
        timeLeft = (hardMode ? 40 : 60) - elapsed;
        if (timeLeft <= 0) gameRunning = false;
    }

    cout << "\nGame Over! Score: " << score << endl;
    saveHighScore();
    system("pause");
}
//initialize main board without intitializing consecutive three or more candies
void initializeBoard() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            board[i][j] = randomCandy();
        }
    }
    for(int i=0 ; i<boardSize ; i++){
        for(int j=0 ; j<boardSize ; j++){
            while((i>=2 && board[i][j]==board[i-1][j] && board[i-2][j]==board[i][j]) || (j>=2 && board[i][j]==board[i][j-1] && board[i][j-2]==board[i][j])){
                board[i][j] = randomCandy();
            }
        }
    }
}
//display board
void displayBoard() {
    system("cls");
    setC(2);
    cout << "\nScore: " << score << "\nTime Left: " << formatTime(timeLeft) << "s" << endl;

    cout << "   ";
    setC(2);
    for (int c = 0; c < boardSize; c++) cout << c <<" ";
    cout << endl;

    for (int r = 0; r < boardSize; r++) {
        setC(2);
        cout << r <<"  ";
        for (int c = 0; c < boardSize; c++) {
            if(board[r][c]=='%') setC(4);
            else if(board[r][c] == '#') setC(5);
            else if(board[r][c] == '@') setC(1);
            else if(board[r][c] == '&') setC(3);
            else if(board[r][c] == '$') setC(6);
            else if(board[r][c] == '!') setC(12);
            else if(board[r][c] == '*') setC(15);
            cout << board[r][c] << " ";
        }
        cout << endl;
    }
}

void getSelection(int &r, int &c) {
    string input1="", input2="";
    setC(2);
    cout << "enter row column (p to pause) : ";
    cin >> input1;
    if(input1 == "p" || input1 == "P"){c=-1; r=-1; return;}
    cin >> input2;
    if(input1 == "" || input2 == ""){c=-2; r=-2; return;}
    r = input1[0]-'0';
    c = input2[0]-'0';
    if(r < 0 || r>=boardSize || c<0 || c >= boardSize){r=-2; c=-2; return;}
}

int getArrow() {
    cout << "Use arrow keys to swap: ";
    while (true) {
        int k = _getch();
        if (k == 224 || k == 0) {
            int arrow = _getch();
            cout << "Direction selected!\n";
            return arrow;
        }
    }
}

void swapCandies(int r, int c, int dr, int dc) {
    int nr = r + dr, nc = c + dc;
    if (nr<0 || nc<0 || nr>=boardSize || nc>=boardSize) return;

    char t = board[r][c];
    board[r][c] = board[nr][nc];
    board[nr][nc] = t;
}
//check for any match
int checkMatches() {
    void displayBoard();
    int found = 0;
    for(int i=0 ; i<boardSize ; i++){
        for(int j=0 ; j<boardSize ; j++){
            markBoard[i][j] = 0;
        }
    }
    //vertical checks
    for(int i=0 ; i<boardSize ; i++){
        for(int j=0 ; j<boardSize ; j++){
            if(i<boardSize-4 && board[i][j]==board[i+1][j] && board[i][j]==board[i+2][j] && board[i][j]==board[i+3][j] && board[i][j]==board[i+4][j]){
                markBoard[i][j]=markBoard[i+1][j]=markBoard[i+2][j]=markBoard[i+3][j]=markBoard[i+4][j]=1;
                score += 20;
                found = 1;
            }
            else if(i<boardSize-3 && board[i][j]==board[i+1][j] && board[i][j]==board[i+2][j] && board[i][j]==board[i+3][j]){
                markBoard[i][j]=markBoard[i+1][j]=markBoard[i+2][j]=markBoard[i+3][j]=1;
                score += 15;
                found = 1;
            }
            else if(i<boardSize-2 && board[i][j]==board[i+1][j] && board[i][j]==board[i+2][j]){
                markBoard[i][j]=markBoard[i+1][j]=markBoard[i+2][j]=1;
                score += 10;
                found = 1;
            }
        }
    }
    //horizontal checks
    for(int i=0 ; i<boardSize ; i++){
        for(int j=0 ; j<boardSize ; j++){
            if(j<boardSize-4 && board[i][j]==board[i][j+1] && board[i][j]==board[i][j+2] && board[i][j]==board[i][j+3] && board[i][j]==board[i][j+4]){
                markBoard[i][j]=markBoard[i][j+1]=markBoard[i][j+2]=markBoard[i][j+3]=markBoard[i][j+4]=1;
                score += 20;
                found = 1;
            }
            else if(j<boardSize-3 && board[i][j]==board[i][j+1] && board[i][j]==board[i][j+2] && board[i][j]==board[i][j+3]){
                markBoard[i][j]=markBoard[i][j+1]=markBoard[i][j+2]=markBoard[i][j+3]=1;
                score += 15;
                found = 1;
            }
            else if(j<boardSize-2 && board[i][j]==board[i][j+1] && board[i][j]==board[i][j+2]){
                markBoard[i][j]=markBoard[i][j+1]=markBoard[i][j+2]=1;
                score += 10;
                found = 1;
            }
        }
    }
    //L checks
    for(int i=0 ; i<boardSize ; i++){
        for(int j=0 ; j<boardSize ; j++){
            if(i<boardSize-2 && j<boardSize-2 && board[i][j]==board[i+1][j] && board[i][j]==board[i+2][j] && board[i][j]==board[i+2][j+1] && board[i][j]==board[i+2][j+2]){
                score+=5;
                found =1;
                markBoard[i][j]=markBoard[i+1][j]=markBoard[i+2][j]=markBoard[i+2][j+1]=markBoard[i+2][j+2]=1;
            }
            else if(i<boardSize-2 && j<boardSize-2 && board[i][j]==board[i][j+1] && board[i][j]==board[i][j+2] && board[i][j]==board[i+1][j] && board[i][j]==board[i+2][j]){
                score+=5;
                found = 1;
                markBoard[i][j]=markBoard[i][j+1]=markBoard[i][j+2]=markBoard[i+1][j]=markBoard[i+2][j]=1;
            }
            else if(i<boardSize-2 && j>1 && board[i][j]==board[i][j-1] && board[i][j]==board[i][j-2] && board[i][j]==board[i+1][j] && board[i][j]==board[i+2][j]){
                score+=5;
                found = 1;
                markBoard[i][j]=markBoard[i][j-1]=markBoard[i][j-2]=markBoard[i+1][j]=markBoard[i+2][j]=1;
            }
            else if(i<boardSize-2 && j>1 && board[i][j]==board[i+1][j] && board[i][j]==board[i+2][j] && board[i][j]==board[i+2][j-1] && board[i][j]==board[i+2][j-2]){
                score+=5;
                found = 1;
                markBoard[i][j]=markBoard[i+1][j]=markBoard[i+2][j]=markBoard[i+2][j-1]=markBoard[i+2][j-2]=1;
            }
        }
    }
    return found;
}

void removeMatches() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (markBoard[i][j] == 1) {
                board[i][j] = ' ';
                markBoard[i][j] = 0;
            }
        }
    }
    Sleep(300);
}

void applyGravity() {
    for (int j = 0; j < boardSize; j++) {
        for (int i = boardSize - 1; i >= 0; i--) {
            if (board[i][j] == ' ') {
                int k = i - 1;
                while (k >= 0 && board[k][j] == ' ') {
                    k--;
                }
                if (k >= 0) {
                    board[i][j] = board[k][j];
                    board[k][j] = ' ';
                }
            }
        }
    }
    Sleep(200);
}

void refillBoard() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = randomCandy();
            }
        }
    }
}
// cascade effect to give animations
void cascade() {
    while (checkMatches() > 0) {
        removeMatches();
        Sleep(600);
        displayBoard();
        applyGravity();
        Sleep(500);
        displayBoard();
        refillBoard();
        Sleep(500);
        displayBoard();
    }
}

void saveGame() {
    ofstream save("saved.txt");
    if(!save.is_open()){
        cout << "error saving game";
        return;
    }
    save << score << " " << timeLeft << " " << boardSize << " " << hardMode << endl;
    for(int i=0 ; i<boardSize ; i++){
        for(int j=0 ; j<boardSize ; j++){
            save << board[i][j] << " ";
        }
        save << endl;
    }
    save.close();
    cout << "game saved successfully\n";
}

bool loadGame() {
    ifstream load("saved.txt");
    if(!load.is_open()){
        return false;
    }
    load >> score >> timeLeft >> boardSize >> hardMode;
    for(int i=0 ; i<boardSize ; i++){
        for(int j=0 ; j<boardSize ; j++){
            load >> board[i][j];
        }
    }
    load.close();
    cout << "game loaded successfully\n";
    system("pause");
    return true;
}

void saveHighScore() {
    cout << "enter your name : ";
    string n;
    cin >> n;
    string name[11];
    int scores[11];
    for(int i = 1; i < 11; i++) { 
        name[i] = ""; 
        scores[i] = 0; 
    }
    scores[0] = score;
    name[0] = n;
    ifstream read("scores.txt");
    for(int i=1 ; i<11 ; i++){
        if(read.eof()) { name[i] = ""; scores[i] = 0; }
        else read >> name[i] >> scores[i];
    }
    read.close();
    for(int i=0 ; i<11 ; i++){
        int key = scores[i];
        string nm = name[i];
        int j = i-1;
        while(j>=0 && scores[j]<key){
            scores[j+1] = scores[j];
            name[j+1] = name[j];
            j--;
        }
        scores[j+1] = key;
        name[j+1] = nm;
    }
    ofstream write("scores.txt");
    for(int i=0 ; i<10 ; i++){
        if (scores[i] != 0)
            write << name[i] << " " << scores[i] << endl;
    }
    write.close();
    cout << "scores updated successfully\n";
}

void showHighScores() {
    ifstream read("scores.txt");
    if(!read.is_open()){
        cout<< "No Highscores saved yet.\n";
        return ;
    }
    cout << "\n======= HIGH SCORES =======";
    string name;
    int scr;
    int count=1;
    cout << "\nRANK\tNAME\tSCORE";
    while(!read.eof()){
        read >> name >> scr;
        if(read.fail()) break;
        cout << endl << count << "\t" << name << "\t" << scr;
        count++;
        if (count > 10) break;
    }
    cout << endl;
    read.close();
}
//initialize random candies
char randomCandy() {
    if (hardMode == true) return hardCandies[rand() % 7];
    return easyCandies[rand() % 5];
}
//format time in MM:SS
string formatTime(int seconds) {
    int mins = seconds / 60;
    int secs = seconds % 60;
    string result = to_string(mins) + ":";
    if (secs < 10) result += "0";
    result += to_string(secs);
    return result;
}
