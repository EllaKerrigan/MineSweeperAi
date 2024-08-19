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
#include <cassert>

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent(), rowDimension(_rowDimension), colDimension(_colDimension), totalMines(_totalMines), agentX(_agentX), agentY(_agentY) {

	// Initialize Board based on row and col with value -2 and the initial X, Y value
	board = std::vector<std::vector<int>>(_colDimension, std::vector<int>(_rowDimension, -2));
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
	//printBoard();

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
	
	// cout << "Here?" << endl;

	//Check if the value is equal to the number of flags in adjacent cells
	Action uncoverAdjacentFlag = uncoverRemainingAdjacent();
	if (uncoverAdjacentFlag.action != LEAVE) {
		//debug
        // cout << "Maybe here" << endl;

		return uncoverAdjacentFlag;
	}

	//Check one edge pattern
	Action onePattern = checkOneOneEdgePattern();
	if (onePattern.action != LEAVE) {
		return onePattern;
	}

	// 121 pattern
	Action handle121 = handle121Pattern();
	if (handle121.action != LEAVE) {
		totalMines = totalMines - 1;		
		return handle121;
	}

	// 1221 pattern
	Action handle1221 = handle1221Pattern();
	if (handle1221.action != LEAVE) {
		totalMines = totalMines - 1;
		return handle1221;
	}

	// 11 pattern
	Action handle11 = handle11Pattern();
	if (handle11.action != LEAVE) {
		//debug
		//cout << "pattern 11 executed" << endl;
		totalMines = totalMines - 1;
		return handle11;
	}

	// 12 plus pattern
	Action handle12Plus = handle12PlusPattern();
	if (handle12Plus.action != LEAVE) {
		totalMines = totalMines - 1;
		return handle12Plus;
	}

	// 11 plus pattern
	Action handle11Plus = handle11PlusPattern();
	if (handle11Plus.action != LEAVE) {
		totalMines = totalMines - 1;
		return handle11Plus;
	}
	
	//12c pattern
	Action handle12C = handle12CPattern();
	if (handle12C.action != LEAVE) {
		//cout << "Here" << endl;
		totalMines = totalMines-1;
		return handle12C;
	}

	if (totalMines == 0) {
		Action uncoverRemaining = uncoverRemainingCells();
		return uncoverRemaining;
	}

	Action guessAction = educatedGuess();
	if(guessAction.action != LEAVE){
		return guessAction;
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
				// for (int k = 0; k < adjValues.size(); ++k) {
				//	std::cout << adjValues[k] << " ";
				//}
				//std::cout << std::endl;

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

//Uncover when value = number of flags in adjacent cells
Agent::Action MyAI::uncoverRemainingAdjacent() {
	for (int i = 0; i < rowDimension - 1; ++i) {
		for (int j = 0; j < colDimension - 1; ++j) {
			if (board[i][j] != 0) {
				std::vector<int> adjCells = getAdjacentCells(i, j);
				int flagcount = 0;
				int coveredcount = 0;
			
				//Debug
				//cout << "Seg fault Here?" << endl;

				for (int k = 0; k < adjCells.size(); ++k) {
					if (adjCells[k] == -1) {
						++flagcount;
					}
					else if (adjCells[k] == -2) {
						++coveredcount;
					}
				}
			
				if (board[i][j] == flagcount) {
					//Debug 
					// cout << board[i][j] << " is equal to " << count << " i " << i << " j " << j << endl;

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
				else if (board[i][j] == flagcount + coveredcount) {
					if (i > 0 && j > 0 && board[ i - 1 ] [ j - 1 ] == -2) {
						agentX = i - 1; agentY = j - 1;
						totalMines = totalMines - 1;
						return {FLAG, i - 1, j - 1};
					} else if (i > 0 && board[ i - 1 ][ j ] == -2) {	// Top Middle 
						totalMines = totalMines - 1;
						agentX = i - 1; agentY = j;
						return {FLAG, i - 1, j};
					} else if (i > 0 && j < colDimension - 1 && board[ i - 1 ][ j + 1 ] == -2) {	// Top Right 
						totalMines = totalMines - 1;
						agentX = i - 1; agentY = j + 1;
						return {FLAG, i - 1, j + 1};
					} else if (j > 0 && board[ i ][ j - 1 ] == -2) {	// Middle Left
						totalMines = totalMines - 1;
						agentX = i; agentY = j - 1;
						return {FLAG, i, j - 1};
					} else if (j < colDimension - 1 && board[ i ][ j + 1 ] == -2) {
						totalMines = totalMines - 1;
						agentX = i; agentY = j + 1;
						return {FLAG, i, j + 1};
					} else if (i < rowDimension - 1 && j > 0 && board[ i + 1 ][ j - 1 ] == -2) {
						totalMines = totalMines - 1;
						agentX = i + 1; agentY = j - 1;
						return {FLAG, i + 1, j - 1};
					} else if (i < rowDimension - 1 && board[ i + 1 ][ j ] == -2) {	// Bottom Middle
						totalMines = totalMines - 1;
						agentX = i + 1; agentY = j;
						return {FLAG, i + 1, j};
					} else if (i < rowDimension - 1 && j < colDimension - 1 && board[ i + 1 ][ j + 1] == -2) {	// Bottom Right
						totalMines = totalMines - 1;
						agentX = i + 1; agentY = j + 1;
						return {FLAG, i + 1, j + 1};
					}
				}
			}
		}
	}
	return {LEAVE, -1, -1};
}

Agent::Action MyAI::checkOneOneEdgePattern() {
    // Check top edge
    for (int j = 0; j < colDimension - 1; ++j) {
        if (board[0][j] == 1 && board[0][j + 1] == 1) {
            if (board[1][j] == -2) {
                agentX = 1;
                agentY = j;
                return {UNCOVER, 1, j};
            }
            if (board[1][j + 1] == -2) {
                agentX = 1;
                agentY = j + 1;
                return {UNCOVER, 1, j + 1};
            }
        }
    }

    // Check bottom edge
    for (int j = 0; j < colDimension - 1; ++j) {
        if (board[rowDimension - 1][j] == 1 && board[rowDimension - 1][j + 1] == 1) {
            if (board[rowDimension - 2][j] == -2) {
                agentX = rowDimension - 2;
                agentY = j;
                return {UNCOVER, rowDimension - 2, j};
            }
            if (board[rowDimension - 2][j + 1] == -2) {
                agentX = rowDimension - 2;
                agentY = j + 1;
                return {UNCOVER, rowDimension - 2, j + 1};
            }
        }
    }
// Check left edge
    for (int i = 0; i < rowDimension - 1; ++i) {
        if (board[i][0] == 1 && board[i + 1][0] == 1) {
            if (board[i][1] == -2) {
                agentX = i;
                agentY = 1;
                return {UNCOVER, i, 1};
            }
            if (board[i + 1][1] == -2) {
                agentX = i + 1;
                agentY = 1;
                return {UNCOVER, i + 1, 1};
            }
        }
    }

    // Check right edge
    for (int i = 0; i < rowDimension - 1; ++i) {
        if (board[i][colDimension - 1] == 1 && board[i + 1][colDimension - 1] == 1) {
            if (board[i][colDimension - 2] == -2) {
                agentX = i;
                agentY = colDimension - 2;
                return {UNCOVER, i, colDimension - 2};
            }
            if (board[i + 1][colDimension - 2] == -2) {
                agentX = i + 1;
                agentY = colDimension - 2;
                return {UNCOVER, i + 1, colDimension - 2};
            }
        }
    }

    return {LEAVE, -1, -1};
}

Agent::Action MyAI::handle121Pattern() {
    for (int i = 1; i < rowDimension - 1; ++i) {
        for (int j = 1; j < colDimension - 1; ++j) {
            // Checking horizontally for the pattern
            if (board[i][j-1] == 1 && board[i][j] == 2 && board[i][j+1] == 1) {
				if (isUncovered(board[i+1][j-1]) && isUncovered(board[i+1][j+1])) {
					if (board[i-1][j-1] == -2) {
						agentX = i - 1; agentY = j - 1;
						return {FLAG, i - 1, j - 1};
					}
					else if (board[i-1][j-1] == -1 && board[i-1][j+1] == -2) {
						agentX = i - 1; agentY = j + 1;
						return {FLAG, i - 1, j + 1};
					}
				}
				else if (isUncovered(board[i-1][j-1]) && isUncovered(board[i-1][j+1])) {
					if (board[i+1][j-1] == -2) {
						agentX = i + 1; agentY = j - 1;
						return {FLAG, i + 1, j - 1};
					}
					else if (board[i+1][j-1] == -1 && board[i+1][j+1] == -2) {
						agentX = i + 1; agentY = j + 1;
						return {FLAG, i + 1, j + 1};
					}
				}
			}
			// Checking vertically for the pattern
			else if (board[i-1][j] == 1 && board[i][j] == 2 && board[i+1][j] == 1) {
				if (isUncovered(board[i-1][j-1]) && isUncovered(board[i+1][j-1])) {
					if (board[i-1][j+1] == -2) {
						agentX = i - 1; agentY = j + 1;
						return {FLAG, i - 1, j + 1};
					}
					else if (board[i-1][j+1] == -1 && board[i+1][j+1] == -2) {
						agentX = i + 1; agentY = j + 1;
						return {FLAG, i + 1, j + 1};
					}
				}
				else if (isUncovered(board[i-1][j+1]) && isUncovered(board[i+1][j+1])) {
					if (board[i-1][j-1] == -2) {
						agentX = i - 1; agentY = j - 1;
						return {FLAG, i - 1, j - 1};
					}
					else if (board[i-1][j-1] == -1 && board[i+1][j-1] == -2) {
						agentX = i + 1; agentY = j - 1;
						return {FLAG, i + 1, j - 1};
					}
				}
			}
		}
	}
    return {LEAVE, -1, -1};
}

//Handle 1221 pattern
Agent::Action MyAI::handle1221Pattern() {
	for (int i = 1; i < rowDimension - 2; ++i) {
		for (int j = 1; j < colDimension - 2; ++j) {
			if (board[i][j-1] == 1 && board[i][j] == 2 && board[i][j+1] == 2 && board[i][j+2] == 1) {
				
				//Debug
				//cout << "1221 PATTERN" << endl;

				if (isUncovered(board[i-1][j]) && isUncovered(board[i-1][j+1])) {
					if (board[i+1][j] == -2) {
						agentX = i + 1; agentY = j;
						return {FLAG, i + 1, j};
					}
					else if (board[i+1][j] == -1 && board[i+1][j+1] == -2) {
						agentX = i + 1; agentY = j + 1;
						return {FLAG, i + 1, j + 1};
					}
				}
				else if (isUncovered(board[i+1][j]) && isUncovered(board[i+1][j+1])) {
					if (board[i-1][j] == -2) {
						agentX = i - 1; agentY = j;
						return {FLAG, i - 1, j};
					}
					else if (board[i-1][j] == -1 && board[i-1][j+1] == -2) {
						agentX = i - 1; agentY = j + 1;
						return {FLAG, i - 1, j + 1};
					}
				}
			}
			else if (board[i-1][j] == 1 && board[i][j] == 2 && board[i+1][j] == 2 && board[i+2][j] == 1) {
				
				//Debug
				//cout << "1221 PATTERN V" << endl;

				if (isUncovered(board[i][j+1]) && isUncovered(board[i+1][j+1])) {
					if (board[i][j-1] == -2) {
						agentX = i; agentY = j - 1;
						return {FLAG, i, j - 1};
					}
					else if (board[i][j-1] == -1 && board[i+1][j-1] == -2) {
						agentX = i + 1; agentY = j - 1;
						return {FLAG, i + 1, j - 1};
					}
				}
				else if (isUncovered(board[i][j-1]) && isUncovered(board[i+1][j-1])) {
					if (board[i][j+1] == -2) {
						agentX = i; agentY = j + 1;
						return {FLAG, i, j + 1};
					}
					else if (board[i][j+1] == -1 && board[i+1][j+1] == -2) {
						agentX = i + 1; agentY = j + 1;
						return {FLAG, i + 1, j + 1};
					}
				}
			}
		}
	}
	return {LEAVE, -1, -1};
}

//Handle pattern for 11
Agent::Action MyAI::handle11Pattern() {
	for (int i = 1; i < rowDimension - 1; ++i) {
		for (int j = 1; j < colDimension - 1; ++j) {
			if (board[i][j] == 1) {
				if (board[i][j-1] == 1) {
					if (isUncovered(board[i+1][j-1]) && isUncovered(board[i+1][j]) && isUncovered(board[i+1][j+1])) {
						if (board[i-1][j+1] == -2) {
							agentX = i - 1; agentY = j + 1;
							return {UNCOVER, agentX, agentY};
						}
					}
					else if (isUncovered(board[i-1][j-1]) && isUncovered(board[i-1][j]) && isUncovered(board[i-1][j+1])) {
						if (board[i+1][j+1] == -2) {
							agentX = i + 1; agentY = j + 1;
							return {UNCOVER, agentX, agentY};
						}
					}
				}
				else if (board[i][j+1] == 1) {
					if (isUncovered(board[i+1][j-1]) && isUncovered(board[i+1][j]) && isUncovered(board[i+1][j+1])) {
						if (board[i-1][j-1] == -2) {
							agentX = i - 1; agentY = j - 1;
							return {UNCOVER, agentX, agentY};
						}
					}
					else if (isUncovered(board[i-1][j-1]) && isUncovered(board[i-1][j]) && isUncovered(board[i-1][j+1])) {
						if (board[i+1][j-1] == -2) {
							agentX = i + 1; agentY = j - 1;
							return {UNCOVER, agentX, agentY};
						}
					}
				}
				else if (board[i-1][j] == 1) {
					if (isUncovered(board[i-1][j+1]) && isUncovered(board[i][j+1]) && isUncovered(board[i+1][j+1])) {
						if (board[i+1][j-1] == -2) {
							agentX = i + 1; agentY = j - 1;
							return {UNCOVER, agentX, agentY};
						}
					}
					else if (isUncovered(board[i-1][j-1]) && isUncovered(board[i][j-1]) && isUncovered(board[i+1][j-1])) {
						if (board[i+1][j+1] == -2) {
							agentX = i + 1; agentY = j + 1;
							return {UNCOVER, agentX, agentY};
						}
					}
				}
				else if (board[i+1][j] == 1) {
					if (isUncovered(board[i-1][j+1]) && isUncovered(board[i][j+1]) && isUncovered(board[i+1][j+1])) {
						if (board[i-1][j-1] == -2) {
							agentX = i - 1; agentY = j - 1;
							return {UNCOVER, agentX, agentY};
						}
					}
					else if (isUncovered(board[i-1][j-1]) && isUncovered(board[i][j-1]) && isUncovered(board[i+1][j-1])) {
						if (board[i-1][j+1] == -2) {
							agentX = i - 1; agentY = j + 1;
							return {UNCOVER, agentX, agentY};
						}
					}
				}
			}
		}
	}
	return {LEAVE, -1, -1};
}

//Handle Pattern 12C

Agent::Action MyAI::handle12CPattern() {
	for (int i = 1; i < rowDimension - 1; ++i) {
		for (int j = 1; j < colDimension - 1; ++j) {
			
			if (board[i][j] == 2) {
				if (board[i][j-1] == 1) {
					if (isUncovered(board[i+1][j-1]) && isUncovered(board[i+1][j]) && isUncovered(board[i+1][j+1]) && isUncovered(board[i][j+1])) {
						if (board[i-1][j+1] == -2) {
							agentX = i - 1; agentY = j + 1;
							return {FLAG, agentX, agentY};
						}
					}
					else if (isUncovered(board[i-1][j-1]) && isUncovered(board[i-1][j]) && isUncovered(board[i-1][j+1]) && isUncovered(board[i][j+1])) {
						if (board[i+1][j+1] == -2) {
							agentX = i + 1; agentY = j + 1;
							return {FLAG, agentX, agentY};
						}
					}
				}
				else if (board[i][j+1] == 1) {
					if (isUncovered(board[i+1][j-1]) && isUncovered(board[i+1][j]) && isUncovered(board[i+1][j+1]) && isUncovered(board[i][j-1])) {
						if (board[i-1][j-1] == -2) {
							agentX = i - 1; agentY = j - 1;
							return {FLAG, agentX, agentY};
						}
					}
					else if (isUncovered(board[i-1][j-1]) && isUncovered(board[i-1][j]) && isUncovered(board[i-1][j+1]) && isUncovered(board[i][j-1])) {
						if (board[i+1][j-1] == -2) {
							agentX = i + 1; agentY = j - 1;
							return {FLAG, agentX, agentY};
						}
					}
				}
				else if (board[i-1][j] == 1) {
					if (isUncovered(board[i-1][j-1]) && isUncovered(board[i][j-1]) && isUncovered(board[i+1][j-1]) && isUncovered(board[i-1][j])) {
						if (board[i+1][j+1] == -2) {
							agentX = i + 1; agentY = j + 1;
							return {FLAG, agentX, agentY};
						}
					}
					else if (isUncovered(board[i-1][j+1]) && isUncovered(board[i][j+1]) && isUncovered(board[i+1][j+1]) && isUncovered(board[i+1][j])) {
						if (board[i+1][j-1] == -2) {
							agentX = i + 1; agentY = j - 1;
							return {FLAG, agentX, agentY};
						}
					}
				}
				else if (board[i+1][j] == 1) {
					if (isUncovered(board[i-1][j-1]) && isUncovered(board[i][j-1]) && isUncovered(board[i+1][j-1]) && isUncovered(board[i-1][j])) {
						if (board[i-1][j+1] == -2) {
							agentX = i - 1; agentY = j + 1;
							return {FLAG, agentX, agentY};
						}
					}
					else if (isUncovered(board[i-1][j+1]) && isUncovered(board[i][j+1]) && isUncovered(board[i+1][j+1]) && isUncovered(board[i-1][j])) {
						if (board[i-1][j-1] == -2) {
							agentX = i - 1; agentY = j - 1;
							return {FLAG, agentX, agentY};
						}
					}
				}
			}
		}
	}
	return {LEAVE, -1, -1};
}			


Agent::Action MyAI::handle12PlusPattern() {
    for (int i = 1; i < rowDimension - 1; ++i) {
        for (int j = 1; j < colDimension - 1; ++j) {
            // Check board horizontally 
            if (board[i][j] == 1 && board[i][j + 1] == 2 && (board[i][j + 2] == 1 || board[i][j + 2] == -1)) {
                if (isUncovered(board[i + 1][j]) && isUncovered(board[i + 1][j + 2])) {
                    if (board[i - 1][j] == -2) {
                        agentX = i - 1;
                        agentY = j;
                        return {FLAG, agentX, agentY};
                    }
                    else if (board[i - 1][j + 2] == -2) {
                        agentX = i - 1;
                        agentY = j + 2;
                        return {FLAG, agentX, agentY};
                    }
                }
                else if (isUncovered(board[i - 1][j]) && isUncovered(board[i - 1][j + 2])) {
                    if (board[i + 1][j] == -2) {
                        agentX = i + 1;
                        agentY = j;
                        return {FLAG, agentX, agentY};
                    }
                    else if (board[i + 1][j + 2] == -2) {
                        agentX = i + 1;
                        agentY = j + 2;
                        return {FLAG, agentX, agentY};
                    }
                }
            }
			// Check board vertically pattern
            else if (board[i][j] == 1 && board[i + 1][j] == 2 && (board[i + 2][j] == 1 || board[i + 2][j] == -1)) {
                if (isUncovered(board[i][j + 1]) && isUncovered(board[i + 2][j + 1])) {
                    if (board[i][j - 1] == -2) {
                        agentX = i;
                        agentY = j - 1;
                        return {FLAG, agentX, agentY};
                    }
                    else if (board[i + 2][j - 1] == -2) {
                        agentX = i + 2;
                        agentY = j - 1;
                        return {FLAG, agentX, agentY};
                    }
                }
                else if (isUncovered(board[i][j - 1]) && isUncovered(board[i + 2][j - 1])) {
                    if (board[i][j + 1] == -2) {
                        agentX = i;
                        agentY = j + 1;
                        return {FLAG, agentX, agentY};
                    }
                    else if (board[i + 2][j + 1] == -2) {
                        agentX = i + 2;
                        agentY = j + 1;
                        return {FLAG, agentX, agentY};
                    }
                }
            }
        }
    }
    return {LEAVE, -1, -1};
}

Agent::Action MyAI::handle11PlusPattern() {
    for (int i = 1; i < rowDimension - 2; ++i) {
        for (int j = 1; j < colDimension - 2; ++j) {
            // Check board horizontally 
            if (board[i][j] == 1 && board[i][j + 1] == 1 && (board[i][j + 2] == 1 || board[i][j + 2] == -1)) {
                if (isUncovered(board[i + 1][j]) && isUncovered(board[i + 1][j + 1])) {
                    if (board[i - 1][j] == -2) {
                        agentX = i - 1;
                        agentY = j;
                        return {FLAG, agentX, agentY};
                    }
                    else if (board[i - 1][j + 1] == -2) {
                        agentX = i - 1;
                        agentY = j + 1;
                        return {FLAG, agentX, agentY};
                    }
                }
                else if (isUncovered(board[i + 1][j]) && isUncovered(board[i + 1][j + 1])) {
                    if (board[i + 1][j] == -2) {
                        agentX = i + 1;
                        agentY = j;
                        return {FLAG, agentX, agentY};
                    }
                    else if (board[i + 1][j + 1] == -2) {
                        agentX = i + 1;
                        agentY = j + 1;
                        return {FLAG, agentX, agentY};
                    }
                }
            }
// Check board vertically
            else if (board[i][j] == 1 && board[i + 1][j] == 1 && (board[i + 2][j] == 1 || board[i + 2][j] == -1)) {
                if (isUncovered(board[i][j + 1]) && isUncovered(board[i + 2][j + 1])) {
                    if (board[i][j - 1] == -2) {
                        agentX = i;
                        agentY = j - 1;
                        return {FLAG, agentX, agentY};
                    }
                    else if (board[i + 2][j - 1] == -2) {
                        agentX = i + 2;
                        agentY = j - 1;
                        return {FLAG, agentX, agentY};
                    }
                }
                else if (isUncovered(board[i][j - 1]) && isUncovered(board[i + 2][j - 1])) {
                    if (board[i][j + 1] == -2) {
                        agentX = i;
                        agentY = j + 1;
                        return {FLAG, agentX, agentY};
                    }
                    else if (board[i + 2][j + 1] == -2) {
                        agentX = i + 2;
                        agentY = j + 1;
                        return {FLAG, agentX, agentY};
                    }
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


Agent::Action MyAI::educatedGuess() {
    double minProbability = 1.0;
    int bestX = -1;
    int bestY = -1;

    for (int i = 0; i < rowDimension; ++i) {
        for (int j = 0; j < colDimension; ++j) {
            if (board[i][j] == -2) { // check only uncovered cells
                double probability = calculateMineProbability(i, j);
                //find lowest probability
                if (probability < minProbability) {
                    minProbability = probability;
                    bestX = i;
                    bestY = j;
                }
            }
        }
    }
    if (bestX != -1 && bestY != -1) {
        agentX = bestX;
        agentY = bestY;
        return {UNCOVER, bestX, bestY};
    }

	for (int i = 0; i < rowDimension - 1; ++i) {
		for (int j = 0; j < colDimension - 1; ++j) {
			if (board[i][j] == -2) {
				return {UNCOVER, i, j};
			}
		}
	}

    return {LEAVE, -1, -1};
}

double MyAI::calculateMineProbability(int x, int y) {
    int mineCount = 0; // # of potential mines
    int unknownCount = 0; // # of unknown cells

    // Loop through adjacent cells to count mines and unknowns 
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int adjX = x + i;
            int adjY = y + j;

			//bound check - calculate mine and unkown counts 
            if (adjX >= 0 && adjX < rowDimension && adjY >= 0 && adjY < colDimension) {
                if (board[adjX][adjY] > 0) { 
                    mineCount += board[adjX][adjY];
                } else if (board[adjX][adjY] == -2) { 
                    unknownCount++;
                }
            }
        }
    }
	//probablity of a cell being a mine 
    return (double)mineCount / unknownCount;
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



