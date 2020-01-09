
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

void	print_config() {
	SET_CURSOR(3,10);
	printf("CONFIGURATIONS:");
	
	SET_CURSOR(5,4);
	printf("dflt_inputs : %d", config.dflt_inputs);
	SET_CURSOR(6,4);
	printf("dflt_nb_layer : %d", config.dflt_nb_layer);
	SET_CURSOR(7,4);
	printf("dflt_neuron_per_layer : ");
	for (int i = 0; i < config.dflt_nb_layer; i++) {
		printf("%d ", config.dflt_neuron_per_layer[i]);
	}
	
	SET_CURSOR(9,4);
	printf("dftl_pool_size : %d", config.dftl_pool_size);
	SET_CURSOR(10,4);
	printf("dftl_player_type : %s", player_type_str(config.dftl_player_type));
	SET_CURSOR(11,4);
	printf("game_per_player : %d", config.game_per_player);
	SET_CURSOR(12,4);
	printf("dftl_gen_per_train : %d", config.dftl_gen_per_train);
	
	SET_CURSOR(14,4);
	printf("map_size : %d", config.dftl_map_size);
	SET_CURSOR(15,4);
	printf("score_win_align : %d", config.score_win_align);
	SET_CURSOR(16,4);
	printf("score_win_capt : %d", config.score_win_capt);
	SET_CURSOR(17,4);
	printf("score_loose : %d", config.score_loose);
	SET_CURSOR(18,4);
	printf("score_tie : %d", config.score_tie);
	SET_CURSOR(19,4);
	printf("score_per_capt : %d", config.score_per_capt);

	SET_CURSOR(21,4);
	printf("dflt_usleep : %d", config.dflt_usleep);
}

void	print_help() {
	SET_CURSOR(3,10);
	printf("COMMANDS:");
	
	SET_CURSOR(5,4);
	printf("set variable value | exemple: set dflt_neuron_per_layer 1 2 3 4 5 6\n");
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


void	print_dyn_tab() {
	if (config.update.dyn_tab == TAB_CONFIG) {
		print_config();
	} else if (config.update.dyn_tab == TAB_HELP) {
		print_help();
	} else if (config.update.dyn_tab == TAB_GAME) {
		print_game_g();
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