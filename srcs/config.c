
#include "gomoku.h"

Config 		config;
pthread_t	train_thread;


int		init_config() {
	config.dflt_inputs = DEFAULT_INPUTS;
	config.dflt_nb_layer = DEFAULT_LAYER;
	config.dflt_neuron_per_layer = malloc(sizeof(int) * config.dflt_nb_layer);
	for (int i = 0; i < config.dflt_nb_layer; i++) {
		config.dflt_neuron_per_layer[i] = DEFAULT_N_PER_L;
	}
	config.dflt_usleep = DEFAULT_USLEEP;
	config.dftl_pool_size = DEFAULT_POOL_SIZE;
	config.dftl_map_size = DEFAULT_MAP_SIZE;
	config.max_turn_per_game = DEFAULT_MAX_TURN_PER_GAME;
	config.dftl_player_type = DEFAULT_PLAYER_TYPE;
	config.game_per_player = GAME_PER_PLAYER;
	config.dftl_gen_per_train = DEFAULT_GEN_PER_TRAIN;
	config.new_player_per_gen = DEFAULT_NEW_P_PER_GEN;
	config.train = 0;
	config.score_win_align = DEFAULT_SCORE_WIN_ALIGN;
	config.score_win_capt = DEFAULT_SCORE_WIN_CAPT;
	config.score_loose = DEFAULT_SCORE_LOOSE;
	config.score_tie = DEFAULT_SCORE_TIE;
	config.score_per_capt = DEFAULT_SCORE_PER_CAPT;
	config.map = NULL;
	config.pool = NULL;
	config.actual_menu = MENU_1;
	config.pool_size = 0;
	for (int i = 0; i < 5; i++) {
		config.history[i][0] = '\0';
	}
	config.p_select = NULL;
	memset(&(config.update), 0, sizeof(graph_updt));
	return (1);
}

void	add_to_history(char *tmp) {
	for (int i = 3; i >= 0; i--) {
		memcpy(config.history[i+1], config.history[i], sizeof(char)*65);
	}
	memcpy(config.history[0], tmp, sizeof(char)*65);
}

void	add_to_last_history(char *tmp) {
	strcat(config.history[0], tmp);
}

void	set_dflt_neuron_per_layer(char *data) {
	int j;
	
	j = 0;
	config.dflt_neuron_per_layer[0] = 0;
	for (int i = 0; data[i]; i++) {
		if (data[i] == ' ') {
			config.dflt_neuron_per_layer[++j] = 0;
			continue;
		}
		config.dflt_neuron_per_layer[j] = config.dflt_neuron_per_layer[j]*10+data[i]-'0';
	}
}

void	set_dflt_nb_layer(int new) {
	config.dflt_neuron_per_layer = realloc(config.dflt_neuron_per_layer, sizeof(int)*new);
	if (new > config.dflt_nb_layer) {
		for (int i = config.dflt_nb_layer; i < new; i++) {
			config.dflt_neuron_per_layer[i] = DEFAULT_N_PER_L;
		}
	}
	config.dflt_nb_layer = new;
}

void	set_dftl_player_type(char *opt) {
	if (!strncmp(opt, "nn0", 3)) {
		config.dftl_player_type = PLAYER_TYPE_NN0;
	} else if (!strncmp(opt, "nn1", 3)) {
		config.dftl_player_type = PLAYER_TYPE_NN1;
	} else if (!strncmp(opt, "human", 5)) {
		config.dftl_player_type = PLAYER_TYPE_HUMAN;
	} else if (!strncmp(opt, "ia", 2)) {
		config.dftl_player_type = PLAYER_TYPE_IA;
	} else {
		add_to_history("Error: Unknow player type");
	}
}

void	set_cmd(char *cmd) {
	if (!strncmp(cmd, "dflt_inputs", 11)) {
		//config.dflt_inputs = atoi(cmd+12);
		add_to_history("dflt_inputs should stay 38");
	} else if (!strncmp(cmd, "dflt_nb_layer", 13)) {
		set_dflt_nb_layer(atoi(cmd+14));
	} else if (!strncmp(cmd, "dflt_neuron_per_layer", 21)) {
		set_dflt_neuron_per_layer(cmd+22);
	} else if (!strncmp(cmd, "dflt_usleep", 11)) {
		config.dflt_usleep = atoi(cmd+12);
	} else if (!strncmp(cmd, "dftl_pool_size", 14)) {
		config.dftl_pool_size = atoi(cmd+15);
	} else if (!strncmp(cmd, "dftl_player_type", 16)) {
		set_dftl_player_type(cmd+17);
	} else if (!strncmp(cmd, "game_per_player", 15)) {
		config.game_per_player = atoi(cmd+16);
	} else if (!strncmp(cmd, "dftl_gen_per_train", 18)) {
		config.dftl_gen_per_train = atoi(cmd+19);
	} else if (!strncmp(cmd, "map_size", 8)) {
		config.dftl_map_size = atoi(cmd+9);
	} else if (!strncmp(cmd, "score_win_align", 15)) {
		config.score_win_align = atoi(cmd+16);
	} else if (!strncmp(cmd, "score_win_capt", 14)) {
		config.score_win_capt = atoi(cmd+15);
	} else if (!strncmp(cmd, "score_loose", 11)) {
		config.score_loose = atoi(cmd+12);
	} else if (!strncmp(cmd, "score_tie", 9)) {
		config.score_tie = atoi(cmd+10);
	} else if (!strncmp(cmd, "score_per_capt", 14)) {
		config.score_per_capt = atoi(cmd+15);
	} else if (!strncmp(cmd, "new_player_per_gen", 18)) {
		config.new_player_per_gen = atoi(cmd+19);
	} else if (!strncmp(cmd, "max_turn_per_game", 17)) {
		config.max_turn_per_game = atoi(cmd+18);
	} else {
		add_to_history("Error: Unknow variable");
	}
	config.update.refresh_dyn_tab = 1;
}

void	cmd_new_pool(char *opt) {
	int		new_size;
	Player	**new_pool;
	
	if (strlen(opt) > 6) {
		new_size = atoi(opt+5);
	} else {
		new_size = config.dftl_pool_size;
	}
	new_pool = malloc(sizeof(Player*) * new_size);
	for (int i = 0; i < config.pool_size && i < new_size; i++) {
		new_pool[i] = config.pool[i];
	}
	
	if (new_size < config.pool_size) {
		for (int i = new_size; i < config.pool_size; i++) {
			p_delete(config.pool[i]);
		}
	} else {
		for (int i = config.pool_size; i < new_size; i++) {
			new_pool[i] = p_new(PLAYER_TYPE_NN0);
		}
	}
	config.pool_size = new_size;
	free(config.pool);
	config.pool = new_pool;	
}

void	cmd_new_player(char *opt) {
	int		type;
	
	if (strlen(opt) < 10) {
		add_to_history("Error: Unknow player type");
		return ;
	}
	if (!strncmp(opt+7, "nn0", 3)) {
		type = PLAYER_TYPE_NN0;
	} else if (!strncmp(opt+7, "nn1", 3)) {
		type = PLAYER_TYPE_NN1;
	} else if (!strncmp(opt+7, "human", 5)) {
		type = PLAYER_TYPE_HUMAN;
	} else if (!strncmp(opt+7, "ia", 2)) {
		type = PLAYER_TYPE_IA;
	} else {
		add_to_history("Error: Unknow player type");
		return ;
	}

	config.pool = realloc(config.pool, sizeof(Player*) * (config.pool_size + 1));
	config.pool[config.pool_size] = p_new(type);
	config.pool_size++;
	add_to_history("Creating player: ");
	add_to_last_history(config.pool[config.pool_size-1]->id);
}

void cmd_new_game(char *opt) {
	char	*ptr;
	Player	*p1;
	Player	*p2;
	
	if (strlen(opt) < 8) {
		add_to_history("Error: Need two players");
		return ;
	}
	opt += 5;
	if ((ptr = strchr(opt, ' ')) == NULL) {
		add_to_history("Error: Need two players");
		return ;
	}
	*ptr = '\0';
	p1 = get_player(opt);
	p2 = get_player(ptr+1);
	if (!p1 || !p2) {
		add_to_history("Error: Unknow player");
		return ;
	}
	config.update.dyn_tab = TAB_GAME;
	print_game(p1, p2);
}

void	new_cmd(char *cmd) {
	if (!strncmp(cmd, "pool", 4)) {
		cmd_new_pool(cmd);
	} else if (!strncmp(cmd, "player", 6)) {
		cmd_new_player(cmd);
	} else if (!strncmp(cmd, "game", 4)) {
		cmd_new_game(cmd);
	} else {
		add_to_history("Error: Unknow new command");
	}
	config.update.refresh_player_lst = 1;
}



void	cmd_rm_pool() {
	for (int i = 0; i < config.pool_size; i++) {
		p_delete(config.pool[i]);
	}
	free(config.pool);
	config.pool = NULL;
	config.pool_size = 0;
}

void	cmd_rm_player(char *opt) {
	Player	*todel;
	int		i;
	
	if (strlen(opt) < 8) {
		add_to_history("Error: Need players");
		return ;
	}
	todel = get_player(opt+7);
	if (!todel) {
		add_to_history("Error: Unknow players");
		return ;
	}
	i = -1;
	while (config.pool[++i] != todel) {;}
	p_delete(config.pool[i]);
	while (++i < config.pool_size) {
		config.pool[i-1] = config.pool[i];
	}
	config.pool[i-1] = NULL;
	config.pool_size--;
}



void	rm_cmd(char *cmd) {
	if (!strncmp(cmd, "pool", 4)) {
		cmd_rm_pool();
	} else if (!strncmp(cmd, "player", 6)) {
		cmd_rm_player(cmd);
	} else {
		add_to_history("Error: Unknow rm command");
	}
	config.update.refresh_player_lst = 1;
}

void	check_player_lst() {
	for (int i = 0; i < config.pool_size; i++) {
		if (config.pool[i]->type == PLAYER_TYPE_HUMAN) {
			add_to_history("Human player deleted from pool");
			p_delete(config.pool[i]);
			for (int j = i; j < config.pool_size-1; j++) {
				config.pool[j] = config.pool[j+1];
			}
			config.pool[config.pool_size-1] = NULL;
			config.pool_size--;
		}
	}
}

void	cmd_train_start(char *cmd) {
	if (config.train == 1) {
		add_to_history("Error: training already launch");
		add_to_history("tips: \"train stop\" to stop training");
		return ;
	}
	if (config.pool_size < 2) {
		add_to_history("Error: need more player for training");
		return ;
	}
	check_player_lst();
	config.total_gen = config.dftl_gen_per_train;
	config.actual_gen = 1;
	if (strlen(cmd) > 6) {
		config.total_gen = atoi(cmd+6);
	}
	int ret = pthread_create(&train_thread, NULL, train, (void*)0);
	if (ret) {
		fprintf(stderr, "Error creating train thread\n");
		exit(0);
	}
}

void	cmd_train_stop() {
	config.train = 0;
}

void	train_cmd(char *cmd) {
	if (!strncmp(cmd, "start", 5)) {
		cmd_train_start(cmd);
	} else if (!strncmp(cmd, "stop", 4)) {
		cmd_train_stop();
	} else {
		add_to_history("Error: Unknow train command");
	}	
}

void	cmd_load_player(char *cmd) {
	int		fd;
	char	file[128];
	
	strcpy(file, "data/players/");
	strcat(file, cmd);
	fd = open(file, O_RDONLY);
	if (fd < 1) {
		add_to_history("Unknow file");
		return ;
	}
	config.pool = realloc(config.pool, sizeof(Player*) * (config.pool_size + 1));
	config.pool[config.pool_size] = p_load(fd);
	config.pool_size++;
	close(fd);
}

void	cmd_load_pool() {
	struct dirent	*dir_info;
    DIR				*dir;
	
	dir = opendir("data/players");
	if (dir == NULL) {
		add_to_history("Error: Can't open folder data/players");
		return ;
	}
	while ((dir_info = readdir(dir)) != NULL) {
		if (dir_info->d_type == DT_REG) {
			cmd_load_player(dir_info->d_name);
		}
	}
	closedir(dir);
}

void	load_cmd(char *cmd) {
	if (!strncmp(cmd, "player", 6)) {
		cmd_load_player(cmd+7);
	} else if (!strncmp(cmd, "pool", 4)) {
		cmd_load_pool();
	} else {
		add_to_history("Error: Unknow load command");
	}
	config.update.refresh_player_lst = 1;
}

void	cmd_save_player(char *cmd) {
	int		fd;
	char	file[128];
	Player	*p;
	
	p = get_player(cmd);
	if (!p) {
		add_to_history("Unknow player");
		return ;
	}
	strcpy(file, "data/players/");
	strcat(file, cmd);
	fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666);
	if (fd < 1) {
		add_to_history("Error : Unable to save player");
		return ;
	}
	p_save(p, fd);
	close(fd);
}

void	cmd_save_pool() {
	int		fd;
	char	file[128];
	
	for (int i = 0; i < config.pool_size; i++) {
		strcpy(file, "data/players/");
		strcat(file, config.pool[i]->id);
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0666);
		if (fd < 1) {
			add_to_history("Error : Unable to save player");
			return ;
		}
		p_save(config.pool[i], fd);
		close(fd);
	}
}

void	save_cmd(char *cmd) {
	if (!strncmp(cmd, "player", 6)) {
		cmd_save_player(cmd+7);
	} else if (!strncmp(cmd, "pool", 4)) {
		cmd_save_pool();
	} else {
		add_to_history("Error: Unknow save command");
	}
}

void	cmd_list_player(char *cmd) {
	Player *tmp;
	
	tmp = get_player(cmd);
	if (!tmp) {
		add_to_history("Error: Unknow player ");
		add_to_last_history(cmd);
		return ;
	}
	if (config.p_select) {
		p_delete(config.p_select);
	}
	config.p_select = p_copy(tmp);
	config.update.dyn_tab = TAB_PLAYER;
	config.update.refresh_dyn_tab = 1;
}

void	list_cmd(char *cmd) {
	if (!strncmp(cmd, "players", 7)) {
		config.update.dyn_tab = TAB_PLAYERS;
		config.update.refresh_dyn_tab = 1;
	} else if (!strncmp(cmd, "player", 6)) {
		cmd_list_player(cmd+7);
	} else {
		add_to_history("Error: Unknow list command");
	}
}
































































