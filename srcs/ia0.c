
#include "gomoku.h"


//Niveau 0: Only try to form the longest line possible


static int	map_is_empty(char **map) {
	for (int y = 0; y < config.dftl_map_size; y++) {
		for (int x = 0; x < config.dftl_map_size; x++) {
			if (map[y][x]) {
				return (0);
			}
		}
	}
	return (1);
}

static int	count(char **map, int x, int y, int addx, int addy) {
	int		score;
	int		score0 = 11;
	int		score1 = 5;
	
	score = 0;
	while (x > 0 && x < config.dftl_map_size
	&& y > 0 && y < config.dftl_map_size && map[y][x] != -1) {
		if (map[y][x] == 1) {
			score += score0;
		} else {
			score += score1;
		}
		if (score0) {
			score0--;
		}
		if (score1) {
			score1--;
		}
		x += addx;
		y += addy;
	}
	return (score);
}

static int	get_move_score(char **map, int x, int y) {
	int		score;
	int		best;
	int		toto[4][2] = {{1, 0}, {0, 1}, {1, 1}, {-1, 1}};
	
	best = -9999999;
	for (int i = 0; i < 4; i++) {
		score = count(map, x, y, toto[i][0], toto[i][1]);
		score += count(map, x, y, toto[i][0]*-1, toto[i][1]*-1);
		if (score > best) {
			best = score;
		}
	}
	return (best*10+rand()%9);
}


TYPE_MOVE	ia0_next_move(Game_player *p) {
	TYPE_MOVE	move;
	int			best;
	int			tmp;
	
	if (map_is_empty(p->map)) {
		return (GET_MOVE(config.dftl_map_size/2, config.dftl_map_size/2));
	}
	best = -99999;
	move = -1;
	
	
	for (int y = 0; y < config.dftl_map_size; y++) {
		for (int x = 0; x < config.dftl_map_size; x++) {
			if (p->map[y][x] == 0) {
				p->map[y][x] = 1;
				tmp = get_move_score(p->map, x, y);
				if (tmp > best) {
					best = tmp;
					move = GET_MOVE(x, y);
				}
				p->map[y][x] = 0;
			}
		}
	}
	return (move);
}


























