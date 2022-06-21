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

int eval() {
	int boo = 10;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (i+j == 5)
				boo++;
		}
	}

    return 0;
}

int max_search(int alpha, int beta, int depth) {
	if (depth == 0) {
		return eval();
	}

	int v = NEG_INF;
	int vv;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (board[i][j] == EMPTY) {
				board[i][j] = player;

				vv = min_search(alpha, beta, depth-1);
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

int min_search(int alpha, int beta, int depth) {
	if (depth == 0) {
		return eval();
	}

	int v = INF;
	int vv;
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (board[i][j] == EMPTY) {
				board[i][j] = opponent;
				
				vv = max_search(alpha, beta, depth-1);
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

pair<int, int> search() {
    int beta = INF;
    int v = NEG_INF;
	int depth = 4;

    pair<int, int> pos = {0, 0};

    int vv;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == EMPTY) {
				board[i][j] = player;

				vv = min_search(v, beta, depth);
				if (vv >= v) {
					v = vv;
					pos = {i, j};
				}

				board[i][j] = EMPTY;
            }
        }
    }

    return pos;
}

void write_valid_spot(std::ofstream& fout) {
	//fout << 0 << " " << 0 << endl;

	pair<int, int> pos = search();
	fout << pos.first << " " << pos.second << endl;
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
