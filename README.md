# Sudoku-Checker
This Sudoku checker will read in a csv file of a 9X9 Sudoku board and check to see if it's a valid board.
The valid board will consist of no duplicate numbers in a row, column, or the 3X3 square in which the number resides.
The valid board will only have numbers 1-9 in any given cell.

If the board is valid, the program will output a list of possible numbers for each empty cell to a csv file.
      Valid boards will have prefilled spots indicated with *#* formatting.

If the board is invalid, the program will output the sudoku board to a csv file that displays the prefilled spots.
      Invalid boards will be marked with !#!, indicating the cell is invalid.
      A more detailed response of why the cell is invalid can be found on the console screen.
      
** Current instruction for user **
  - User shall enter in desired 'sample#.csv' file listed on the console.
  - The results will then be published to an excel spreadsheet.
  
  Note: The current program is set to overwrite the csv results file with the sample.csv file.
  
  


