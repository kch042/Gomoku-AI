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

// evaluate how good is placing disc at (i, j) for the placer
int eval_move(int i, int j, int color) {
	int score = 0;

	bool blocked1 = true, blocked2 = true;
	int len1, len2, len;
	for (int k = 0; k < 4; k++) {
		len1 = count(i, j, color, blocked1, dir[2*k]);
		len2 = count(i, j, color, blocked2, dir[2*k+1]);

		len = len1 + len2 + 1;

		if (len >= 5) {
			score = RANK1;
			break;  // win, no need to do other.
		}

		// both end blocked, no need to consider no matter how long.
		if (blocked1 && blocked2) continue;

		bool bothOpen = (!blocked1) && (!blocked2);
		if (len == 4) {
			if (bothOpen) score += RANK2;
			else score += RANK3;
		} else if (len == 3) {
			if (bothOpen) score += RANK3;
			else score += RANK4;
		} else if (len == 2) {
			score += RANK5;
		} else if (len == 1) {
			score += RANK6;
		}
	}

	return score;
}


int eval(int i, int j) {
	return eval_move(i, j, player) + eval_move(i, j, opponent);
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
				board[i][j] = player;
				delta = eval(i, j);

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
				board[i][j] = opponent;
				delta = eval(i, j);
				
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
	int beta = INF;
	int v = NEG_INF;
	int vv;

	int depth = 3;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (board[i][j] == EMPTY) {
				board[i][j] = player;
				
				vv = min_search(v, beta, depth, eval(i, j));
				if (vv > v) {
					fout << i << " " << j << endl;
					v = vv;
				}

				board[i][j] = EMPTY;
			}
		}
	}


	//pair<int, int> pos = search();
	//fout << pos.first << " " << pos.second << endl;
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
