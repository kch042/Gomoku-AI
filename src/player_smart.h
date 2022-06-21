#include <limits>

// Heuristic of the value setting is that
// for example, 1 contigous four is approximately as good as 2 contigous three.

#define RANK1 (int)1e7  // win, don't set INF in case overflow.
#define RANK2 1<<5
#define RANK3 1<<4
#define RANK4 1<<3
#define RANK5 1<<2
#define RANK6 1

/* attack
# RANK1
1*full 5

# RANK2
1*full open 4

# RANK3
1*half open 4
1*full open 3

# RANK4
1*half open 3

# RANK5
1*two

# RANK6
1*one
*/

/* defense (block)
# RANK1
directly block an open four or five

# RANK2

# RANK3

# RANK4

*/


int dir[8][2] = {
    {0, 1},   // right
    {0, -1},  // left

    {-1, 0},  // up
    {1, 0},   // down

    {-1, -1}, // left-up
    {1, 1},   // right-down

    {1, -1},  // left-down
    {-1, 1},  // right-up
};

int INF = std::numeric_limits<int>::max();
int NEG_INF = std::numeric_limits<int>::min();

int max_search(int alpha, int beta, int depth, int score);
int min_search(int alpha, int beta, int depth, int score);