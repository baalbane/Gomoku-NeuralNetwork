
#include "gomoku.h"


void	p_delete(Player *p) {
	b_delete(p->brain);
	free(p);
}

int		p_new_brain(Player *new, int config_type) {
	Config_nn *conf;
	
	if (config_type >= config.nb_nn) {
		add_to_history("config not exist");
		return (0);
	}
	conf = config.nn_spec[config_type];
	new->type = conf->type+PLAYER_TYPE_NN;
	new->brain = b_new(conf->nb_inputs, conf->nb_layer, conf->neuron_per_layer);	
	return (1);
}

Player	*p_new(int type) {
	Player	*new;
	
	new = malloc(sizeof(Player));
	get_new_id(new->id);
	new->type = type;
	new->score = 0;
	new->real_score = 0;
	if (new->type >= PLAYER_TYPE_HUMAN && new->type < PLAYER_TYPE_NN) {
		new->brain = NULL;
	} else if (new->type >= PLAYER_TYPE_NN) {
		if (!p_new_brain(new, new->type-PLAYER_TYPE_NN)) {
			free(new);
			return NULL;
		}
	} else {
		printf("Error: Unkow player type, should not appened\n");
		exit(0);
	}
	return (new);
}

Player	*p_copy(Player	*old) {
	Player	*new;
	
	new = malloc(sizeof(Player));
	memcpy(new->id, old->id, sizeof(char)*(ID_SIZE+1));
	new->type = old->type;
	new->score = 0;
	new->real_score = 0;
	new->brain = b_copy(old->brain);
	return (new);
}

Player	*p_load(int fd) {
	Player	*new;
	
	new = malloc(sizeof(Player));
	Read(fd, &(new->id), sizeof(new->id));
	Read(fd, &(new->type), sizeof(new->type));
	new->score = 0;
	new->real_score = 0;
	new->brain = b_load(fd);
	return (new);
}

void	p_save(Player *p, int fd) {
	Write(fd, &(p->id), sizeof(p->id));
	Write(fd, &(p->type), sizeof(p->type));
	b_save(p->brain, fd);
}

void	p_mutate(Player *p) {
	get_next_id2(p->id);
	b_mutate(p->brain);
}

TYPE_MOVE	nn0_next_move(Game_player *p) {
	float		**res;
	float		*inputs;
	float		*outputs;
	TYPE_MOVE	move;
	float		best;
	
	res = malloc(sizeof(float*) * config.dftl_map_size);
	for (int y = 0; y < config.dftl_map_size; y++) {
		res[y] = malloc(sizeof(float) * config.dftl_map_size);
		for (int x = 0; x < config.dftl_map_size; x++) {
			res[y][x] = -99.0;
		}
	}
	inputs = malloc(sizeof(float) * p->player->brain->nb_inputs);
	inputs[p->player->brain->nb_inputs-2] = (float)p->capt_me / 10.0;
	inputs[p->player->brain->nb_inputs-1] = (float)p->capt_adv / 10.0;
	for (int by = 0; by < config.dftl_map_size-NN_MAP_SIZE+1; by++) {
		for (int bx = 0; bx < config.dftl_map_size-NN_MAP_SIZE+1; bx++) {
			for (int y = 0; y < NN_MAP_SIZE; y++) {
				for (int x = 0; x < NN_MAP_SIZE; x++) {
					inputs[x+(y*NN_MAP_SIZE)] = (float)p->map[y+by][x+bx];
				}
			}
			outputs = b_launch(p->player->brain, inputs);
			for (int y = 0; y < NN_MAP_SIZE; y++) {
				for (int x = 0; x < NN_MAP_SIZE; x++) {
					if (outputs[x+(y*NN_MAP_SIZE)] > res[y+by][x+bx]) {
						res[y+by][x+bx] = outputs[x+(y*NN_MAP_SIZE)];
					}
				}
			}
			free(outputs);
		}
	}
	free(inputs);
	
	//CHOOSE MOVE
	move = 0;
	best = -9999.0;
	
	for (int y = 0; y < config.dftl_map_size; y++) {
		for (int x = 0; x < config.dftl_map_size; x++) {
			if (p->map[y][x] == 0 && res[y][x] > best) {
				best = res[y][x];
				move = GET_MOVE(x, y);
			}
		}
	}
	
	for (int x = 0; x < config.dftl_map_size; x++) {
		free(res[x]);
	}
	free(res);
	if (best < -999.0) {
		return (-1);
	}
	return (move);
}

TYPE_MOVE	nn1_next_move(Game_player *p) {
	float		*inputs;
	float		*outputs;
	TYPE_MOVE	move;
	float		best;
	float		tmp;
	
	move = -1;
	best = -9999999.9;
	inputs = malloc(sizeof(float) * p->player->brain->nb_inputs);
	inputs[p->player->brain->nb_inputs-2] = (float)p->capt_me / 10.0;
	inputs[p->player->brain->nb_inputs-1] = (float)p->capt_adv / 10.0;
	for (int yy = 0; yy < config.dftl_map_size; yy++) {
		for (int xx = 0; xx < config.dftl_map_size; xx++) {
			if (p->map[yy][xx] != 0) {
				continue;
			}
			p->map[yy][xx] = 1;
			tmp = 0.0;
			for (int by = 0; by < config.dftl_map_size-NN_MAP_SIZE+1; by++) {
				for (int bx = 0; bx < config.dftl_map_size-NN_MAP_SIZE+1; bx++) {
					for (int y = 0; y < NN_MAP_SIZE; y++) {
						for (int x = 0; x < NN_MAP_SIZE; x++) {
							inputs[x+(y*NN_MAP_SIZE)] = (float)p->map[y+by][x+bx];
						}
					}
					outputs = b_launch(p->player->brain, inputs);
					tmp += outputs[0];
					free(outputs);
				}
			}
			if (tmp > best) {
				best = tmp;
				move = GET_MOVE(xx, yy);
			}
			p->map[yy][xx] = 0;
		}
	}
	free(inputs);
	return (move);
}

TYPE_MOVE	human_next_move(Game_player *p) {
	char	buf[65];
	int		ret;
	
	ret = read(STDIN_FILENO, buf, 64);
	buf[ret] = '\0';
	add_to_history(buf);
	config.update.refresh_console = 1;
	return (atoi(buf));
}

TYPE_MOVE	choose_next_move(Game_player *p) {
	
	if (p->player->type >= PLAYER_TYPE_NN) {
		if (p->player->type-PLAYER_TYPE_NN == NN_TYPE_0)
			return (nn0_next_move(p));
		else if (p->player->type-PLAYER_TYPE_NN == NN_TYPE_1)
			return (nn1_next_move(p));
	} else if (p->player->type == PLAYER_TYPE_HUMAN) {
		return (human_next_move(p));
	} else if (p->player->type == PLAYER_TYPE_IA0) {
		return (ia0_next_move(p));
	} else if (p->player->type == PLAYER_TYPE_IA1) {
		return (ia1_next_move(p));
	} else if (p->player->type == PLAYER_TYPE_IA2) {
		return (ia2_next_move(p));
	} else {
		printf("ERROR, UNDEFINED PLAYER TYPE\n");
		exit(0);
	}
}


Player		*get_player(char *id) {
	for (int i = 0; i < config.pool_size; i++) {
		if (!strcmp(config.pool[i]->id, id)) {
			return (config.pool[i]);
		}
	}
	return (NULL);
}












