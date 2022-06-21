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
# 1e7
1*full 5

# 2^5
1*full open 4

# 2^4
1*half open 4
1*full open 3

# 2^3
1*half open 3

# 2^2
1*two

# 1
1*one
*/

/* defense (block)
place at this pos as if i'm the enemy
gain higher score if the enemy can gain more by placing at this pos.

the higher the score (i.e. the more valuable this pos is to the enemy), 
the more damage i caused by placing at this position.
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