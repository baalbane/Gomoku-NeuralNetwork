
#include "gomoku.h"

Player	*get_best_player(int *family) {
	int		start_pos;
	Player	*best;
	int		best_score;
	
	start_pos = rand()%config.pool_size;
	best = config.pool[start_pos];
	best_score = config.pool[start_pos]->score;
	for (int i = start_pos+1; i != start_pos; i++) {
		if (i >= config.pool_size) {
			i = -1;
			continue;
		}
		if (config.pool[i]->score > best_score
		&& family[config.pool[i]->id[0] - 'A'] < config.max_family_number) {
			best = config.pool[i];
			best_score = config.pool[i]->score;
		}
	}
	best->score = -42042042;
	family[best->id[0] - 'A']++;
	return (best);
}

int		mutate() {
	Player	**new;
	Player	**tmp;
	int		cu;
	int		family[30];
	
	memset(family, 0, sizeof(family));
	new = malloc(sizeof(Player*) * config.pool_size);
	for (cu = 0; cu < config.pool_size/2; cu++) {
		new[cu] = get_best_player(family);
	}
	for (;cu < config.pool_size-config.new_player_per_gen; cu++) {
		new[cu] = p_copy(new[cu-config.pool_size/2]);
		p_mutate(new[cu]);
	}
	for (;cu < config.pool_size; cu++) {
		new[cu] = p_new(PLAYER_TYPE_NN+rand()%config.nb_nn);
	}
	
	tmp = config.pool;
	config.pool = new;
	for (cu = 0; cu < config.pool_size; cu++) {
		if (tmp[cu]->score != -42042042) {
			p_delete(tmp[cu]);
		}
	}
	free(tmp);
	return (1);
}