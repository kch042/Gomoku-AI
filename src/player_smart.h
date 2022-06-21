#include <limits>

#define FIVE 1000

int INF = std::numeric_limits<int>::max();
int NEG_INF = std::numeric_limits<int>::min();

int max_search(int alpha, int beta, int depth);
int min_search(int alpha, int beta, int depth);