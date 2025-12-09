/******************************************************
   PF PROJECT - CANDY CRUSH GAME
   Simple Console Pipeline
   - No vectors
   - Simple arrays
   - Easy logic structure
   - All required features included
*******************************************************/

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <fstream>
#include <chrono>
#include<ctime>
using namespace std;

/* ------------ GLOBAL VARIABLES (Minimal) ------------ */

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

/* ------------ FUNCTION PROTOTYPES ------------ */

void showMenu();
void instructions();
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
            if (!loadGame()) cout << "\nNo saved game found.\n";
            system("pause");
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

/* ------------ FUNCTION DEFINITIONS ------------ */

void showMenu() {
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
    cout << "\nHow To Play:\n";
    cout << "Select row and column.\n";
    cout << "Use arrow keys to swap.\n";
    cout << "Match 3 or more candies to score.\n";
    cout << "Press P to pause and save.\n";
}

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
             swapCandies(r, c, dr, dc); // swap back if no match
        }

        auto now = chrono::steady_clock::now();
        int elapsed = chrono::duration_cast<chrono::seconds>(now - start).count();
        if (elapsed >= timeLeft) gameRunning = false;
    }

    cout << "\nGame Over! Score: " << score << endl;
    saveHighScore();
    system("pause");
}

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

void displayBoard() {
    system("cls");
    cout << "\nScore: " << score << "\nTimeLeft: " << timeLeft << endl;

    cout << "   ";
    for (int c = 0; c < boardSize; c++) cout << c <<" ";
    cout << endl;

    for (int r = 0; r < boardSize; r++) {
        cout << r <<"  ";
        for (int c = 0; c < boardSize; c++) {
            cout << board[r][c] << " ";
        }
        cout << endl;
    }
}

void getSelection(int &r, int &c) {
    cout << "Row Col (P to pause): ";
    char ch = _getch();

    if (ch == 'P' || ch == 'p') {
        r = -1; c = -1;
        return;
    } else {
        cout << ch; 
        r = ch - '0';
        cin >> c;
    }
}

int getArrow() {
    while (true) {
        int k = _getch();
        if (k == 224) return _getch();
    }
}

void swapCandies(int r, int c, int dr, int dc) {
    int nr = r + dr, nc = c + dc;
    if (nr<0 || nc<0 || nr>=boardSize || nc>=boardSize) return;

    char t = board[r][c];
    board[r][c] = board[nr][nc];
    board[nr][nc] = t;
}

int checkMatches() {
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

void cascade() {
    while (checkMatches() > 0) {
        removeMatches();
        Sleep(200);
        displayBoard();
        applyGravity();
        Sleep(200);
        displayBoard();
        refillBoard();
        Sleep(200);
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
    return true;
}

void saveHighScore() {
    cout << "enter your name : ";
    string n;
    cin>>n;
    string name[11];
    int scores[11];
    scores[0] = score;
    name[0] = n;
    ifstream read("scores.txt");
    for(int i=1 ; i<11 ; i++){
        if(read.eof()) { name[i] = ""; scores[i] = 0; }
        else read >> name[i] >> scores[i];
    }
    read.close();
    for(int i=1 ; i<11 ; i++){
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
    cout << "======= HIGH SCORES =======";
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

char randomCandy() {
    if (hardMode == true) return hardCandies[rand() % 7];
    return easyCandies[rand() % 5];
}
