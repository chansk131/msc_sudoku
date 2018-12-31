#include "sudoku.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* pre-supplied function to load a Sudoku board from a file */
void load_board(const char* filename, char board[9][9]) {
  cout << "Loading Sudoku board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in) cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer, 512);
  while (in && row < 9) {
    for (int n = 0; n < 9; n++) {
      assert(buffer[n] == '.' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer, 512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_frame(int row) {
  if (!(row % 3))
    cout << "  +===========+===========+===========+" << endl;
  else
    cout << "  +---+---+---+---+---+---+---+---+---+" << endl;
}

/* internal helper function */
void print_row(const char* data, int row) {
  cout << (char)('A' + row) << " ";
  for (int i = 0; i < 9; i++) {
    cout << ((i % 3) ? ':' : '|') << " ";
    cout << ((data[i] == '.') ? ' ' : data[i]) << " ";
  }
  cout << "|" << endl;
}

/* pre-supplied function to display a Sudoku board */
void display_board(const char board[9][9]) {
  cout << "    ";
  for (int r = 0; r < 9; r++) cout << (char)('1' + r) << "   ";
  cout << endl;
  for (int r = 0; r < 9; r++) {
    print_frame(r);
    print_row(board[r], r);
  }
  print_frame(9);
}

/* add your functions here */
bool is_complete(const char board[9][9]) {
  for (int r = 0; r < 9; r++) {
    for (int c = 0; c < 9; c++) {
      if (board[r][c] == '.') return false;
    }
  }
  return true;
}

bool get_row_and_column(const char * position, int& row, int& column) {
  if (position[2]) {
    return false;
  }
  if (position[0]) {
    row = position[0] - 'A';
    if (row < 0 || row > 8) {
      return false;
    }
  }
  if (position[1]) {
    column = position[1] - '0';
    if (column < 0 || column > 8) {
      return false;
    }
  }

  return true;
}

bool is_duplicated_block(const char board[9][9], int row, int col, const char digit) {
  int block_row = row / 3;
  int block_col = col / 3;
  for (int r = 0; r < 3; r++) {
    for (int c = 0; c < 3; c++) {
      if (board[block_row*3 + r][block_col*3 + c] == digit) return true;
    }
  }
  return false;
}

bool is_duplicated_row(const char board[9][9], int row, const char digit) {
  for (int col = 0; col < 9; col++) {
    if (board[row][col] == digit) return true;
  }
  return false;
}

bool is_duplicated_column(const char board[9][9], int col, const char digit) {
  for (int row = 0; row < 9; row++) {
    if (board[row][col] == digit) return true;
  }
  return false;
}

bool is_valid_move(char board[9][9], int row, int column, const char digit) {
  int number = digit - '0';
  if (number < 0 || number > 9) {
    cerr << "Number given is not between 0 and 9" << endl;
    return false;
  }

  if (board[row][column] != '.') {
    // cerr << "This cell is not free" << endl;
    return false;
  }

  // check block
  if (is_duplicated_block(board, row, column, digit)) {
    // cerr << "Duplicate in the same block" << row << column << digit << endl;
    return false;
  }

  // check row
  if (is_duplicated_row(board, row, digit)) {
    // cerr << "Duplicate in the same row" << row << column << digit << endl;
    return false;
  }

  // check column
  if (is_duplicated_column(board, column, digit)) {
    // cerr << "Duplicate in the same column"  << row << column << digit << endl;
    return false;
  }

  return true;
}

bool make_move(const char position[2], const char digit, char board[9][9]) {
  int row, column, number;
  if (!get_row_and_column(position, row, column)) {
    cerr << "position given is not on the board" << endl;
    return false;
  }

  number = digit - '0';
  if (number < 0 || number > 9) {
    cerr << "Number given is not between 0 and 9" << endl;
    return false;
  }

  if (board[row][column] != '.') {
    cerr << "This cell is not free" << endl;
    return false;
  }

  board[row][column] = digit;
  return true;
}

bool save_board(const char* filename, char board[9][9]) {
  ofstream out;
  out.open(filename);
  if (!out) {
    cout << "Failed!" << endl;
    return false;
  }

  for (int r = 0; r < 9; r++) {
    for (int c = 0; c < 9; c++) {
      out << board[r][c];
      if (out.fail()) return false;
    }
    out << endl;
  }
  out.close();
  
  return true;
}

bool solve_board(char board[9][9]) {
  if (is_complete(board)) return true;

  for (int row = 0; row < 9; row++) {
    for (int column = 0; column < 9; column++) {
      if (board[row][column] == '.') {
        for (char digit = '1'; digit <= '9'; digit++) {
          if (is_valid_move(board, row, column, digit)) {
            board[row][column] = digit;
            // display_board(board);
            if (solve_board(board)) {
              return true;
            } else {
              board[row][column] = '.';
            }
          }
        }
        return false;
      }
    }
  }
  return false;
}