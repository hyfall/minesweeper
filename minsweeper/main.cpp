#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <string>

using namespace std;

int rowSize = 9;
int colSize = 9;
int bombAmount = 10;

string gameLoop(string mapSeed);
vector<vector<int>> makeGameboard(int firstRow, int firstColumn, time_t timeValue);
vector<vector<int>> makeGameboard(string mapSeed);
void printGameboard(vector<vector<string>> currentBoard, int guessedBombs);
void printLosingGameboard(vector<vector<int>> markedBoard, vector<vector<string>> currentBoard);

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
		string mapSeed = "";
		cout << "What difficulty would you like to play?" << endl << "1) Easy (9x9)" << endl << "2) Medium (16x16)" << endl << "3) Hard (16x30)" << endl << "4) Enter Map Seed" << endl;
		cin >> version;
		while (version != 1 && version != 2 && version != 3 && version != 4) {
			cout << "That is not an option, please select 1, 2, 3, or 4" << endl;
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
		else if (version == 4) {
			cout << "Enter map seed:" << endl;
			cin >> mapSeed;
			rowSize= stoi(mapSeed.substr(mapSeed.length() - 6, 2));
			colSize = stoi(mapSeed.substr(mapSeed.length() - 4, 2));
			bombAmount = stoi(mapSeed.substr(mapSeed.length() - 2));
		} 
		mapSeed = gameLoop(mapSeed);
		char playAgain;
		cout << "Play Again? [y/n] or [s]hare map seed" << endl;
		cin >> playAgain;
		if (playAgain == 's') {
			cout << "Map Seed: " << mapSeed << endl;
			cout << "Play Again? [y/n]" << endl;
			cin >> playAgain;
		}
		if (playAgain != 'y') {
			play = false;
		}
	}
	
	
	return 0;
}

string gameLoop(string mapSeed) {
	
	vector<vector<string>> playingBoard(rowSize, vector<string>(colSize, "-"));
	int guessedBombs = bombAmount;
	printGameboard(playingBoard, guessedBombs);
	vector<vector<int>> markedBoard;

	int totalBombsLeft = bombAmount;
	bool notLost = true, firstTurn = true;
	string newMapSeed = "";
	while (notLost && totalBombsLeft > 0) {
		int row, column;
		char bomb;
		if (!firstTurn) {
			cout << "Select your next move: selecting bomb (y/n)" << endl;
			cin >> bomb;
			while (bomb != 'y' && bomb != 'n') {
				cout << "That is not an option, please select y or n" << endl;
				cin >> bomb;
			}
		}
		else {
			bomb = 'n';
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
		if (firstTurn && mapSeed.empty()) {
			time_t timeValue = time(NULL);
			markedBoard = makeGameboard(row, column, timeValue);
			//make mapseed
			string rowString = to_string(row), columnString = to_string(column);
			if (rowString.length() == 1) {
				rowString = "0" + to_string(row);
			}
			if (columnString.length() == 1) {
				columnString = "0" + to_string(column);
			}
			string rowSizeString = to_string(rowSize), columnSizeString = to_string(colSize);
			if (rowSizeString.length() == 1) {
				rowSizeString = "0" + to_string(rowSize);
			}
			if (columnSizeString.length() == 1) {
				columnSizeString = "0" + to_string(colSize);
			}
			newMapSeed = to_string(timeValue) + rowString + columnString + rowSizeString + columnSizeString + to_string(bombAmount);

			firstTurn = false;
		}
		else if (firstTurn && !mapSeed.empty()) {
			newMapSeed = mapSeed + "";
			mapSeed.erase(mapSeed.length() - 6);
			markedBoard = makeGameboard(mapSeed);

			firstTurn = false;
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
			notLost = false;
		}
		else {
			bool checkAround = false;
			if (playingBoard[row][column] != "*" && playingBoard[row][column] != "-") {
				checkAround = true;;
			}
			if (playingBoard[row][column] == "*") {
				guessedBombs++;
			}
			char aChar = '0' + newTile;
			playingBoard[row][column] = aChar;
			struct Node {
				int r;
				int c;
				struct Node* next;
			};
			// search for all neighboring blocks if zero
			if (newTile == 0 || checkAround) {
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
						// middle left
						aChar = '0' + markedBoard[i][j - 1];
						if (markedBoard[i][j - 1] == 0 && playingBoard[i][j - 1] == "-") {
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
						// middle right
						aChar = '0' + markedBoard[i][j + 1];
						if (markedBoard[i][j + 1] == 0 && playingBoard[i][j + 1] == "-") {
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
						if (j != 0 && playingBoard[i + 1][j - 1] != "*") {
							// bottom left
							aChar = '0' + markedBoard[i + 1][j - 1];
							if (markedBoard[i + 1][j - 1] == 0 && playingBoard[i + 1][j - 1] == "-") {
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
							// bottom right
							aChar = '0' + markedBoard[i + 1][j + 1];
							if (markedBoard[i + 1][j + 1] == 0 && playingBoard[i + 1][j + 1] == "-") {
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
			return newMapSeed;
		}
	}
	if (totalBombsLeft == 0) {
		cout << "YOU WIN!" << endl;
		return newMapSeed;
	}
}

vector<vector<int>> makeGameboard(int firstRow, int firstColumn, time_t timeValue) {
	vector<vector<int>> board(rowSize, vector<int>(colSize, 0));

	srand(timeValue);

	int i, j;

	for (i = 0; i < bombAmount;) {
		int random = rand() % (rowSize * colSize);
		int x = random / colSize;
		int y = random % colSize;
		if (board[x][y] == -1) {
			continue;
		}
		// logging for if errors
		/*cout << "x: " << x << " fR " << (firstRow) << endl;
		cout << "y: " << y << " fC " << (firstColumn) << endl;
		if (firstRow - 1 <= x && x <= firstRow + 1) {
			cout << " in x range " << endl;
		}
		if (firstColumn - 1 <= y && y <= firstColumn + 1) {
			cout << " in y range " << endl;
		}*/
		// check if around first row and column
		if (firstRow - 1 <= x && x <= firstRow + 1 && firstColumn - 1 <= y && y <= firstColumn + 1) {
			continue;
		}

		board[x][y] = -1;
		i++;
	}

	//cout << "making gameboard" << endl;

	for (i = 0; i < rowSize; ++i) {
		for (j = 0; j < colSize; ++j) {
			if (board[i][j] == -1) {
				continue;
			}
			int bombsInVicinty = 0;
			if (i != 0) {
				if (j != 0) {
					// top left
					if (board[i - 1][j - 1] == -1) {
						bombsInVicinty++;
					}
				}
				if (j != (colSize - 1)) {
					// top right
					if (board[i - 1][j + 1] == -1) {
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
				// middle left
				if (board[i][j - 1] == -1) {
					bombsInVicinty++;
				}
			}
			if (j != (colSize - 1)) {
				// middle right
				if (board[i][j + 1] == -1) {
					bombsInVicinty++;
				}
			}
			// bottom
			if (i != (rowSize - 1)) {
				if (j != 0) {
					// bottom left
					if (board[i + 1][j - 1] == -1) {
						bombsInVicinty++;
					}
				}
				if (j != (colSize - 1)) {
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
			board[i][j] = bombsInVicinty;
		}
	}

	return board;
}

vector<vector<int>> makeGameboard(string mapSeed) {
	vector<vector<int>> board(rowSize, vector<int>(colSize, 0));

	// map seed probably stores srand info and the first row/column
	

	int firstRow = stoi(mapSeed.substr(mapSeed.length() - 4, 2));
	int firstColumn = stoi(mapSeed.substr(mapSeed.length() - 2));
	cout << firstRow << " " << firstColumn << endl;
	
	mapSeed.erase(mapSeed.length() - 4);
	int timeValue = stoi(mapSeed);


	//cout << "time value? " << timeValue << endl;	

	srand(timeValue);

	int i, j;

	for (i = 0; i < bombAmount;) {
		int random = rand() % (rowSize * colSize);
		int x = random / colSize;
		int y = random % colSize;
		cout << i << ": " << x << " " << y << endl;
		if (board[x][y] == -1) {
			continue;
		}
		// logging for if errors
		/*cout << "x: " << x << " fR " << (firstRow) << endl;
		cout << "y: " << y << " fC " << (firstColumn) << endl;
		if (firstRow - 1 <= x && x <= firstRow + 1) {
			cout << " in x range " << endl;
		}
		if (firstColumn - 1 <= y && y <= firstColumn + 1) {
			cout << " in y range " << endl;
		}*/
		// check if around first row and column
		//int firstRow = 0;
		//int firstColumn = 0;
		if (firstRow - 1 <= x && x <= firstRow + 1 && firstColumn - 1 <= y && y <= firstColumn + 1) {
			continue;
		}

		board[x][y] = -1;
		i++;
	}

	//cout << "making gameboard" << endl;

	for (i = 0; i < rowSize; ++i) {
		for (j = 0; j < colSize; ++j) {
			if (board[i][j] == -1) {
				continue;
			}
			int bombsInVicinty = 0;
			if (i != 0) {
				if (j != 0) {
					// top left
					if (board[i - 1][j - 1] == -1) {
						bombsInVicinty++;
					}
				}
				if (j != (colSize - 1)) {
					// top right
					if (board[i - 1][j + 1] == -1) {
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
				// middle left
				if (board[i][j - 1] == -1) {
					bombsInVicinty++;
				}
			}
			if (j != (colSize - 1)) {
				// middle right
				if (board[i][j + 1] == -1) {
					bombsInVicinty++;
				}
			}
			// bottom
			if (i != (rowSize - 1)) {
				if (j != 0) {
					// bottom left
					if (board[i + 1][j - 1] == -1) {
						bombsInVicinty++;
					}
				}
				if (j != (colSize - 1)) {
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
			board[i][j] = bombsInVicinty;
		}
	}

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

void printLosingGameboard(vector<vector<int>> markedBoard, vector<vector<string>> currentBoard) {

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