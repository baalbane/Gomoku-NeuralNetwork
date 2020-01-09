
#include "gomoku.h"

void	reset_players() {
	for (int i = 0; i < config.pool_size; i++) {
		config.pool[i]->score = 0;
		config.pool[i]->nb_fights = 0;
	}
}

Player	*get_fightless_player(Player *exclude) {
	int		start_pos;
	Player	*best;
	int		best_fight;
	
	start_pos = rand()%config.pool_size;
	while (config.pool[start_pos] == exclude) {
		start_pos = rand()%config.pool_size;
	}
	best = config.pool[start_pos];
	best_fight = config.pool[start_pos]->nb_fights;
	for (int i = start_pos+1; i != start_pos; i++) {
		if (i >= config.pool_size) {
			i = -1;
			continue;
		}
		if (config.pool[i] != exclude && config.pool[i]->nb_fights < best_fight) {
			best = config.pool[i];
			best_fight = config.pool[i]->nb_fights;
		}
	}
	if (best_fight >= config.game_per_player) {
		return (NULL);
	}
	return (best);
}

int		make_game() {
	Player	*p1 = get_fightless_player(NULL);
	Player	*p2 = get_fightless_player(p1);
	
	if (!p1 || !p2) {
		return (0);
	}
	p1->nb_fights++;
	p2->nb_fights++;
	new_game(p1, p2);
	p1->real_score = p1->score;
	p2->real_score = p2->score;
	return (1);
}

void	*train(void	*data) {
	config.train = 1;
	while ((config.total_gen < 0 || config.actual_gen <= config.total_gen) && config.train) {
		reset_players();
		while (make_game()){;}
		mutate();
		config.update.refresh_player_lst = 1;
		if (config.total_gen > 0) {
			config.actual_gen++;
		}
		//sleep(1);
	}
	config.train = 0;
	return NULL;
}