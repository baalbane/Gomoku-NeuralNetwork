
#include "gomoku.h"



void		delete_game(Game_player **old) {
	for (int y = 0; y < config.dftl_map_size; y++) {
		free(old[1]->map[y]);
		free(old[0]->map[y]);
	}
	free(old[1]->map);
	free(old[0]->map);
	free(old[1]);
	free(old[0]);
	free(old);
}

void		delete_print_game(Game_player **old) {
	for (int y = 0; y < config.dftl_map_size; y++) {
		free(old[1]->map[y]);
		free(old[0]->map[y]);
	}
	free(old[1]->map);
	free(old[0]->map);
	p_delete(old[1]->player);
	p_delete(old[0]->player);
	free(old[1]);
	free(old[0]);
	free(old);
	config.map = NULL;
	config.update.refresh_player_lst = 1;
}

Game_player	**init_game(Player *p1, Player *p2) {
	Game_player	**new;
	
	new = malloc(sizeof(Game_player*) * 2);
	new[0] = malloc(sizeof(Game_player));
	new[1] = malloc(sizeof(Game_player));
	new[0]->player = p1;
	new[1]->player = p2;
	new[0]->capt_me = 0;
	new[1]->capt_me = 0;
	new[0]->capt_adv = 0;
	new[1]->capt_adv = 0;
	new[0]->map = malloc(sizeof(char*) * config.dftl_map_size);
	new[1]->map = malloc(sizeof(char*) * config.dftl_map_size);
	for (int y = 0; y < config.dftl_map_size; y++) {
		new[0]->map[y] = malloc(sizeof(char) * config.dftl_map_size);
		new[1]->map[y] = malloc(sizeof(char) * config.dftl_map_size);
		for (int x = 0; x < config.dftl_map_size; x++) {
			new[0]->map[y][x] = 0;
			new[1]->map[y][x] = 0;
		}
	}
	return (new);
}


int print_map(char **map) {
	for (int y = 0; y < config.dftl_map_size; y++) {
		printf("\n|");
		for (int x = 0; x < config.dftl_map_size; x++) {
			printf("%c|", map[y][x]==1?'X':map[y][x]==0?' ':'O');
		}
	}
	printf("\n\n");
	return (1);
}


int		new_game(Player *p1, Player *p2) {
	int			turn;
	Game_player	**players;
	TYPE_MOVE	move;
	
	turn = -1;
	players = init_game(p1, p2);
	while (++turn < config.max_turn_per_game) {
		move = choose_next_move(players[0]);
		if (apply_move(players[0], players[1], move)) {
			delete_game(players);
			return (1);
		}
		++turn;
		move = choose_next_move(players[1]);
		if (apply_move(players[1], players[0], move)) {
			delete_game(players);
			return (1);
		}
		p1->score += config.score_per_turn;
		p2->score += config.score_per_turn;
	}
	p1->score += config.score_tie;
	p2->score += config.score_tie;
	delete_game(players);
	return (1);
}


void	print_print_game(char **map) {
	for (int i = 0; i < config.dftl_map_size; i++) {
		SET_CURSOR(3+i,5);
		printf("|");
		for (int j = 0; j < config.dftl_map_size; j++) {
			printf("%c|", map[i][j]==1?'X':map[i][j]==0?' ':'O');
		}
	}
}


int		print_game(Player *p1, Player *p2) {
	Game_player	**players;
	TYPE_MOVE	move;
	
	players = init_game(p_copy(p1), p_copy(p2));
	config.map = players[0]->map;
	config.update.refresh_console = 1;
	config.update.refresh_dyn_tab = 1;
	while (1) {
		move = choose_next_move(players[0]);
		if (apply_move(players[0], players[1], move)) {
			config.update.refresh_dyn_tab = 1;
			usleep(500000);
			delete_print_game(players);
			return (1);
		}
		config.update.refresh_dyn_tab = 1;
		usleep(500000);
		move = choose_next_move(players[1]);
		if (apply_move(players[1], players[0], move)) {
			config.update.refresh_dyn_tab = 1;
			usleep(500000);
			delete_print_game(players);
			return (1);
		}
		config.update.refresh_dyn_tab = 1;
		usleep(500000);
	}
	return (0);	
}
































