#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>

#include "player_smart.h"

using namespace std;

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
int opponent;

const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

void read_board(std::ifstream& fin) {
    fin >> player;
    opponent = (player == BLACK) ? WHITE: BLACK;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

bool inside(int i, int j) {
	return i >= 0 && j >= 0 && i < SIZE && j < SIZE;
}

// return number of contiguous discs of the input color
// blocked indicates whether blocked at the end
int count(int i, int j, int color, bool &blocked, int dir[2]) {
	blocked = true;

	i += dir[0];
	j += dir[1];

	int cnt = 0;
	while (inside(i, j) && board[i][j] == color) {
		cnt++;
		i += dir[0];
		j += dir[1];
	}

	if (inside(i, j) && board[i][j] == EMPTY)
		blocked = false;

	return cnt;
}


int eval(int i, int j, int color) {
	int score = 0;

	int len1, len2, len;
	bool blocked1, blocked2;

	// score for enlongating the existing discs
	for (int k = 0; k < 4; k++) {
		len1 = count(i, j, color, blocked1, dir[2*k]);
		len2 = count(i, j, color, blocked2, dir[2*k+1]);

		len = len1 + len2 + 1;

		if (len >= 5) {
			score = WIN5;
			return score;
		}

		if (blocked1 && blocked2) continue;

		bool open = (!blocked1) && (!blocked2);
		if (len == 4) {
			score += (open ? OPEN4: HALF4);
		} else if (len == 3) {
			score += (open ? OPEN3: HALF3);
		} else if (len == 2) {
			score += (open ? OPEN2: HALF2);
		} else {
			score += open;
		}
	}

	return score;
}

int max_search(int alpha, int beta, int depth, int score) {
	if (depth == 0) {
		return score;
	}

	int v = NEG_INF;
	int vv;

	int delta;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (board[i][j] == EMPTY) {
				delta = eval(i, j, player);
				if (delta == WIN5)
					return WIN5;

				board[i][j] = player;

				vv = min_search(alpha, beta, depth-1, score + delta);
				v = max(v, vv);
				if (v >= beta) {
					board[i][j] = EMPTY;
					return v;
				}
				alpha = max(alpha, vv);
				
				board[i][j] = EMPTY;
			}
		}
	}

	return v;
}

int min_search(int alpha, int beta, int depth, int score) {
	if (depth == 0) {
		return score;
	}

	int v = INF;
	int vv;
	int delta;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (board[i][j] == EMPTY) {
				delta = eval(i, j, opponent);
				if (delta == WIN5)
					return -WIN5;

				board[i][j] = opponent;
				vv = max_search(alpha, beta, depth-1, score - delta);
				v = min(v, vv);
				if (v <= alpha) {
					board[i][j] = EMPTY;
					return v;
				}
				beta = min(beta, v);

				board[i][j] = EMPTY;
			}
		}
	}

	return v;
}

void write_valid_spot(std::ofstream& fout) {
	int v = NEG_INF;
	int vv;
	int delta;

	fout << 0 << " " << 0 << endl;

	// iterative deepening
	for (int depth = 1; depth <= 3; depth += 2) {
		cout << "now depth: " << depth << endl;

		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				if (board[i][j] == EMPTY && ((delta = eval(i, j, player)) > 0)) {
					board[i][j] = player;

					vv = min_search(v, INF, depth, delta);
					if (vv > v) {
						fout << i << " " << j << endl;
						v = vv;

						// this means must-do
						// thus no need to search further
						if (vv >= WIN5)
							return;
					}

					board[i][j] = EMPTY;
				}
			}
		}
	}
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);  // read the player color and board
    std::ofstream fout(argv[2]); // output (x, y) of next move
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
