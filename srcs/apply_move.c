
#include "gomoku.h"

void add_capt(Game_player *me, char a) {
	if (a == 1) {
		me->capt_me++;
		me->player->score += config.score_per_capt;
	} else {
		me->capt_adv++;
	}
}

void	apply_capt(Game_player *me, Game_player *adv, TYPE_MOVE move) {
	int x = GET_MOVE_X(move);
	int y = GET_MOVE_Y(move);
	char PL1 = me->map[y][x];
	char PL2 = me->map[y][x]*-1;
	
	//LEFT
	if (x > 2 && me->map[y][x-1] == PL2 && me->map[y][x-2] == PL2 && me->map[y][x-3] == PL1) {
		me->map[y][x-1] = 0;
		me->map[y][x-2] = 0;
		add_capt(me, PL1);
	}
	//RIGHT
	if (x < config.dftl_map_size-3 && me->map[y][x+1] == PL2 && me->map[y][x+2] == PL2 && me->map[y][x+3] == PL1) {
		me->map[y][x+1] = 0;
		me->map[y][x+2] = 0;
		add_capt(me, PL1);
	}
	//TOP
	if (y > 2 && me->map[y-1][x] == PL2 && me->map[y-2][x] == PL2 && me->map[y-3][x] == PL1) {
		me->map[y-1][x] = 0;
		me->map[y-2][x] = 0;
		add_capt(me, PL1);
	}
	//BOT
	if (y < config.dftl_map_size-3 && me->map[y+1][x] == PL2 && me->map[y+2][x] == PL2 && me->map[y+3][x] == PL1) {
		me->map[y+1][x] = 0;
		me->map[y+2][x] = 0;
		add_capt(me, PL1);
	}
	
	//TOP-LEFT
	if (x > 2 && y > 2 &&
	me->map[y-1][x-1] == PL2 && me->map[y-2][x-2] == PL2 && me->map[y-3][x-3] == PL1) {
		me->map[y-1][x-1] = 0;
		me->map[y-2][x-2] = 0;
		add_capt(me, PL1);
	}
	//TOP-RIGHT
	if (x < config.dftl_map_size-3 && y > 2 &&
	me->map[y-1][x+1] == PL2 && me->map[y-2][x+2] == PL2 && me->map[y-3][x+3] == PL1) {
		me->map[y-1][x+1] = 0;
		me->map[y-2][x+2] = 0;
		add_capt(me, PL1);
	}
	//BOT-LEFT
	if (x > 2 && y < config.dftl_map_size-3 &&
	me->map[y+1][x-1] == PL2 && me->map[y+2][x-2] == PL2 && me->map[y+3][x-3] == PL1) {
		me->map[y+1][x-1] = 0;
		me->map[y+2][x-2] = 0;
		add_capt(me, PL1);
	}
	//BOT-RIGHT
	if (x < config.dftl_map_size-3 && y < config.dftl_map_size-3 &&
	me->map[y+1][x+1] == PL2 && me->map[y+2][x+2] == PL2 && me->map[y+3][x+3] == PL1) {
		me->map[y+1][x+1] = 0;
		me->map[y+2][x+2] = 0;
		add_capt(me, PL1);
	}
}

int		check_win(Game_player *me, TYPE_MOVE move, int add_x, int add_y) {
	int	check;
	int x = GET_MOVE_X(move);
	int y = GET_MOVE_Y(move);
	
	check = 1;
	while ((x-add_x) >= 0 && (y-add_y) >= 0 &&
	me->map[y-add_y][x-add_x] == 1) {
		check++;
		x -= add_x;
		y -= add_y;
	}
	x = GET_MOVE_X(move);
	y = GET_MOVE_Y(move);
	while ((x+add_x) < config.dftl_map_size && (y+add_y) < config.dftl_map_size &&
	me->map[y+add_y][x+add_x] == 1) {
		check++;
		x += add_x;
		y += add_y;
	}
	if (check >= 5) {
		return (1);
	}
	return (0);
}

int fucking_diagonal(Game_player *me, TYPE_MOVE move) {
	int	check;
	int x = GET_MOVE_X(move);
	int y = GET_MOVE_Y(move);
	
	check = 1;
	while ((x-1) >= 0 && (y+1) < config.dftl_map_size
	&& me->map[y+1][x-1] == 1) {
		check++;
		x--;
		y++;
	}
	x = GET_MOVE_X(move);
	y = GET_MOVE_Y(move);
	while ((x+1) < config.dftl_map_size && (y-1) >= 0
	&& me->map[y-1][x+1] == 1) {
		check++;
		x++;
		y--;
	}
	if (check >= 5) {
		return (1);
	}
	return (0);
}

int		check_wins(Game_player *me, Game_player *adv, TYPE_MOVE move) {
	if (check_win(me, move, 1, 0) || check_win(me, move, 0, 1)
		|| check_win(me, move, 1, 1) || fucking_diagonal(me, move)) {
		me->player->score += config.score_win_align;
		adv->player->score += config.score_loose;
		add_to_history("Win by alignment");
		return (1);
	}
	return (0);
}

int		apply_move(Game_player *me, Game_player *adv, TYPE_MOVE move) {
	int x;
	int y;
	
	if (move == -1) {
		me->player->score += config.score_tie;
		adv->player->score += config.score_tie;
		add_to_history("No more move, it's a tie!");
		return (-1);
	}
	x = GET_MOVE_X(move);
	y = GET_MOVE_Y(move);
	me->map[y][x] = 1;
	adv->map[y][x] = -1;
	
	apply_capt(me, adv, move);
	apply_capt(adv, me, move);
	
	if (me->capt_me == 5) {
		me->player->score += config.score_win_capt;
		adv->player->score += config.score_loose;
		add_to_history("Win by capture");
		return (1);
	}
	return (check_wins(me, adv, move));
}































