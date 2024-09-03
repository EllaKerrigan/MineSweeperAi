
/// ======================================================================
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
// =====================================================================
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

	//if (rowDimension > 16 || colDimension > 16) {
	//	printBoard();
	//}

	// Check if there are any 0’s on the board where the adjacent values aren’t uncovered yet
	Action uncoverAction = uncoverAdjacentZero();
	if (uncoverAction.action != LEAVE) {
		//cout << "uncoverAdjacentZero" << endl;
		return uncoverAction;
	}

	//Check if there is a cell with value 1 and only one adjacent cell that isn’t uncovered
	//if (colDimension > 16) {
	Action flagAction = flagAdjacentOne();
	if (flagAction.action != LEAVE) {
		//cout << "flagAdjacentOne" << endl;
		totalMines = totalMines - 1;
		return flagAction;
	}
//}
	
	// cout << "Here?" << endl;

	//Check if the value is equal to the number of flags in adjacent cells
	//if (colDimension > 16) {
	Action uncoverAdjacentFlag = uncoverRemainingAdjacent();
	if (uncoverAdjacentFlag.action != LEAVE) {
		//debug
        //cout << "uncoverAdjacentFlag" << endl;

		return uncoverAdjacentFlag;
	}
//}

	//Check one edge pattern
	//Action onePattern = checkOneOneEdgePattern();
	//if (onePattern.action != LEAVE) {
	//	//cout << "onePattern" << endl;
	//	return onePattern;
	//}

	// 11 pattern
	Action handle11 = handle11Pattern();
	if (handle11.action != LEAVE) {
		//debug
		//cout << "pattern 11 executed" << endl;
		totalMines = totalMines - 1;
		return handle11;
	}
	
	//12c pattern
	Action handle12C = handle12CPattern();
	if (handle12C.action != LEAVE) {
		//cout << "Handle12CPattern" << endl;
		totalMines = totalMines-1;
		return handle12C;
	}

	// 12 plus pattern
	Action handle12Plus = handle12PlusPattern();
	if (handle12Plus.action != LEAVE) {
		totalMines = totalMines - 1;
		return handle12Plus;
	}

	// 121 pattern
	Action handle121 = handle121Pattern();
	if (handle121.action != LEAVE) {
		//cout << "handle121Pattern" << endl;
		totalMines = totalMines - 1;
		return handle121;
	}

	// 1221 pattern
	Action handle1221 = handle1221Pattern();
	if (handle1221.action != LEAVE) {
		//cout << "handle1221Pattern" << endl;
		totalMines = totalMines - 1;
		return handle1221;
	}
	
	if (totalMines == 0) {
		//cout << "uncoverRemaining" << endl;
		Action uncoverRemaining = uncoverRemainingCells();
		return uncoverRemaining;
	}

	//if (colDimension > 16) {
	Action guessAction = educatedGuess();
	if(guessAction.action != LEAVE){
		//cout << "Educated Guess" << endl;
		return guessAction;
	}
//}
	//cout << "Leave" << endl;
	return {LEAVE, -1, -1};
}

    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================




// ======================================================================
// YOUR CODE BEGINS
// ======================================================================

Agent::Action MyAI::uncoverAdjacentZero() {
	for (int i = 0; i < colDimension; ++i) {
		for (int j = 0; j < rowDimension; ++j) {
			if (board[ i ][ j ] == 0) {
				if (i > 0 && j > 0 && board[ i - 1 ] [ j - 1 ] == -2) {
					agentX = i - 1; agentY = j - 1;
					return {UNCOVER, i - 1, j - 1};
				} else if (i > 0 && board[ i - 1 ][ j ] == -2) {	// Top Middle 
					agentX = i - 1; agentY = j;
					return {UNCOVER, i - 1, j};
				} else if (i > 0 && j < rowDimension - 1 && board[ i - 1 ][ j + 1 ] == -2) {	// Top Right 
					agentX = i - 1; agentY = j + 1;
					return {UNCOVER, i - 1, j + 1};
				} else if (j > 0 && board[ i ][ j - 1 ] == -2) {	// Middle Left
					agentX = i; agentY = j - 1;
					return {UNCOVER, i, j - 1};
				} else if (j < rowDimension - 1 && board[ i ][ j + 1 ] == -2) {
					agentX = i; agentY = j + 1;
					return {UNCOVER, i, j + 1};
				} else if (i < colDimension - 1 && j > 0 && board[ i + 1 ][ j - 1 ] == -2) {
					agentX = i + 1; agentY = j - 1;
					return {UNCOVER, i + 1, j - 1};
				} else if (i < colDimension - 1 && board[ i + 1 ][ j ] == -2) {	// Bottom Middle
					agentX = i + 1; agentY = j;
					return {UNCOVER, i + 1, j};
				} else if (i < colDimension - 1 && j < rowDimension - 1 && board[ i + 1 ][ j + 1] == -2) {	// Bottom Right
					agentX = i + 1; agentY = j + 1;
					return {UNCOVER, i + 1, j + 1};
				}
			}
		}
	}
	return {LEAVE, -1, -1};
}

Agent::Action MyAI::flagAdjacentOne() {
	for (int i = 1; i < colDimension; ++i) {
		for (int j = 1; j < rowDimension; ++j) {
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
	for (int i = 0; i < colDimension; ++i) {
		for (int j = 0; j < rowDimension; ++j) {
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

					if (i > 0 && j > 0 && adjCells[0] == -2) {
						agentX = i - 1; agentY = j - 1;
						return {UNCOVER, i - 1, j - 1};
					} else if (i > 0 && adjCells[1] == -2) {	// Top Middle 
						agentX = i - 1; agentY = j;
						return {UNCOVER, i - 1, j};
					} else if (i > 0 && j < rowDimension - 1 && adjCells[2] == -2) {	// Top Right 
						agentX = i - 1; agentY = j + 1;
						return {UNCOVER, i - 1, j + 1};
					} else if (j > 0 && adjCells[3] == -2) {	// Middle Left
						agentX = i; agentY = j - 1;
						return {UNCOVER, i, j - 1};
					} else if (j < rowDimension - 1 && adjCells[4] == -2) {
						agentX = i; agentY = j + 1;
						return {UNCOVER, i, j + 1};
					} else if (i < colDimension - 1 && j > 0 && adjCells[5] == -2) {
						agentX = i + 1; agentY = j - 1;
						return {UNCOVER, i + 1, j - 1};
					} else if (i < colDimension - 1 && adjCells[6] == -2) {	// Bottom Middle
						agentX = i + 1; agentY = j;
						return {UNCOVER, i + 1, j};
					} else if (i < colDimension - 1 && j < rowDimension - 1 && adjCells[7] == -2) {	// Bottom Right
						agentX = i + 1; agentY = j + 1;
						return {UNCOVER, i + 1, j + 1};
					}
				}
				else if (board[i][j] == flagcount + coveredcount) {
					if (i > 0 && j > 0 && adjCells[0] == -2) {
						agentX = i - 1; agentY = j - 1;
						totalMines = totalMines - 1;
						return {FLAG, i - 1, j - 1};
					} else if (i > 0 && adjCells[1] == -2) {	// Top Middle 
						totalMines = totalMines - 1;
						agentX = i - 1; agentY = j;
						return {FLAG, i - 1, j};
					} else if (i > 0 && j < rowDimension - 1 && adjCells[2] == -2) {	// Top Right 
						totalMines = totalMines - 1;
						agentX = i - 1; agentY = j + 1;
						return {FLAG, i - 1, j + 1};
					} else if (j > 0 && adjCells[3] == -2) {	// Middle Left
						totalMines = totalMines - 1;
						agentX = i; agentY = j - 1;
						return {FLAG, i, j - 1};
					} else if (j < rowDimension - 1 && adjCells[4] == -2) {
						totalMines = totalMines - 1;
						agentX = i; agentY = j + 1;
						return {FLAG, i, j + 1};
					} else if (i < colDimension - 1 && j > 0 && adjCells[5] == -2) {
						totalMines = totalMines - 1;
						agentX = i + 1; agentY = j - 1;
						return {FLAG, i + 1, j - 1};
					} else if (i < colDimension - 1 && adjCells[6] == -2) {	// Bottom Middle
						totalMines = totalMines - 1;
						agentX = i + 1; agentY = j;
						return {FLAG, i + 1, j};
					} else if (i < colDimension - 1 && j < rowDimension - 1 && adjCells[7] == -2) {	// Bottom Right
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
    for (int i = 2; i < colDimension - 2; ++i) {
        for (int j = 2; j < rowDimension - 2; ++j) {
            // Checking vertically for the pattern
            if (board[i][j-1] == 1 && board[i][j] == 2 && board[i][j+1] == 1 && isUncovered(board[i][j-2]) && isUncovered(board[i][j+2])) {
				if (isUncovered(board[i-2][j-1]) && isUncovered(board[i-1][j-1]) && isUncovered(board[i][j-1]) && 
					isUncovered(board[i+1][j-1]) && isUncovered(board[i+2][j-1])) {
					if (board[i-2][j+1] == -2) {
						totalMines = totalMines - 1;
						agentX = i - 2; agentY = j + 1;
						return {UNCOVER, agentX, agentY};
					}
					else if (board[i-1][j+1] == -2) {
						agentX = i - 1; agentY = j + 1;
						return {FLAG, agentX, agentY};
					}
					else if (board[i][j+1] == -2) {
						totalMines = totalMines - 1;
						agentX = i; agentY = j + 1;
						return {UNCOVER, agentX, agentY};
					}
					else if (board[i+1][j+1] == -2) {
						agentX = i + 1; agentY = j + 1;
						return {FLAG, agentX, agentY};
					}
					else if (board[i+2][j+1] == -2) {
						totalMines = totalMines - 1;
						agentX = i + 2; agentY = j + 1;
						return {UNCOVER, agentX, agentY};
					}
				}
				else if (isUncovered(board[i-2][j+1]) && isUncovered(board[i-1][j+1]) && isUncovered(board[i][j+1]) && 
						 isUncovered(board[i+1][j+1]) && isUncovered(board[i+2][j+1])) {
					if (board[i-2][j-1] == -2) {
						totalMines = totalMines - 1;
						agentX = i - 2; agentY = j - 1;
						return {UNCOVER, agentX, agentY};
					}
					else if (board[i-1][j-1] == -2) {
						agentX = i - 1; agentY = j - 1;
						return {FLAG, agentX, agentY};
					}
					else if (board[i][j-1] == -2) {
						totalMines = totalMines - 1;
						agentX = i; agentY = j - 1;
						return {UNCOVER, agentX, agentY};
					}
					else if (board[i+1][j-1] == -2) {
						agentX = i + 1; agentY = j - 1;
						return {FLAG, agentX, agentY};
					}
					else if (board[i+2][j-1] == -2) {
						totalMines = totalMines - 1;
						agentX = i + 2; agentY = j - 1;
						return {UNCOVER, agentX, agentY};
					}
				}
			}
			// Checking horizontally for the pattern
			else if (board[i-1][j] == 1 && board[i][j] == 2 && board[i+1][j] == 1 && isUncovered(board[i-2][j]) && isUncovered(board[i+2][j])) {
				if (isUncovered(board[i-1][j-2]) && isUncovered(board[i-1][j-1]) && isUncovered(board[i-1][j]) &&
					isUncovered(board[i-1][j+1]) && isUncovered(board[i-1][j+2])) {
					if (board[i+1][j-2] == -2) {
						totalMines = totalMines - 1;
						agentX = i + 1; agentY = j - 2;
						return {UNCOVER, agentX, agentY};
					}
					else if (board[i+1][j-1] == -2) {
						agentX = i + 1; agentY = j - 1;
						return {FLAG, agentX, agentY};
					}
					else if (board[i+1][j] == -2) {
						totalMines = totalMines - 1;
						agentX = i + 1; agentY = j;
						return {UNCOVER, agentX, agentY};
					}
					else if (board[i+1][j+1] == -2) {
						agentX = i + 1; agentY = j + 1;
						return {FLAG, agentX, agentY};
					}
					else if (board[i+1][j+2] == -2) {
						totalMines = totalMines - 1;
						agentX = i + 1; agentY = j + 2;
						return {UNCOVER, agentX, agentY};
					}
				}
				else if (isUncovered(board[i+1][j-2]) && isUncovered(board[i+1][j-1]) && isUncovered(board[i+1][j]) &&
						 isUncovered(board[i+1][j+1]) && isUncovered(board[i+1][j+2])) {
					if (board[i-1][j-2] == -2) {
						totalMines = totalMines - 1;
						agentX = i - 1; agentY = j - 2;
						return {UNCOVER, agentX, agentY};
					}
					else if (board[i-1][j-1] == -2) {
						agentX = i - 1; agentY = j - 1;
						return {FLAG, agentX, agentY};
					}
					else if (board[i-1][j] == -2) {
						totalMines = totalMines - 1;
						agentX = i - 1; agentY = j;
						return {UNCOVER, agentX, agentY};
					}
					else if (board[i-1][j+1] == -2) {
						agentX = i - 1; agentY = j + 1;
						return {FLAG, agentX, agentY};
					}
					else if (board[i-1][j+2] == -2) {
						totalMines = totalMines - 1;
						agentX = i - 1; agentY = j + 2;
						return {UNCOVER, agentX, agentY};
					}
				}
			}
		}
	}
    return {LEAVE, -1, -1};
}

// Handle 1221 pattern
Agent::Action MyAI::handle1221Pattern() {
    for (int i = 1; i < colDimension - 2; ++i) {
        for (int j = 1; j < rowDimension - 2; ++j) {
            if (board[i][j - 1] == 1 && board[i][j] == 2 && board[i][j + 1] == 2 && board[i][j + 2] == 1) {
                // Boundary checks and overlapping pattern checks
                if (i > 1 && j > 1 && i < rowDimension - 3 && j < colDimension - 3 &&
                    (board[i - 1][j - 1] != -2 || board[i - 1][j + 2] != -2) &&
                    (board[i - 1][j + 2] != -2 || board[i + 1][j - 1] != -2) &&
                    (board[i - 2][j] != -2 || board[i + 2][j] != -2)) {
                    if (isUncovered(board[i - 1][j]) && isUncovered(board[i - 1][j + 1])) {
                        if (board[i + 1][j] == -2) {
                            agentX = i + 1;
                            agentY = j;
                            return {FLAG, agentX, agentY};
                        } else if (board[i + 1][j] == -1 && board[i + 1][j + 1] == -2) {
                            agentX = i + 1;
                            agentY = j + 1;
                            return {FLAG, agentX, agentY};
                        }
                    } else if (isUncovered(board[i + 1][j]) && isUncovered(board[i + 1][j + 1])) {
                        if (board[i - 1][j] == -2) {
                            agentX = i - 1;
                            agentY = j;
                            return {FLAG, agentX, agentY};
                        } else if (board[i - 1][j] == -1 && board[i - 1][j + 1] == -2) {
                            agentX = i - 1;
                            agentY = j + 1;
                            return {FLAG, agentX, agentY};
                        }
                    }
                }
            } else if (board[i - 1][j] == 1 && board[i][j] == 2 && board[i + 1][j] == 2 && board[i + 2][j] == 1) {
                // Boundary checks and overlapping pattern checks
                if (i > 1 && j > 1 && i < rowDimension - 3 && j < colDimension - 3 &&
                    (board[i - 2][j] != -2 || board[i + 2][j] != -2) &&
                    (board[i - 1][j - 1] != -2 || board[i + 1][j + 1] != -2)) {
                    if (isUncovered(board[i][j + 1]) && isUncovered(board[i + 1][j + 1])) {
                        if (board[i][j - 1] == -2) {
                            agentX = i;
                            agentY = j - 1;
                            return {FLAG, agentX, agentY};
                        } else if (board[i][j - 1] == -1 && board[i + 1][j - 1] == -2) {
                            agentX = i + 1;
                            agentY = j - 1;
                            return {FLAG, agentX, agentY};
                        }
                    } else if (isUncovered(board[i][j - 1]) && isUncovered(board[i + 1][j - 1])) {
                        if (board[i][j + 1] == -2) {
                            agentX = i;
                            agentY = j + 1;
                            return {FLAG, agentX, agentY};
                        } else if (board[i][j + 1] == -1 && board[i + 1][j + 1] == -2) {
                            agentX = i + 1;
                            agentY = j + 1;
                            return {FLAG, agentX, agentY};
                        }
                    }
                }
            }
        }
    }

    return {LEAVE, -1, -1};
}


//Handle pattern for 11
Agent::Action MyAI::handle11Pattern() {
	for (int i = 0; i < colDimension - 1; ++i) {
		for (int j = 0; j < rowDimension - 1; ++j) {	
			if (board[i][j] == 1 && board[i][j+1] == 1) {
				std::vector<int> adjValues = getAdjacentCells1x2(i, j);

				// For debugging
				//cout << "Vector (1x2): ";
				//for (int k = 0; k < adjValues.size(); ++k) {
				//	cout << adjValues[k] << " ";
				//}
				//cout << endl;

				if ((adjValues[0] == -3 && adjValues[4] == -3 && adjValues[6] == -3) ||
					 (isUncovered(adjValues[0]) && isUncovered(adjValues[4]) && isUncovered(adjValues[6]))) {
					if (isUncovered(adjValues[1]) && isUncovered(adjValues[2]) && isUncovered(adjValues[3]) && isUncovered(adjValues[5])) {
						if (adjValues[7] == -2 && adjValues[8] == -2 && adjValues[9] == -2) {
							agentX = i + 1; agentY = j + 2;
							return {UNCOVER, agentX, agentY};
						}
					}
					if (isUncovered(adjValues[7]) && isUncovered(adjValues[8]) && isUncovered(adjValues[9]) && isUncovered(adjValues[5])) {
						if (adjValues[1] == -2 && adjValues[2] == -2 && board[i-1][j+2] == -2) {
							agentX = i - 1; agentY = j + 2;
							return {UNCOVER, agentX, agentY};
						}
					}
				}
				else if ((adjValues[3] == -3 && adjValues[5] == -3 && adjValues[9] == -3) ||
						(isUncovered(adjValues[3]) && isUncovered(adjValues[5]) && isUncovered(adjValues[9]))) {
					if (isUncovered(adjValues[0]) && isUncovered(adjValues[1]) && isUncovered(adjValues[2]) && isUncovered(adjValues[4])) {
						if (adjValues[6] == -2 && adjValues[7] == -2 && adjValues[8] == -2) {
							agentX = i + 1; agentY = j - 1;
							return {UNCOVER, agentX, agentY};
						}
					}	
					if (isUncovered(adjValues[4]) && isUncovered(adjValues[6]) && isUncovered(adjValues[7]) && isUncovered(adjValues[8])) {
						if (adjValues[0] == -2 && adjValues[1] == -2 && adjValues[2] == -2) {
							agentX = i - 1; agentY = j - 1;
							return {UNCOVER, agentX, agentY};
						}
					}
				}
			}
			else if (board[i][j] == 1 &&  board[i+1][j] == 1) {
				
				//debug line
				//cout << "this is before getAdjacentCells2x1" << endl;

				std::vector<int> adjValues = getAdjacentCells2x1(i, j);

				// For debugging
				//cout << "Vector (2x1): ";
				//for (int k = 0; k < adjValues.size(); ++k) {
				//	cout << adjValues[k] << " ";
				//}
				//cout << endl;

				if ((adjValues[0] == -3 && adjValues[1] == -3 && adjValues[2] == -3) ||
					 (isUncovered(adjValues[0]) && isUncovered(adjValues[1]) && isUncovered(adjValues[2]))) {
					if (isUncovered(adjValues[4]) && isUncovered(adjValues[6]) && isUncovered(adjValues[8]) && isUncovered(adjValues[9])) {
						if (adjValues[3] == -2 && adjValues[5] == -2 && adjValues[7] == -2) {
							agentX = i + 2; agentY = j - 1;
							return {UNCOVER, agentX, agentY};
						}
					}
					if (isUncovered(adjValues[3]) && isUncovered(adjValues[5]) && isUncovered(adjValues[7]) && isUncovered(adjValues[8])) {
						if (adjValues[4] == -2 && adjValues[6] == -2 && adjValues[9] == -2) {
							agentX = i + 2; agentY = j + 1;
							return {UNCOVER, agentX, agentY};
						}
					}
				}
				else if ((adjValues[7] == -3 && adjValues[8] == -3 && adjValues[9] == -3) ||
						(isUncovered(adjValues[7]) && isUncovered(adjValues[8]) && isUncovered(adjValues[9]))) {
					if (isUncovered(adjValues[1]) && isUncovered(adjValues[2]) && isUncovered(adjValues[4]) && isUncovered(adjValues[6])) {
						if (adjValues[0] == -2 && adjValues[3] == -2 && adjValues[5] == -2) {
							agentX = i - 1; agentY = j - 1;
							return {UNCOVER, agentX, agentY};
						}
					}
					if (isUncovered(adjValues[0]) && isUncovered(adjValues[1]) && isUncovered(adjValues[3]) && isUncovered(adjValues[5])) {
						if (adjValues[2] == -2 && adjValues[4] == -2 && adjValues[6] == -2) {
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
	for (int i = 1; i < colDimension - 1; ++i) {
		for (int j = 1; j < rowDimension - 1; ++j) {
			
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
    for (int i = 1; i < colDimension - 1; ++i) {
        for (int j = 1; j < rowDimension - 1; ++j) {
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
    for (int i = 1; i < colDimension - 2; ++i) {
        for (int j = 1; j < rowDimension - 2; ++j) {
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

	for (int l = -1; l <= 1; ++l) {
		for (int m = -1; m <= 1; ++m) {
			if (l == 0 && m == 0) continue;
			int newX = x + l;
			int newY = y + m;

			if (newX >= 0 && newX < colDimension && newY >= 0 && newY < rowDimension) {
				adjacentcells.push_back(board[newX][newY]);
			} else {
				adjacentcells.push_back(out_of_bound_value);
			}
		}
	}

	return adjacentcells;
}

std::vector<int> MyAI::getAdjacentCells1x2(int x, int y) {
	std::vector<int> adjacentcells;
	const int out_of_bound_value = -3;

	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 2; ++j) {
			if (i == 0 && j == 0) continue;
			if (i == 0 && j == 1) continue;
			int newX = x + i;
			int newY = y + j;

			if (newX >= 0 && newX < colDimension && newY >= 0 && newY < rowDimension) {
				adjacentcells.push_back(board[newX][newY]);
			} else {
				adjacentcells.push_back(out_of_bound_value);
			}
		}
	}

	return adjacentcells;
}

std::vector<int> MyAI::getAdjacentCells2x1(int x, int y) {
	std::vector<int> adjacentcells;
	const int out_of_bound_value = -3;

	for (int i = -1; i <= 2; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (i == 0 && j == 0) continue;
			if (i == 1 && j == 0) continue;
			int newX = x + i;
			int newY = y + j;

			if (newX >= 0 && newX < colDimension && newY >= 0 && newY < rowDimension) {
				adjacentcells.push_back(board[newX][newY]);
			} else {
				adjacentcells.push_back(out_of_bound_value);
			}
		}
	}

	return adjacentcells;
}

Agent::Action MyAI::uncoverRemainingCells() {
	for (int i = 0; i < colDimension; ++i) {
		for (int j = 0; j < rowDimension; ++j) {
			if (board[ i ][ j ] == -2) {
				agentX = i; agentY = j;
				return {UNCOVER, i, j};
			}
		}
	}
	return {LEAVE, -1, -1};
}
	bool MyAI::isValidCell(int x, int y){
    return (x >= 0 && x < rowDimension && y >= 0 && y < colDimension);
	}


Agent::Action MyAI::educatedGuess() {
    double minRisk = 1.0;
    int guessX = -1, guessY = -1;

    // Find cell with the minimum mine probability
    for (int i = 0; i < colDimension; ++i) {
        for (int j = 0; j < rowDimension; ++j) {
            if (board[i][j] == -2) {  // Only consider covered cells
                double risk = calculateMineProbability(i, j);
                if (risk < minRisk) {
                    minRisk = risk;
                    guessX = i;
                    guessY = j;
                }
            }
        }
    }

    // If a valid guess is found
    if (guessX != -1 && guessY != -1) {
        agentX = guessX;
        agentY = guessY;
        return {UNCOVER, guessX, guessY};
    }

	for (int i = 0; i < colDimension; ++i) {
		for (int j = 0; j < rowDimension; ++j) {
			if (board[i][j] == -2) {
				totalMines = totalMines - 1;
				agentX = i; agentY = j;
				return {UNCOVER, i, j};
			}
		}
	}

    // If no guess is found, leave the game
    return {LEAVE, -1, -1};
}

double MyAI::calculateMineProbability(int x, int y) {
    int flaggedCount = 0;
    int coveredCount = 0;
    int uncoveredCount = 0;
    int adjacentCells[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

    // Track sum of neighboring probabilities
    double neighboringRisk = 0.0;

    for (int k = 0; k < 8; ++k) {
        int adjX = x + adjacentCells[k][0];
        int adjY = y + adjacentCells[k][1];

        if (isValidCell(adjX, adjY)) {
            if (board[adjX][adjY] == -1) {
                ++flaggedCount;
            } else if (board[adjX][adjY] == -2) {
                ++coveredCount;
            } else {
                ++uncoveredCount;
                neighboringRisk += calculateLocalRisk(adjX, adjY);
            }
        }
    }

    // Base probability (global view)
    double mineProbability = (totalMines - flaggedCount) / static_cast<double>(coveredCount);

    // Adjust probability using local risk
    if (uncoveredCount > 0) {
        mineProbability = min(mineProbability, neighboringRisk / uncoveredCount);
    }

    return mineProbability;
}

double MyAI::calculateLocalRisk(int adjX, int adjY) {
    int numAdjacentMines = board[adjX][adjY];
    int numAdjacentFlags = 0;
    int numAdjacentCovered = 0;
    int adjacentCells[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

    for (int l = 0; l < 8; ++l) {
        int neighborX = adjX + adjacentCells[l][0];
        int neighborY = adjY + adjacentCells[l][1];

        if (isValidCell(neighborX, neighborY)) {
            if (board[neighborX][neighborY] == -1) {
                ++numAdjacentFlags;
            } else if (board[neighborX][neighborY] == -2) {
                ++numAdjacentCovered;
            }
        }
    }

    if (numAdjacentCovered > 0) {
        return static_cast<double>(numAdjacentMines - numAdjacentFlags) / numAdjacentCovered;
    }
    return 1.0; // Default to max risk if no covered cells are adjacent
}

// Returns whether the cell is uncovered or not 
bool MyAI::isUncovered(int number) {
	if (number == -1 || number == -2 || number == -3) {
		return false;
	}
	return true;
}

Agent::Action MyAI::CSPMove() {
    set<pair<int, int>> safeMoves;
    set<pair<int, int>> potentialMines;

    // find safe moves
    for (int i = 0; i < colDimension; ++i) {
        for (int j = 0; j < rowDimension; ++j) {
            if (board[i][j] >= 0) { 
                int minesAround = 0;
                int unknownCellsAround = 0;
                vector<pair<int, int>> unknownCells;

                // Check adjacent cells
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        int nx = i + dx;
                        int ny = j + dy;

                        if (nx >= 0 && nx < rowDimension && ny >= 0 && ny < colDimension) {
                            if (board[nx][ny] == -1) {
                                minesAround++;
                            } else if (board[nx][ny] == -2) {
                                unknownCellsAround++;
                                unknownCells.push_back({nx, ny});
                            }
                        }
                    }
                }

                // If # of mines around equals the clue, we know that all other unknown cells are safe
                if (minesAround == board[i][j]) {
                    safeMoves.insert(unknownCells.begin(), unknownCells.end());
                }

                // However if the # of unknown cells around equals the remaining mines, they are probably mines
                if (unknownCellsAround == board[i][j] - minesAround) {
                    potentialMines.insert(unknownCells.begin(), unknownCells.end());
                }
            }
        }
    }

    // Make a safe move(if possible)
    if (!safeMoves.empty()) {
        auto it = safeMoves.begin();
        agentX = it->first;
        agentY = it->second;
        return {UNCOVER, agentX, agentY};
    }

    // If no safe move is found, perform a random guess
    return educatedGuess();
}

void MyAI::printBoard() {
	std::cout << "Board within the function" << std::endl;
	for (int i = 0; i < colDimension; ++i) {
		for (int j = 0; j < rowDimension; ++j) {
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