void load_board(const char* filename, char board[9][9]);
void display_board(const char board[9][9]);
bool is_complete(const char board[9][9]);
bool make_move(const char position[2], const char digit, char board[9][9]);
bool save_board(const char* filename, char board[9][9]);
bool solve_board(char board[9][9]);

bool is_duplicated_block(const char board[9][9], int row, int col, const char digit);
bool is_duplicated_row(const char board[9][9], int row, const char digit);
bool is_duplicated_column(const char board[9][9], int col, const char digit);
bool is_valid_move(char board[9][9], int row, int column, const char digit);