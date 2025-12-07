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
bool hasInitialMatch(int r,int c);

// ----------------------------------------------
//                     MAIN
// ----------------------------------------------

int main() {
    int choice;

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

/* ------------ FUNCTION DEFINITIONS (TODO) ------------ */

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
        }

        // Timer
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
    // TODO: ensure no initial matches
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
        cin >> r >> c;
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
    if (nr < 0 || nc < 0 || nr >= boardSize || nc >= boardSize) return;

    char t = board[r][c];
    board[r][c] = board[nr][nc];
    board[nr][nc] = t;
}

int checkMatches() {
    // TODO: Find horizontal, vertical, L matches
    return 0;
}

void removeMatches() {
    // TODO: mark blank
}

void applyGravity() {
    // TODO: candies fall
}

void refillBoard() {
    // TODO: add new candies
}

void cascade() {
    // TODO: repeat until no more matches
}

void saveGame() {
    // TODO: save board, time, score to file
}

bool loadGame() {
    // TODO: load board and details from file
    return false;
}

void saveHighScore() {
    // TODO: ask name and save top 10 scores
}

void showHighScores() {
    // TODO: show highestScore.txt
}

char randomCandy() {
    if (hardMode) return hardCandies[rand() % 7];
    return easyCandies[rand() % 5];
}

bool hasInitialMatch(int r, int c) {
    // TODO: prevent starting matches
    return false;
}
