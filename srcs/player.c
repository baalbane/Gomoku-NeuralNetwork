
#include "gomoku.h"


void	p_delete(Player *p) {
	b_delete(p->brain);
	free(p);
}

void	nn0_new_brain(Player *new) {
	int	old_output;
	
	if (config.dflt_inputs != 38) {
		add_to_history("dflt_inputs set to 38 for this type of network");
	}
	old_output = config.dflt_neuron_per_layer[config.dflt_nb_layer-1];
	if (old_output != 36) {
		add_to_history("output set to 36 for this type of network");
	}
	config.dflt_neuron_per_layer[config.dflt_nb_layer-1] = 36;
	new->brain = b_new(38, config.dflt_nb_layer, config.dflt_neuron_per_layer);
	config.dflt_neuron_per_layer[config.dflt_nb_layer-1] = old_output;
}

void	nn1_new_brain(Player *new) {
	int	old_output;
	
	if (config.dflt_inputs != 38) {
		add_to_history("dflt_inputs set to 38 for this type of network");
	}
	old_output = config.dflt_neuron_per_layer[config.dflt_nb_layer-1];
	if (old_output != 1) {
		add_to_history("output set to 1 for this type of network");
	}
	config.dflt_neuron_per_layer[config.dflt_nb_layer-1] = 1;
	new->brain = b_new(38, config.dflt_nb_layer, config.dflt_neuron_per_layer);
	config.dflt_neuron_per_layer[config.dflt_nb_layer-1] = old_output;
}

Player	*p_new(int type) {
	Player	*new;
	
	new = malloc(sizeof(Player));
	get_new_id(new->id);
	new->type = type;
	new->score = 0;
	new->real_score = 0;
	if (new->type == PLAYER_TYPE_NN0) {
		nn0_new_brain(new);
	} else if (new->type == PLAYER_TYPE_NN1) {
		nn1_new_brain(new);
	} else {
		new->brain = NULL;
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
	inputs = malloc(sizeof(float) * DEFAULT_INPUTS);
	inputs[DEFAULT_INPUTS-2] = (float)p->capt_me / 10.0;
	inputs[DEFAULT_INPUTS-1] = (float)p->capt_adv / 10.0;
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

TYPE_MOVE	human_next_move(Game_player *p) {
	char	buf[65];
	int		ret;
	
	//SET_CURSOR(WIN_HEIGHT-2,4);
	//fflush(stdout);
	ret = read(STDIN_FILENO, buf, 64);
	buf[ret] = '\0';
	add_to_history(buf);
	config.update.refresh_console = 1;
	return (atoi(buf));
}

TYPE_MOVE	choose_next_move(Game_player *p) {
	
	if (p->player->type == PLAYER_TYPE_NN0) {
		return (nn0_next_move(p));
	} else if (p->player->type == PLAYER_TYPE_NN1) {
		printf("DIDN'T CODE THAT YET\n");
	} else if (p->player->type == PLAYER_TYPE_HUMAN) {
		return (human_next_move(p));
	} else if (p->player->type == PLAYER_TYPE_IA) {
		printf("DIDN'T CODE THAT YET\n");
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


char		*player_type_str(int type) {
	if (type == PLAYER_TYPE_NN0) {
		return ("nn0");
	} else if (type == PLAYER_TYPE_NN1) {
		return ("nn1");
	} else if (type == PLAYER_TYPE_HUMAN) {
		return ("human");
	} else if (type == PLAYER_TYPE_IA) {
		return ("ia");
	}
	return ("UNKNOW PLAYER TYPE");
}










