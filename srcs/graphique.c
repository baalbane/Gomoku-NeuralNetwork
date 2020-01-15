
#include "gomoku.h"


void	reset_player_list() {
	for (int i = 4; i < 31; i++) {
		SET_CURSOR(i,144);
		printf("                ");
	}
}

void	print_player_list() {
	//reset_player_list();
	if (config.pool == NULL) {
		SET_CURSOR(4,145);
		printf("None");
	}
	else {
		for (int i = 0; i < config.pool_size && i < 26; i++) {
			SET_CURSOR(4+i,145);
			printf("%*s: %d", ID_SIZE, config.pool[i]->id, config.pool[i]->real_score);
		}
	}
}

void	reset_console() {
	for (int i = 33; i < 39; i++) {
		SET_CURSOR(i,4);
		printf("                                                                 ");
	}
}

void	print_console() {
	//reset_console();
	for (int i = 4; i >= 0; i--) {
		SET_CURSOR(37-i,4);
		printf("%s", config.history[i]);
	}
}

void	print_config_nn() {
	for (int i = 0; i < config.nb_nn; i++) {
		int y = 4+(i%5)*5;
		SET_CURSOR(y++, 4+(i/5)*47);
		printf("Config nn%d:", i);
		SET_CURSOR(y++, 4+(i/5)*47);
		printf("type : nn%d | ", config.nn_spec[i]->type);
		printf("nb_inputs : %d | ", config.nn_spec[i]->nb_inputs);
		printf("nb_layer : %d", config.nn_spec[i]->nb_layer);
		SET_CURSOR(y++, 4+(i/5)*47);
		printf("neuron_per_layer :");
		for (int x = 0; x < config.nn_spec[i]->nb_layer; x++) {
			printf(" %d", config.nn_spec[i]->neuron_per_layer[x]);
		}
	}
}

void	print_config() {
	int		y;
	SET_CURSOR(3,10);
	printf("CONFIGURATIONS:");
	
	y = 5;
	SET_CURSOR(y++,4);
	printf("dftl_pool_size : %d", config.dftl_pool_size);
	SET_CURSOR(y++,4);
	printf("game_per_player : %d", config.game_per_player);
	SET_CURSOR(y++,4);
	printf("dftl_gen_per_train : %d", config.dftl_gen_per_train);
	SET_CURSOR(y++,4);
	printf("new_player_per_gen : %d", config.new_player_per_gen);
	SET_CURSOR(y++,4);
	printf("max_family_number : %d", config.max_family_number);
	y++;
	SET_CURSOR(y++,4);
	printf("map_size : %d", config.dftl_map_size);
	SET_CURSOR(y++,4);
	printf("score_win_align : %d", config.score_win_align);
	SET_CURSOR(y++,4);
	printf("score_win_capt : %d", config.score_win_capt);
	SET_CURSOR(y++,4);
	printf("score_loose : %d", config.score_loose);
	SET_CURSOR(y++,4);
	printf("score_tie : %d", config.score_tie);
	SET_CURSOR(y++,4);
	printf("score_per_capt : %d", config.score_per_capt);
	SET_CURSOR(y++,4);
	printf("score_per_turn : %d", config.score_per_turn);
	SET_CURSOR(y++,4);
	printf("max_turn_per_game : %d", config.max_turn_per_game);
	SET_CURSOR(y++,4);
	printf("game_adv : ");
	if (config.game_adv) {
		printf("%s", config.game_adv->id);
	} else {
		printf("None");
	}
	SET_CURSOR(y++,4);
	printf("double_fight : %d", config.double_fight);
	
	y++;
	SET_CURSOR(y++,4);
	printf("dflt_usleep : %d", config.dflt_usleep);
}

void	print_help() {
	SET_CURSOR(4,10);
	printf("see readme.md for more details on available commands");
}

void	print_game_g() {
	if (!config.map) {
		return ;
	}
	for (int i = 0; i < config.dftl_map_size; i++) {
		SET_CURSOR(3+i,5);
		printf("|");
		for (int j = 0; j < config.dftl_map_size; j++) {
			printf("%c|", config.map[i][j]==1?'X':config.map[i][j]==0?' ':'O');
		}
	}
}

void	print_list_players() {
	int		x;
	int		y;
	
	y = 0;
	x = 0;
	for (int i = 0; i < config.pool_size; i++) {
		SET_CURSOR(y+4,(x*14)+4);
		printf("%*s:%d", ID_SIZE, config.pool[i]->id, config.pool[i]->real_score);
		y++;
		if (y > 25) {
			y = 0;
			x++;
		}
		if (x > 9) {
			return ;
		}
	}
}

void	print_list_player() {
	Player	*p;
	Brain	*b;
	
	p = config.p_select;
	b = p->brain;
	SET_CURSOR(3,5);
	printf("Id: %s   Score: %d   Type: ", p->id, p->real_score);
	if (p->type >= PLAYER_TYPE_NN) {
		printf("nn%d", p->type-PLAYER_TYPE_NN);
	} else if (p->type == PLAYER_TYPE_HUMAN) {
		printf("Human");
	} else if (p->type > PLAYER_TYPE_HUMAN && p->type < PLAYER_TYPE_NN) {
		printf("Ia%d", p->type-PLAYER_TYPE_IA0);
	}
	printf("   Inputs: %d   layer: %d   outputs: %d", b->nb_inputs,
	b->nb_layer-1, b->neuron_per_layer[b->nb_layer-1]);
	SET_CURSOR(4,5);
	printf("Neuron per layer: ");
	for (int i = 0; i < b->nb_layer-1; i++) {
		printf("%d ", b->neuron_per_layer[i]);
	}
}

void	print_dyn_tab() {
	if (config.update.dyn_tab == TAB_CONFIG) {
		print_config();
	} else if (config.update.dyn_tab == TAB_CONFIG_NN) {
		print_config_nn();
	} else if (config.update.dyn_tab == TAB_HELP) {
		print_help();
	} else if (config.update.dyn_tab == TAB_GAME) {
		print_game_g();
	} else if (config.update.dyn_tab == TAB_PLAYERS) {
		print_list_players();
	} else if (config.update.dyn_tab == TAB_PLAYER) {
		print_list_player();
	}
}

void	reset_dyn_tab() {
	for (int i = 2; i < 30; i++) {
		SET_CURSOR(i,2);
		printf("                                                                                                                                             ");
	}
}

void	print_menu_base() {
	int		fd;
	char	tmp[WIN_WIDTH];
	
	SET_CURSOR(0,0);
	fd = open("data/menu1", O_RDONLY);
	for (int i = 0; i < WIN_HEIGHT; i++) {
		Read(fd, &(tmp), sizeof(char)*WIN_WIDTH);
		printf("%s", tmp);
		if (i+1 < WIN_HEIGHT) {
			printf("\n");
			Read(fd, &(tmp), sizeof(char));
		}
	}
	close(fd);
	
	config.update.refresh_console = 1;
	config.update.refresh_player_lst = 1;
	config.update.refresh_dyn_tab = 1;
}



void	*graph_thread(void *p_data) {
	char	check;
	
	check = 0;
	config.update.refresh = 1;	
	while (1) {
		if (config.update.refresh) {
			check++;
			config.update.refresh = 0;
			RESET_CONSOLE();
			print_menu_base();
		}
		if (config.update.refresh_console) {
			check++;
			config.update.refresh_console = 0;
			reset_console();
			print_console();
		}
		if (config.update.refresh_player_lst) {
			check++;
			config.update.refresh_player_lst = 0;
			reset_player_list();
			print_player_list();
		}
		if (config.update.refresh_dyn_tab) {
			check++;
			config.update.refresh_dyn_tab = 0;
			reset_dyn_tab();
			print_dyn_tab();
		}
		if (check) {
			check = 0;
			SET_CURSOR(WIN_HEIGHT-2,4);
			fflush(stdout);
		}
		
		usleep(config.dflt_usleep);
	}
	return (NULL);
}