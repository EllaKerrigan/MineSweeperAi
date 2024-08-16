// ======================================================================
// FILE:        MyAI.cpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#include "MyAI.hpp"

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent(), rowDimension(_rowDimension), colDimension(_colDimension), totalMines(_totalMines), agentX(_agentX), agentY(_agentY) {

	// Initialize Board based on row and col with value -2 and the initial X, Y value
	board = std::vector<std::vector<int>>(_rowDimension, std::vector<int>(_colDimension, -2));
	board[_agentX][_agentY] = 0;
};


Agent::Action MyAI::getAction( int number )
{
    // ======================================================================
    // YOUR CODE BEGINS
    // ======================================================================
	// Update the board
	board[agentX][agentY] = number;
	
	// print the board 
	// 					FOR DEBUG
	// printBoard();

	// Check if there are any 0’s on the board where the adjacent values aren’t uncovered yet
	Action uncoverAction = uncoverAdjacentZero();
	if (uncoverAction.action != LEAVE) {
		return uncoverAction;
	}

	//Check if there is a cell with value 1 and only one adjacent cell that isn’t uncovered
	Action flagAction = flagAdjacentOne();
	if (flagAction.action != LEAVE) {
		totalMines = totalMines - 1;
		return flagAction;
	}
	
	if (totalMines == 0) {
		Action uncoverRemaining = uncoverRemainingCells();
		return uncoverRemaining;
	}

	return {LEAVE, -1, -1};
}

    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================




// ======================================================================
// YOUR CODE BEGINS
// ======================================================================

Agent::Action MyAI::uncoverAdjacentZero() {
	for (int i = 0; i < rowDimension; ++i) {
		for (int j = 0; j < colDimension; ++j) {
			if (board[ i ][ j ] == 0) {
				if (i > 0 && j > 0 && board[ i - 1 ] [ j - 1 ] == -2) {
					agentX = i - 1; agentY = j - 1;
					return {UNCOVER, i - 1, j - 1};
				} else if (i > 0 && board[ i - 1 ][ j ] == -2) {	// Top Middle 
					agentX = i - 1; agentY = j;
					return {UNCOVER, i - 1, j};
				} else if (i > 0 && j < colDimension - 1 && board[ i - 1 ][ j + 1 ] == -2) {	// Top Right 
					agentX = i - 1; agentY = j + 1;
					return {UNCOVER, i - 1, j + 1};
				} else if (j > 0 && board[ i ][ j - 1 ] == -2) {	// Middle Left
					agentX = i; agentY = j - 1;
					return {UNCOVER, i, j - 1};
				} else if (j < colDimension - 1 && board[ i ][ j + 1 ] == -2) {
					agentX = i; agentY = j + 1;
					return {UNCOVER, i, j + 1};
				} else if (i < rowDimension - 1 && j > 0 && board[ i + 1 ][ j - 1 ] == -2) {
					agentX = i + 1; agentY = j - 1;
					return {UNCOVER, i + 1, j - 1};
				} else if (i < rowDimension - 1 && board[ i + 1 ][ j ] == -2) {	// Bottom Middle
					agentX = i + 1; agentY = j;
					return {UNCOVER, i + 1, j};
				} else if (i < rowDimension - 1 && j < colDimension - 1 && board[ i + 1 ][ j + 1] == -2) {	// Bottom Right
					agentX = i + 1; agentY = j + 1;
					return {UNCOVER, i + 1, j + 1};
				}
			}
		}
	}
	return {LEAVE, -1, -1};
}

Agent::Action MyAI::flagAdjacentOne() {
	for (int i = 1; i < rowDimension - 1; ++i) {
		for (int j = 1; j < colDimension - 1; ++j) {
			if (board[ i ][ j ] == 1) {
				std::vector<int> adjValues = getAdjacentCells(i, j);
			
				//for debugging
				for (int k = 0; k < adjValues.size(); ++k) {
					std::cout << adjValues[k] << " ";
				}
				std::cout << std::endl;

                // Check top-left
                if (isUncovered(adjValues[0]) && isUncovered(adjValues[1]) && adjValues[2] == -2 &&
                    isUncovered(adjValues[3]) && isUncovered(adjValues[4]) && isUncovered(adjValues[5]) && isUncovered(adjValues[6]) && isUncovered(adjValues[7])) {
                    agentX = i - 1;
                    agentY = j + 1;
                    return {FLAG, i - 1, j + 1};
                }
                // Check top-right
                else if (adjValues[0] == -2 && isUncovered(adjValues[1]) && isUncovered(adjValues[2]) &&
                         isUncovered(adjValues[3]) && isUncovered(adjValues[4]) && isUncovered(adjValues[5]) && isUncovered(adjValues[6]) && isUncovered(adjValues[7])) {
                    agentX = i - 1;
                    agentY = j - 1;
                    return {FLAG, i - 1, j - 1};
                }
                // Check bottom-right
                else if (isUncovered(adjValues[0]) && isUncovered(adjValues[1]) && isUncovered(adjValues[2]) &&
                         isUncovered(adjValues[3]) && isUncovered(adjValues[4]) && isUncovered(adjValues[5]) && isUncovered(adjValues[6]) && adjValues[7] == -2) {
                    agentX = i + 1;
                    agentY = j + 1;
                    return {FLAG, i + 1, j + 1};
                }
                // Check bottom-left
                else if (isUncovered(adjValues[0]) && isUncovered(adjValues[1]) && isUncovered(adjValues[2]) &&
                         isUncovered(adjValues[3]) && isUncovered(adjValues[4]) && adjValues[5] == -2 && isUncovered(adjValues[6]) && isUncovered(adjValues[7])) {
                    agentX = i + 1;
                    agentY = j - 1;
                    return {FLAG, i + 1, j - 1};
                }
			}
		}
	}
	return {LEAVE, -1, -1};
}

// Returns a container with all of the adjacent cells 
// Values that goes out of bound is set to -3
std::vector<int> MyAI::getAdjacentCells(int x, int y) {
	std::vector<int> adjacentcells;
	const int out_of_bound_value = -3;

	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (i == 0 && j == 0) continue;
			int newX = x + i;
			int newY = y + j;

			if (newX >= 0 && newY < rowDimension && newY >= 0 && newY < colDimension) {
				adjacentcells.push_back(board[newX][newY]);
			} else {
				adjacentcells.push_back(out_of_bound_value);
			}
		}
	}

	return adjacentcells;
}

Agent::Action MyAI::uncoverRemainingCells() {
	for (int i = 0; i < rowDimension; ++i) {
		for (int j = 0; j < colDimension; ++j) {
			if (board[ i ][ j ] == -2) {
				agentX = i; agentY = j;
				return {UNCOVER, i, j};
			}
		}
	}
	return {LEAVE, -1, -1};
}

// Returns whether the cell is uncovered or not 
bool MyAI::isUncovered(int number) {
	if (number == -1 || number == -2 || number == -3) {
		return false;
	}
	return true;
}

void MyAI::printBoard() {
	std::cout << "Board within the function" << std::endl;
	for (int i = 0; i < rowDimension; ++i) {
		for (int j = 0; j < colDimension; ++j) {
			if (board[i][j] == -2) {
				std::cout << "." << " ";
			}
			else if (board[i][j] == -1) {
				std:: cout << "-" << " ";
			}
			else {
				std::cout << board[i][j] << " ";
			}
		}
		std::cout << std::endl;
	}
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================
