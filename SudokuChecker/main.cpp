/*SUDOKU CHECKER V.1
  Shannon Lowry
  March 25, 2019
*/

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct squarePair {
	int x;
	int y;
};

struct cell {
	vector<int> possibleNumbers;
	bool preFilled;
	int num;
	squarePair square;
};

// This will read in the csv fild into a 9X9 2D vector
vector<vector<cell>> readInData(string fileName) {
	vector<vector<cell>> retVector(9, vector<cell>(9));
	ifstream infile(fileName);
	if (!infile.is_open()) {
		cout << "Error opening file" << endl;
	}
	string _row;
	// Making sure only 9 values per row read in
	for (int row = 0; row < 9; row++) {
		getline(infile, _row);
		stringstream ss(_row);
		string cellNum;
		for (int col = 0; col < 9; col++) {
			getline(ss, cellNum, ',');
			// Checking cell by cell to see if there is a number
			try {
				retVector[row][col].num = stoi(cellNum);
				retVector[row][col].preFilled = true;
				retVector[row][col].square.x = (row / 3) * 3; // Labeling each square with their top left cells coordinates
				retVector[row][col].square.y = (col / 3) * 3;
			}
			// There was no number in the cell
			catch (invalid_argument e) {
				retVector[row][col].num = 0;
				retVector[row][col].preFilled = false;
				retVector[row][col].square.x = (row / 3) * 3;
				retVector[row][col].square.y = (col / 3) * 3;
			}
		}
	}
	return retVector;
}

// This function will check each cell against it's entire row, column, and square
bool isValid(cell _cell, vector<vector<cell>>& cells, int _row, int _col) {
	// Starting by checking if the cell in questin contains a number 1-9
	if (cells[_row][_col].num < 0 || cells[_row][_col].num > 9) {
		cout << "Cell at [" << _row << "][" << _col << "] "<<  cells[_row][_col].num << " is not a number 1-9" << endl;
		return false;
	}
	// Start at cell in question and check down the column for a match
	for (int row = 0; row < 9; row++) {
		if (row == _row) {
			continue;
		}
		// If a match is found in the column, the board is invalid
		if (cells[row][_col].num == _cell.num) {
			cout << "Cell at [" << row << "][" << _col << "] " << cells[_row][_col].num << " matches " << cells[_row][_col].num;
			cout << " at [" << _row << "][" << _col << "] " << endl;
			return false;
		}
	}
	// Start at cell in question and check across the row for a match
	for (int col = 0; col < 9; col++) {
		if (col == _col) {
			continue;
		}
		// If a match is found in the row, the board is invalid
		if (cells[_row][col].num == _cell.num) {
			cout << "Cell at [" << _row << "][" << col << "] " << cells[_row][_col].num << " matches " << cells[_row][_col].num;
			cout << " at [" << _row << "][" << _col << "] " << endl;
			return false;
		}
	}
	// Start at cell in question and check its square for a match
	// Each square is bound by three spaces across or down
	for (int row = _cell.square.x; row < _cell.square.x + 3; row++) {
		for (int col = _cell.square.y; col < _cell.square.y + 3; col++) {
			if ((row == _row && col == _col)) {
				continue;
			}
			// If a match is found in the square, the board is invalid
			if (cells[row][col].num == _cell.num) {
				cout << "Cell at [" << row << "][" << col << "] " << cells[_row][_col].num << " matches " << cells[_row][_col].num;
				cout << " at [" << _row << "][" << _col << "] " << endl;
				return false;
			}
		}
	}
	// One cell has been check and is valid, return true and move onto the next
	return true;
}

//Checks of the vector contains a value. If it does, return the index position
int contains(vector<int> vec, int val) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == val) {
			return i;
		}
	}
	return -1;
}
// This function will check each row, column, and square for pre-filled cells
vector<int> findPossibilites(cell _cell, vector<vector<cell>>& cells, int _row, int _col) {
	vector<int> retVector{ 1,2,3,4,5,6,7,8,9 };
	// Starting at base cell, traverse down each row looking for a pre-filled cell
	for (int row = 0; row < 9; row++) {
		// If a pre-filled cell is found, remove it from the retVector because it's not possible
		if (cells[row][_col].preFilled) {
			int foundIndex = contains(retVector, cells[row][_col].num);
			if (foundIndex != -1) {
				vector<int>::iterator itr = retVector.begin() + foundIndex;
				retVector.erase(itr);
			}
		}
	}
	// Starting at base cell, traverse across each column looking for a pre-filled cell
	for (int col = 0; col < 9; col++) {
		// If a pre-filled cell is found, remove it from the retVector because it's not possible
		if (cells[_row][col].preFilled) {
			int foundIndex = contains(retVector, cells[_row][col].num);
			if (foundIndex != -1) {
				vector<int>::iterator itr = retVector.begin() + foundIndex;
				retVector.erase(itr);
			}
		}
	}
	// Starting at base cell, check each cell in 3X3 square for pre-filled cell
	for (int row = _cell.square.x; row < _cell.square.x + 3; row++) {
		for (int col = _cell.square.y; col < _cell.square.y + 3; col++) {
			// If a pre-filled cell is found, remove it from the retVector because it's not possible
			if (cells[row][col].preFilled) {
				int foundIndex = contains(retVector, cells[row][col].num);
				if (foundIndex != -1) {
					vector<int>::iterator itr = retVector.begin() + foundIndex;
					retVector.erase(itr);
				}
			}
		}
	}

	return retVector;
}
// This will write the completed board back to a CSV file
void writeToCSV(vector<vector<cell>> cells, string fileName) {
	ofstream outfile(fileName);
	if (!outfile.is_open()) {
		cout << "IT BROKE" << endl;
	}
	// The pre-filled cells will have an asterisk surround the number. i.e *8* was already in the cell
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			if (cells[row][col].preFilled) {
				outfile << "*" << cells[row][col].num << "*" << ",";
			}
			else {
				// If the cell wasn't pre-filled, each cell will contain the possible numbers retVector
				for (int i = 0; i < cells[row][col].possibleNumbers.size(); i++) {
					outfile << cells[row][col].possibleNumbers[i] << " ";
				}
				outfile << ",";
			}
		}
		outfile << "\n";
	}
}

int main() {

	cout << "Enter a sample csv file:	sample1.csv		sample2.csv		sample3.csv" << endl;
	string fileName;
	cin >> fileName;

	string writeFile = "results.csv";
	vector<vector<cell>> cells = readInData(fileName);
	// Checking to see if each pre-filled cell is valid
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			if (cells[row][col].preFilled) {
				if (!isValid(cells[row][col], cells, row, col)) {
					cout << "It's not a valid board! Same number found in row, column, or square!" << endl;
					return 1;
				}
			}
		}
	}
	// If the cell is not pre-filled, find the possible numbers
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			if (!cells[row][col].preFilled) {
				cells[row][col].possibleNumbers = findPossibilites(cells[row][col], cells, row, col);
			}
		}
	}
	cout << "It's a valid board! Check the excel spreadsheet for possible numbers in each cell!";
	writeToCSV(cells, writeFile);

	return 0;

}