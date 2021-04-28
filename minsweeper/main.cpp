#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>

using namespace std;

vector<vector<int>> makeGameboard();
void printGameboard(vector<vector<string>> currentBoard, int guessedBombs);
void printLosingGameboard(vector<vector<int>> markedBoard, vector<vector<string>> currentBoard);
void printGameboard(char currentBoard[][9]);
string uncoverTile(vector<vector<int>> completeBoard, int row, int column);

class Node {
	public:
		int r;
		int c;
	Node* next;
};

int main() {
	cout << "Welcome to Minesweeper" << endl;
	vector<vector<int>> markedBoard = makeGameboard();
	// first number is left and second number is top row
	// cout << markedBoard[0][8] << endl; 
	vector<vector<string>> playingBoard(9, vector<string>(9, "-"));
	/*char newBoard[9][9];
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			newBoard[i][j] = '-';
		}
	}*/
	int guessedBombs = 10;
	printGameboard(playingBoard, guessedBombs);
	int totalBombsLeft = 10;
	bool notLost = true;
	while (notLost && totalBombsLeft>0) {
		int row, column;
		char bomb;
		cout << "Select your next move: selecting bomb (y/n)" << endl;
		cin >> bomb;
		cout << "Select your next move: pick the row" << endl;
		cin >> row;
		cout << "Pick your column" << endl;
		cin >> column;
		int newTile = markedBoard[row][column];
		if (bomb == 'y') {
			playingBoard[row][column] = "*";
			guessedBombs--;
			if (markedBoard[row][column] == -1) {
				totalBombsLeft--;
			}
		}
		else if (newTile == -1) {
			cout << "YOU LOST!" << endl;
			notLost = false;
			printLosingGameboard(markedBoard, playingBoard);
			continue; // need to print full board somehow
		}
		else {
			bool checkAround = false;
			if (playingBoard[row][column] != "*" && playingBoard[row][column] != "-") {
				checkAround = true;
				cout << playingBoard[row][column] << endl;
				cout << "Checking around" << endl;
			}
			if (playingBoard[row][column] == "*") {
				guessedBombs++;
			}
			//cout << newTile << endl;
			char aChar = '0' + newTile;
			//cout << aChar << endl;
			playingBoard[row][column] = aChar;
			struct Node {
				int r;
				int c;
				struct Node* next;
			};
			// search for all neighboring blocks if zero
			if (newTile == 0 || checkAround) {
				//cout << "Tile was 0" << endl;
				Node* head = NULL;
				head = new Node();
				head->r = row;
				head->c = column;
				Node* lastNode = head;
				Node* nodeSearching = head;
				while (nodeSearching != NULL) {
					int i = nodeSearching->r, j = nodeSearching->c;
					if (i != 0) {
						if (j != 0 && playingBoard[i - 1][j - 1] != "*") {
							//cout << "j does not equal 0";
							// top left
							aChar = '0' + markedBoard[i - 1][j - 1];
							if (markedBoard[i - 1][j - 1] == 0 && playingBoard[i - 1][j - 1] == "-") {
								Node* newNode = NULL;
								newNode = new Node();
								newNode->r = i - 1;
								newNode->c = j - 1;
								lastNode->next = newNode;
								lastNode = newNode;
							}
							playingBoard[i - 1][j - 1] = aChar;
						}
						if (j != 8 && playingBoard[i - 1][j + 1] != "*") {
							//cout << "j does not equal 8";
							// top right
							aChar = '0' + markedBoard[i - 1][j + 1];
							if (markedBoard[i - 1][j + 1] == 0 && playingBoard[i - 1][j + 1] == "-") {
								Node* newNode = NULL;
								newNode = new Node();
								newNode->r = i - 1;
								newNode->c = j + 1;
								lastNode->next = newNode;
								lastNode = newNode;
							}
							playingBoard[i - 1][j + 1] = aChar;
						}
						// top
						if (playingBoard[i - 1][j] != "*") {
							aChar = '0' + markedBoard[i - 1][j];
							if (markedBoard[i - 1][j] == 0 && playingBoard[i - 1][j] == "-") {
								Node* newNode = NULL;
								newNode = new Node();
								newNode->r = i - 1;
								newNode->c = j;
								lastNode->next = newNode;
								lastNode = newNode;
							}
							playingBoard[i - 1][j] = aChar;
						}	
					}
					// middle
					if (j != 0 && playingBoard[i][j - 1] != "*") {
						//cout << "j does not equal 0";
						// middle left
						aChar = '0' + markedBoard[i][j - 1];
						if (markedBoard[i][j - 1] == 0 && playingBoard[i][j - 1] == "-") {
							//uncover on playingBoard
								//add new Node to linkedList
							Node* newNode = NULL;
							newNode = new Node();
							newNode->r = i;
							newNode->c = j - 1;
							lastNode->next = newNode;
							lastNode = newNode;
						}
						playingBoard[i][j - 1] = aChar;
					}
					if (j != 8 && playingBoard[i][j + 1] != "*") {
						//cout << "j does not equal 8";
						// middle right
						aChar = '0' + markedBoard[i][j + 1];
						if (markedBoard[i][j + 1] == 0 && playingBoard[i][j + 1] == "-") {
							//uncover on playingBoard
								//add new Node to linkedList
							Node* newNode = NULL;
							newNode = new Node();
							newNode->r = i;
							newNode->c = j + 1;
							lastNode->next = newNode;
							lastNode = newNode;
						}
						playingBoard[i][j + 1] = aChar;
					}
					// bottom
					if (i != 8) {
						//cout << "i does not equal 8";
						if (j != 0 && playingBoard[i + 1][j - 1] != "*") {
							//cout << "j does not equal 0";
							// bottom left
							aChar = '0' + markedBoard[i + 1][j - 1];
							if (markedBoard[i + 1][j - 1] == 0 && playingBoard[i + 1][j - 1] == "-") {
								//uncover on playingBoard
									//add new Node to linkedList
								Node* newNode = NULL;
								newNode = new Node();
								newNode->r = i + 1;
								newNode->c = j - 1;
								lastNode->next = newNode;
								lastNode = newNode;
							}
							playingBoard[i + 1][j - 1] = aChar;
						}
						if (j != 8 && playingBoard[i + 1][j + 1] != "*") {
							//cout << "j does not equal 8";
							// bottom right
							aChar = '0' + markedBoard[i + 1][j + 1];
							if (markedBoard[i + 1][j + 1] == 0 && playingBoard[i + 1][j + 1] == "-") {
								//uncover on playingBoard
									//add new Node to linkedList
								Node* newNode = NULL;
								newNode = new Node();
								newNode->r = i + 1;
								newNode->c = j + 1;
								lastNode->next = newNode;
								lastNode = newNode;
							}
							playingBoard[i + 1][j + 1] = aChar;
						}
						// bottom
						if (playingBoard[i + 1][j] != "*") {
							aChar = '0' + markedBoard[i + 1][j];
							if (markedBoard[i + 1][j] == 0 && playingBoard[i + 1][j] == "-") {
								//uncover on playingBoard
									//add new Node to linkedList
								Node* newNode = NULL;
								newNode = new Node();
								newNode->r = i + 1;
								newNode->c = j;
								lastNode->next = newNode;
								lastNode = newNode;
							}
							playingBoard[i + 1][j] = aChar;
						}
					}
					nodeSearching = nodeSearching->next;
				}
			}
		}
		
		printGameboard(playingBoard, guessedBombs);
	}
	if (totalBombsLeft == 0) {
		cout << "YOU WIN!" << endl;
	}
	return 0;
}

vector<vector<int>> makeGameboard() {
	//int board[9][9];
	vector<vector<int>> board (9, vector<int>(9,0));
	int bombsRemaining = 10;

	int randomNumber = rand();
	cout << randomNumber << endl;

	srand(time(NULL));
	int secret = rand() % 10 + 1;
	cout << secret << endl;

	int i, j;

	for (i = 0; i < bombsRemaining;) {
		int random = rand() % (9 * 9);
		int x = random / 9;
		int y = random % 9;
		cout << random <<": " << x << y << endl;
		if (board[x][y] == -1) {
			continue;
		}
		board[x][y] = -1;
		i++;
	}

	cout << "making gameboard" << endl;

	for (i = 0; i < 9; ++i) {
		for (j = 0; j < 9; ++j) {
			//cout << "checking board[" << i << "][" << j << "]" << endl;
			// does it have bomb?
			// if no initialize at zero
			// count in tl t tr l r bl b br
			if (board[i][j] == -1) {
				continue;
			}
			//board[i][j] = 0;
			// classic
			int bombsInVicinty = 0;
			// top row
			if (i != 0) {
				//cout << "i does not equal 0";
				if (j != 0) {
					//cout << "j does not equal 0";
					// top left
					if (board[i - 1][j - 1] == -1) {
						bombsInVicinty++;
					}
				}
				if (j != 8) {
					//cout << "j does not equal 8";
					// top right
					if (board[i - 1][j+1] == -1) {
						bombsInVicinty++;
					}
				}
				// top
				if (board[i - 1][j] == -1) {
					bombsInVicinty++;
				}
			}
			// middle
			if (j != 0) {
				//cout << "j does not equal 0";
				// middle left
				if (board[i][j - 1] == -1) {
					bombsInVicinty++;
				}
			}
			if (j != 8) {
				//cout << "j does not equal 8";
				// middle right
				if (board[i][j + 1] == -1) {
					bombsInVicinty++;
				}
			}
			// bottom
			if (i != 8) {
				//cout << "i does not equal 8";
				if (j != 0) {
					//cout << "j does not equal 0";
					// bottom left
					if (board[i + 1][j - 1] == -1) {
						bombsInVicinty++;
					}
				}
				if (j != 8) {
					//cout << "j does not equal 8";
					// bottom right
					if (board[i + 1][j + 1] == -1) {
						bombsInVicinty++;
					}
				}
				// bottom
				if (board[i + 1][j] == -1) {
					bombsInVicinty++;
				}
			}
			//cout << "bombs around: " << bombsInVicinty;
			board[i][j] = bombsInVicinty;
		}
	}
	/*cout << "Printing board" << endl;
	
	cout << "  ";
	for (i = 0; i < 9; i++) {
		cout << i << " ";
	}
	cout << endl;
	for (i = 0; i < 9; i++) {
		cout << i << " ";
		for (j = 0; j < 9; j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}*/

	return board;
}

void printGameboard(vector<vector<string>> currentBoard, int guessedBombs) {
	int i, j;
	cout << "BOMBS LEFT: " << guessedBombs << endl;
	cout << "  ";
	for (i = 0; i < 9; i++) {
		cout << i << " ";
	}
	cout << endl;
	for (i = 0; i < 9; i++) {
		cout << i << " ";
		for (j = 0; j < 9; j++) {
			cout << currentBoard[i][j] << " ";
		}
		cout << endl;
	}
}

void printGameboard(char currentBoard[][9]) {
	int i, j;
	cout << "  ";
	for (i = 0; i < 9; i++) {
		cout << i << " ";
	}
	cout << endl;
	for (i = 0; i < 9; i++) {
		cout << i << " ";
		for (j = 0; j < 9; j++) {
			cout << currentBoard[i][j] << " ";
		}
		cout << endl;
	}
}

void printLosingGameboard(vector<vector<int>> markedBoard, vector<vector<string>> currentBoard) {
	int i, j;
	cout << "  ";
	for (i = 0; i < 9; i++) {
		cout << i << " ";
	}
	cout << endl;
	for (i = 0; i < 9; i++) {
		cout << i << " ";
		for (j = 0; j < 9; j++) {
			if (markedBoard[i][j] == -1 && currentBoard[i][j] != "*") {
				cout << "B ";
			}
			else {
				cout << currentBoard[i][j] << " ";
			}
		}
		cout << endl;
	}
}