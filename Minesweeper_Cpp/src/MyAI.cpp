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

// Check if there are any 0’s on the board where the adjacent values aren’t uncovered yet
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

		// Check if there is a cell where the value is 1 and only one adjacent cell that isn’t uncovered
		for (int i = 0; i < rowDimension; ++i) {
			for (int j = 0; j < colDimension; ++j) {
				if (board[ i ][ j ] == 1) {
					int coveredNeighbors = 0;
                	int flagX = -1, flagY = -1;

                	if (i > 0 && j > 0 && board[i - 1][j - 1] == -2) {
						coveredNeighbors++; flagX = i - 1; flagY = j - 1; 
					}
                	if (i > 0 && board[i - 1][j] == -2) {
						coveredNeighbors++; flagX = i - 1; flagY = j;
					}
               		if (i > 0 && j < colDimension - 1 && board[i - 1][j + 1] == -2) {
						coveredNeighbors++; flagX = i - 1; flagY = j + 1; 
					}
                	if (j > 0 && board[i][j - 1] == -2) {
						coveredNeighbors++; flagX = i; flagY = j - 1; 
					}
                	if (j < colDimension - 1 && board[i][j + 1] == -2) { 
						coveredNeighbors++; flagX = i; flagY = j + 1; 
					}
                	if (i < rowDimension - 1 && j > 0 && board[i + 1][j - 1] == -2) { 
						coveredNeighbors++; flagX = i + 1; flagY = j - 1; 
					}
                	if (i < rowDimension - 1 && board[i + 1][j] == -2) {
						coveredNeighbors++; flagX = i + 1; flagY = j; 
					}
                	if (i < rowDimension - 1 && j < colDimension - 1 && board[i + 1][j + 1] == -2) {
						coveredNeighbors++; flagX = i + 1; flagY = j + 1; 
					}
                	if (coveredNeighbors == 1) {
                   		agentX = flagX; agentY = flagY;
                    	return {FLAG, flagX, flagY};
                	}
				}
			}
		}

		// Leave otherwise
		return {LEAVE,-1,-1};
}
    // ======================================================================
    // YOUR CODE ENDS
    // ======================================================================




// ======================================================================
// YOUR CODE BEGINS
// ======================================================================



// ======================================================================
// YOUR CODE ENDS
// ======================================================================
