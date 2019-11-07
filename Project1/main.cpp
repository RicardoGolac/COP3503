#include <iostream>
#include <sstream>
#include <list>
using namespace std;

struct node {
	int value;
	int row;
	int col;
};

int search(list<node> matrix, int sp_row, int sp_col) {
	while (matrix.size() != 0) {
		node temp = matrix.front();
		// row and col match, return element
		if ((temp.row == sp_row) && (temp.col == sp_col)) {
			return temp.value;
		}
		matrix.pop_front();
	}
	return 0;
}

// Finds number of nonzero terms in first row
int totalColumns(list<node> matrix) {
	int totalCol = 0;
	list<node> tempMatrix = matrix;
	int currentRow = 0;
	// Add up total non filled rows in 
	while (currentRow == 0) {
		node temp = tempMatrix.front();
		if (temp.row > 0)
			break;
		tempMatrix.pop_front();
		totalCol++;
		currentRow += temp.row;
	}
	return totalCol;
}

// returns minor of input matrix, taking out specified row and column
list<node> minorFunc(list<node> sparsematrix, int row_rm, int col_rm) {
	// Must return a list with now row and col numbers, starting from 0, 0;
	list<node> tempMatrix;
	int avoid_col = col_rm;
	// row will always be one less than temp value
	node temp = sparsematrix.front();
	int rowCount = temp.row;
	while (sparsematrix.size() != 0) {
		temp = sparsematrix.front();
		// Reset row if needed
		if (rowCount != temp.row) {
			rowCount = temp.row;
		}
		// Ignore elements that are in the target row or column
		if ((temp.row != row_rm) && (temp.col != col_rm)) {
			temp.row = (temp.row - 1); // new row will always be one less
			if (temp.col > avoid_col)
				temp.col = temp.col - 1;
			tempMatrix.push_back(temp);
		}
		sparsematrix.pop_front();
	}
	return tempMatrix;
}

int determinant(list<node> sparsematrix) {
	int det = 0;
	// Base Case: deal with a 1x1 matrix
	if (sparsematrix.size() == 1) {
		node oneMatrix = sparsematrix.front();
		return oneMatrix.value;
	}
	// Base Case: deal with a 2x2 matrix
	if (sparsematrix.size() <= 4) {
		int zero = search(sparsematrix, 0, 0);
		int one = search(sparsematrix, 0, 1);
		int two = search(sparsematrix, 1, 0);
		int three = search(sparsematrix, 1, 1);
		det = (zero * three) - (one * two);
		return det;
	}
	// Recursive Case: If sparse matrix is bigger than 2x2 Matrix
	// Discover # of filled columns in first row
	int tot_col = totalColumns(sparsematrix);
	// Do FOR LOOP adding determinants of minors multipied by column elements
	for (int i = 0; i < tot_col; i++) {
		node temp = sparsematrix.front();
		int coeff = 1;
		if (((temp.col + 1) % 2) == 0) // if column is even, make negative
			coeff = -1;
		det += (coeff * temp.value * determinant(minorFunc(sparsematrix, temp.row, temp.col)));
		sparsematrix.pop_front();
	}
	return det;
}

/***** MAIN *****/
int main() {
	//std::cout << "Enter in a square matrix to recieve its determinant" << endl;
	// Data containers
	list<node> SparseMatrix;
	string input;
	int row_num = 0;
	int col_num = 0;
	int firstColumnCount = 0;
	int totalColumn = 0; // Keepst Tack of # number of elements in array

	// Read in matrix input into SparseMatrix
	while (cin.good()) {
		getline(cin, input);
		// Parse through line
		stringstream linestream(input);
		int number;
		int col_num = 0;
		// Used to test if matrix is a square
		int totalRow = 0;
		while (linestream >> number) {
			node temp;
			temp.value = number;
			temp.row = row_num;
			temp.col = col_num;
			if (temp.value != 0)
				SparseMatrix.push_back(temp);
			if (row_num == 0)
				firstColumnCount++;
			totalColumn++;
			col_num++;
		}
		row_num++;
	}

	// Check if matrix is a square, if not throw error message;
	if (totalColumn != (row_num * row_num) || totalColumn != (firstColumnCount * firstColumnCount)) {
		cout << "Error! Non-square matrix!" << endl;
		return 0;
	}

	// Take Determinant of Sparse Matrix
	int result = determinant(SparseMatrix);
	cout << result << endl;
	return 0;
}