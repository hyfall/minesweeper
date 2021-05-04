#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>

using namespace std;

int rowSize = 9;
int colSize = 9;
int bombAmount = 10;

void gameLoop();
vector<vector<int>> makeGameboard();
void printGameboard(vector<vector<string>> currentBoard, int guessedBombs);
void printLosingGameboard(vector<vector<int>> markedBoard, vector<vector<string>> currentBoard);
//void printGameboard(char currentBoard[][]);
string uncoverTile(vector<vector<int>> completeBoard, int row, int column);

class Node {
	public:
		int r;
		int c;
	Node* next;
};

int main() {
	bool play = true;
	cout << "Welcome to Minesweeper" << endl;
	while (play) {
		int version;
		cout << "What difficulty would you like to play?" << endl << "1) Easy (9x9)" << endl << "2) Medium (16x16)" << endl << "3) Hard (16x30)" << endl;
		cin >> version;
		while (version != 1 && version != 2 && version != 3) {
			cout << "That is not an option, please select 1, 2, or 3" << endl;
			cin >> version;
		}
		if (version == 1) {
			rowSize = 9;
			colSize = 9;
			bombAmount = 10;
		}
		else if (version == 2) {
			rowSize = 16;
			colSize = 16;
			bombAmount = 40;
		} 
		else if (version == 3) {
			rowSize = 16;
			colSize = 30;
			bombAmount = 99;
		}
		gameLoop();
		char playAgain;
		cout << "Play Again? [y/n]" << endl;
		cin >> playAgain;
		if (playAgain != 'y') {
			play = false;
		}
	}
	
	
	return 0;
}

void gameLoop() {
	vector<vector<int>> markedBoard = makeGameboard();
	// first number is left and second number is top row
	// cout << markedBoard[0][8] << endl; 
	vector<vector<string>> playingBoard(rowSize, vector<string>(colSize, "-"));
	/*char newBoard[9][9];
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			newBoard[i][j] = '-';
		}
	}*/
	int guessedBombs = bombAmount;
	printGameboard(playingBoard, guessedBombs);
	int totalBombsLeft = bombAmount;
	bool notLost = true;
	while (notLost && totalBombsLeft > 0) {
		int row, column;
		char bomb;
		cout << "Select your next move: selecting bomb (y/n)" << endl;
		cin >> bomb;
		while (bomb != 'y' && bomb != 'n') {
			cout << "That is not an option, please select y or n" << endl;
			cin >> bomb;
		}
		cout << "Select your next move: pick the row" << endl;
		cin >> row;
		while (row > (rowSize - 1) || row < 0) {
			cout << "Out of bounds, please reselect" << endl;
			cin >> row;
		}
		cout << "Pick your column" << endl;
		cin >> column;
		while (column > (colSize - 1) || column < 0) {
			cout << "Out of bounds, please reselect" << endl;
			cin >> column;
		}
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
							else if (markedBoard[i - 1][j - 1] == -1) {
								notLost = false;
								break;
							}
							playingBoard[i - 1][j - 1] = aChar;
						}
						if (j != (colSize - 1) && playingBoard[i - 1][j + 1] != "*") {
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
							else if (markedBoard[i - 1][j + 1] == -1) {
								notLost = false;
								break;
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
							else if (markedBoard[i - 1][j] == -1) {
								notLost = false;
								break;
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
						else if (markedBoard[i][j - 1] == -1) {
							notLost = false;
							break;
						}
						playingBoard[i][j - 1] = aChar;
					}
					if (j != (colSize - 1) && playingBoard[i][j + 1] != "*") {
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
						else if (markedBoard[i][j + 1] == -1) {
							notLost = false;
							break;
						}
						playingBoard[i][j + 1] = aChar;
					}
					// bottom
					if (i != (rowSize - 1)) {
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
							else if (markedBoard[i + 1][j - 1] == -1) {
								notLost = false;
								break;
							}
							playingBoard[i + 1][j - 1] = aChar;
						}
						if (j != (colSize - 1) && playingBoard[i + 1][j + 1] != "*") {
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
							else if (markedBoard[i + 1][j + 1] == -1) {
								notLost = false;
								break;
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
							else if (markedBoard[i + 1][j] == -1) {
								notLost = false;
								break;
							}
							playingBoard[i + 1][j] = aChar;
						}
					}
					nodeSearching = nodeSearching->next;
				}
			}
		}
		if (notLost) {
			printGameboard(playingBoard, guessedBombs);
		}
		else {
			cout << "YOU LOST!" << endl;
			printLosingGameboard(markedBoard, playingBoard);
		}
	}
	if (totalBombsLeft == 0) {
		cout << "YOU WIN!" << endl;
	}
}

vector<vector<int>> makeGameboard() {
	//int board[9][9];
	vector<vector<int>> board (rowSize, vector<int>(colSize,0));
	int bombsRemaining = 10;

	int randomNumber = rand();
	cout << randomNumber << endl;

	srand(time(NULL));
	int secret = rand() % 10 + 1;
	cout << secret << endl;

	int i, j;

	for (i = 0; i < bombAmount;) {
		int random = rand() % (rowSize * colSize);
		int x = random / colSize;
		int y = random % colSize;
		cout << random <<": " << x << " " << y << endl;
		if (board[x][y] == -1) {
			continue;
		}
		board[x][y] = -1;
		i++;
	}

	cout << "making gameboard" << endl;

	for (i = 0; i < rowSize; ++i) {
		for (j = 0; j < colSize; ++j) {
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
				if (j != (colSize - 1)) {
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
			if (j != (colSize - 1)) {
				//cout << "j does not equal 8";
				// middle right
				if (board[i][j + 1] == -1) {
					bombsInVicinty++;
				}
			}
			// bottom
			if (i != (rowSize - 1)) {
				//cout << "i does not equal 8";
				if (j != 0) {
					//cout << "j does not equal 0";
					// bottom left
					if (board[i + 1][j - 1] == -1) {
						bombsInVicinty++;
					}
				}
				if (j != (colSize - 1)) {
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
	cout << "   ";
	for (i = 0; i < colSize; i++) {
		if (i < 10) {
			cout << i << "  ";
		}
		else {
			cout << i << " ";
		}
	}
	cout << endl;
	for (i = 0; i < rowSize; i++) {
		if (i < 10) {
			cout << i << "  ";
		}
		else {
			cout << i << " ";
		}
		for (j = 0; j < colSize; j++) {
			cout << currentBoard[i][j] << "  ";
		}
		cout << endl;
	}
}

/*void printGameboard(char currentBoard[][colSize]) {
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
}*/

void printLosingGameboard(vector<vector<int>> markedBoard, vector<vector<string>> currentBoard) {
	/*int i, j;
	cout << "  ";
	for (i = 0; i < colSize; i++) {
		cout << i << " ";
	}
	cout << endl;
	for (i = 0; i < rowSize; i++) {
		cout << i << " ";
		for (j = 0; j < colSize; j++) {
			if (markedBoard[i][j] == -1 && currentBoard[i][j] != "*") {
				cout << "B ";
			}
			else {
				cout << currentBoard[i][j] << " ";
			}
		}
		cout << endl;
	}*/

	int i, j;
	cout << "   ";
	for (i = 0; i < colSize; i++) {
		if (i < 10) {
			cout << i << "  ";
		}
		else {
			cout << i << " ";
		}
	}
	cout << endl;
	for (i = 0; i < rowSize; i++) {
		if (i < 10) {
			cout << i << "  ";
		}
		else {
			cout << i << " ";
		}
		for (j = 0; j < colSize; j++) {
			if (markedBoard[i][j] == -1 && currentBoard[i][j] != "*") {
				cout << "B  ";
			}
			else {
				cout << currentBoard[i][j] << "  ";
			}
		}
		cout << endl;
	}
}