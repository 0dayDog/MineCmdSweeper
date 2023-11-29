#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <ctime>

using namespace std;

const char* yellow = "\033[01;33m";
const char* white = "\033[01;37m";
const char* green = "\033[01;32m";
const char* blue = "\033[01;34m";
const char* red = "\033[1;31m";
const char* stop = "\033[0m";

const int BOARD_SIZE = 10;
const int NUM_MINES = 10;
const char COVERED_CELL = '#';
const char MINE_CELL = '*';

int score = 0;
int maxScore = BOARD_SIZE * BOARD_SIZE - NUM_MINES;

void score_show(int s, int max, chrono::duration<int> ptime) {
    cout << "\n----\n"
            "Bombs minesweeper result\n"
            "   Type: palyer score\n"
            "   Version: 0.1.5\n"
            "   Author: h_key\n"
            "   Score: " << s << "\n";
    cout << "   Out: " << max << "\n";
    cout << "   Your time: " << ptime.count() << " seconds\n";
    cout << "----\n";
}

void initializeBoard(vector<vector<char>>& board) {
    board.assign(BOARD_SIZE, vector<char>(BOARD_SIZE, COVERED_CELL));
}

void placeMines(vector<vector<char>>& board) {
    // Use current time as seed for random generator
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<int> dis(0, BOARD_SIZE - 1);

    int minesPlaced = 0;
    while (minesPlaced < NUM_MINES) {
        int row = dis(gen);
        int col = dis(gen);

        if (board[row][col] != MINE_CELL) {
            board[row][col] = MINE_CELL;
            minesPlaced++;
        }
    }
}

void displayBoard(const vector<vector<char>>& board, bool showMines) {
    cout << "\n   ";
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << i << " ";
    }
    
    // cout << endl;

    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << "\n" << i << "  ";
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == MINE_CELL && !showMines) {
                cout << COVERED_CELL << " ";
            } else {
                if (board[i][j] == MINE_CELL) {
                    cout << red << board[i][j] << stop << " ";
                } else if (isdigit(board[i][j])) {
                    cout << green << board[i][j] << stop << " ";
                } else {
                    cout << board[i][j] << " ";
                }
            }
        }
    }
    cout << endl;
}

int countAdjacentMines(const vector<vector<char>>& board, int row, int col) {
    int count = 0;

    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE) {
                if (board[i][j] == MINE_CELL) {
                    count++;
                }
            }
        }
    }

    return count;
}

void uncoverCell(vector<vector<char>>& board, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return;
    }

    if (board[row][col] == COVERED_CELL) {
        score++;
        int count = countAdjacentMines(board, row, col);
        if (count == 0) {
            board[row][col] = ' ';
            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    uncoverCell(board, i, j);
                }
            }
        } else {
            board[row][col] = count + '0';
        }
    }
}

bool isMine(const vector<vector<char>>& board, int row, int col) {
    return board[row][col] == MINE_CELL;
}

time_t time() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    return now_time;
}

int main() {
    // score = 0;
    // wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    char playAgain = 'y';
    while (playAgain == 'y' || playAgain == 'Y') {
        auto start = std::chrono::high_resolution_clock::now();
        score = 0;
        vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE, COVERED_CELL));
        vector<vector<bool>> visited(BOARD_SIZE, vector<bool>(BOARD_SIZE, false));

        // Initialize the board and place mines
        initializeBoard(board);
        placeMines(board);

        // Display the initial board
        time_t t = time();
        cout << "\n[*] Start minesweeper @ " << put_time(localtime(&t), "%H:%M:%S") << endl;
        displayBoard(board, false);

        bool gameOver = false;
        // Check '/n' if needed
        int check;
        while (!gameOver) {
            // Get user input
            int row, col;
            time_t t = time();
            if (check == 0) {
                cout << "\n[" << blue << put_time(localtime(&t), "%H:%M:%S") << stop << "] [" << green << "INFO" << stop << "] Enter row and column (e.g., 0 0) :" << blue << "~" << stop << "$ ";
                check = 0;
            }
            if (check == 1) {
                cout << "[" << blue << put_time(localtime(&t), "%H:%M:%S") << stop << "] [" << green << "INFO" << stop << "] Enter row and column (e.g., 0 0) :" << blue << "~" << stop <<"$ ";
                check = 0;
            }
            // cout << "\n[" << blue << put_time(localtime(&t), "%H:%M:%S") << stop << "] [" << green << "INFO" << stop << "] Enter row and column (e.g., 0 0) :" << blue << "~" << stop <<"$ ";
            cin >> row;
            
            if (cin.peek() != ' ') {
                time_t t = time();
                cout << "[" << blue << put_time(localtime(&t), "%H:%M:%S") << stop << "] [" << red << "WARNING" << stop << "] Invalid input format. Please enter row and column as numbers separated by a space." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                check = 1;
                continue;
            }
            
            cin.ignore(); // Ignore the space character
            
            cin >> col;

            // Check if the input is within the board size
            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || visited[row][col]) {
                time_t t = time();
                cout << "[" << blue << put_time(localtime(&t), "%H:%M:%S") << stop << "] [" << red << "WARNING" << stop << "] Invalid input. Please try again." << endl;
                check = 1;
                // cout << "[WARNING] Invalid input. Please try again." << endl;
                continue;
            }

            // Check if the user stepped on a mine
            if (isMine(board, row, col)) {
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
                gameOver = true;
                cout << "\n[-] Game over! You stepped on a mine." << endl;
                displayBoard(board, true);
                score_show(score, maxScore, duration);
                break;
            } else {
                uncoverCell(board, row, col);
                visited[row][col] = true;
            }

            // Check if the player has won
            int uncoveredCount = 0;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] != COVERED_CELL && board[i][j] != MINE_CELL) {
                        uncoveredCount++;
                    }
                }
            }
            if (uncoveredCount == BOARD_SIZE * BOARD_SIZE - NUM_MINES) {
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
                gameOver = true;
                cout << "\n[+] Congratulations! You won the game!" << endl;
                displayBoard(board, true);
                score_show(score, maxScore, duration);
                break;
            }

            // Display the updated board
            displayBoard(board, false);
        }

        do {
            cout << "[" << blue << put_time(localtime(&t), "%H:%M:%S") << stop << "] [" << green << "INFO" << stop << "] Do you want to play again? (y/n) :" << blue << "~" << stop <<"$ ";
            cin >> playAgain;
            switch(tolower(playAgain)) {
                case 'y':
                case 'n':
                    break;
                default:
                    cout << "[" << blue << put_time(localtime(&t), "%H:%M:%S") << stop << "] [" << red << "WARNING" << stop << "] Invalid input. Please enter 'y' or 'n'." << endl;
                    playAgain = 0; // Reset playAgain to an invalid value
                    break;
            }
        } while (playAgain != 'y' && playAgain != 'n');
    }

    return 0;
}