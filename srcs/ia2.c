
#include "gomoku.h"


//Niveau 2: 
// check win, then defend, then try to do 5 in a row


static int	map_is_empty(char **map) {
	for (int y = 0; y < config.dftl_map_size; y++) {
		for (int x = 0; x < config.dftl_map_size; x++) {
			if (map[y][x] != 0) {
				return (0);
			}
		}
	}
	return (1);
}

static int	count(char **map, int xx, int yy, int addx, int addy) {
	int		score;
	int		score0;
	int		score1;
	int		x;
	int		y;
	int		check;
	
	score = 0;
	check = 0;
	score0 = 11;
	score1 = 5;
	x = xx;
	y = yy;
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
		check++;
	}
	
	score0 = 11;
	score1 = 5;
	x = xx;
	y = yy;
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
		x -= addx;
		y -= addy;
		check++;
	}
	if (check < 6) {
		return (0);
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
		if (score > best) {
			best = score;
		}
	}
	return (best*10+rand()%9);
}


static int	check_win_loop(char **map, int xx, int yy, int addx, int addy) {
	int		x;
	int		y;
	int		check;
	
	check = 0;
	x = xx;
	y = yy;
	while (x > 0 && x < config.dftl_map_size
	&& y > 0 && y < config.dftl_map_size && map[y][x] == 1) {
		x += addx;
		y += addy;
		check++;
	}
	
	x = xx - addx;
	y = yy - addy;
	while (x > 0 && x < config.dftl_map_size
	&& y > 0 && y < config.dftl_map_size && map[y][x] == 1) {
		x -= addx;
		y -= addy;
		check++;
	}
	if (check < 5) {
		return (0);
	}
	return (1);
}

static int	check_win(char **map, int x, int y) {
	int		toto[4][2] = {{1, 0}, {0, 1}, {1, 1}, {-1, 1}};
	
	for (int i = 0; i < 4; i++) {
		if (check_win_loop(map, x, y, toto[i][0], toto[i][1])) {
			return (1);
		}
	}
	return (0);
}


static int	check_loose_loop(char **map, int xx, int yy, int addx, int addy) {
	int			x;
	int			y;
	int			enemy;
	int			empty;
	TYPE_MOVE	move;
	
	move = -1;
	enemy = 0;
	empty = 0;
	x = xx;
	y = yy;
	while (x > 0 && x < config.dftl_map_size
	&& y > 0 && y < config.dftl_map_size
	&& map[y][x] != 1 && empty < 3) {
		if (map[y][x] == -1) {
			enemy++;
		} else {
			if (move == -1) {
				move = GET_MOVE(x, y);
			}
			empty++;
		}
		x += addx;
		y += addy;
	}
	
	x = xx - addx;
	y = yy - addy;
	while (x > 0 && x < config.dftl_map_size
	&& y > 0 && y < config.dftl_map_size
	&& (map[y][x] == -1 || (map[y][x] == 0 && empty < 3))) {
		if (map[y][x] == -1) {
			enemy++;
		} else {
			if (move == -1) {
				move = GET_MOVE(x, y);
			}
			empty++;
		}
		x -= addx;
		y -= addy;
	}
	if (enemy >= 4 && empty >= 1) {
		return (move);
	} else if (enemy == 3 && empty >= 2) {
		return (move);
	}
	return (-1);
}

static int	check_loose(char **map, int x, int y) {
	int			toto[4][2] = {{1, 0}, {0, 1}, {1, 1}, {-1, 1}};
	TYPE_MOVE	move;
	
	for (int i = 0; i < 4; i++) {
		if ((move = check_loose_loop(map, x, y, toto[i][0], toto[i][1])) != -1) {
			return (move);
		}
	}
	return (-1);
}


TYPE_MOVE	ia2_next_move(Game_player *p) {
	TYPE_MOVE	move;
	int			best;
	int			tmp;
	
	//1st move only
	if (map_is_empty(p->map)) {
		return (GET_MOVE(config.dftl_map_size/2, config.dftl_map_size/2));
	}
	
	//check win
	for (int y = 0; y < config.dftl_map_size; y++) {
		for (int x = 0; x < config.dftl_map_size; x++) {
			if (p->map[y][x] == 0) {
				p->map[y][x] = 1;
				if (check_win(p->map, x, y)) {
					p->map[y][x] = 0;
					return (GET_MOVE(x, y));
				}
				p->map[y][x] = 0;
			}
		}
	}
	
	//defend
	for (int y = 0; y < config.dftl_map_size; y++) {
		for (int x = 0; x < config.dftl_map_size; x++) {
			if (p->map[y][x] == -1 && (move = check_loose(p->map, x, y)) != -1) {
				return (move);
			}
		}
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


























