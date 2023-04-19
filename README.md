# A simple gomoku AI

NTHU Final Project

## Algorithm
Minimax with alpha beta pruning and iterative deepening

## Dependency
- linux: timeout
- macos: gtimeout

## Usage
Go to the source code directory
```
$ cd src
```

Compile
```
$ make
```

Run
```
$ ./main ./[agent1] ./[agent2]
```
where `[agent1]` and `[agent2]` can be
- `player_random`: random agent
- `player_smart`:  smart agent using minimax algorithm
