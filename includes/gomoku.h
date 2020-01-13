#ifndef MAIN_H_
# define MAIN_H_


# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <time.h>
# include <pthread.h>
# include <string.h>
# include <dirent.h>

# include "config.h"

typedef struct	t_neuron {
	int			nb_inputs;
	float		*weights;
	float		Nweight;
}				Neuron;

typedef struct	t_brain {
	int			type;
	int			nb_inputs;
	int			nb_layer;
	int			*neuron_per_layer;
	Neuron		***brain;
	
}				Brain;


typedef struct	t_player {
	char		id[ID_SIZE+1];
	int			type;
	int			score;
	int			real_score;
	int			nb_fights;
	Brain		*brain;
}				Player;

typedef struct	t_game_player {
	Player		*player;
	char		**map;
	int			capt_me;
	int			capt_adv;
}				Game_player;

typedef struct	t_graph_updt {
	int			refresh;
	int			refresh_console;
	int			refresh_dyn_tab;
	int			refresh_player_lst;
	int			dyn_tab;
}				graph_updt;

typedef struct	t_config_nn {
	int			type;
	int			nb_inputs;
	int			nb_layer;
	int			*neuron_per_layer;
}				Config_nn;

typedef struct	t_config {
	int			nb_nn;
	Config_nn	**nn_spec;
	
	int			dflt_usleep;
	int			dftl_pool_size;
	int			dftl_map_size;
	int			max_turn_per_game;
	
	int			game_per_player;
	int			dftl_gen_per_train;
	int			new_player_per_gen;
	
	int			train;
	int			score_win_align;
	int			score_win_capt;
	int			score_loose;
	int			score_tie;
	int			score_per_capt;
	
	int			total_gen;
	int			actual_gen;
	
	char		**map;
	
	int			pool_size;
	Player		**pool;
	
	int			actual_menu;
	
	char		history[5][65];
	Player		*p_select;
	graph_updt	update;
}				Config;


extern Config config;


//Neuron
Neuron		*n_new(int _nb_inputs);
Neuron		*n_copy(Neuron *old);
Neuron		*n_load(int fd, int _nb_inputs);
void		n_save(Neuron *n, int fd);
void		n_delete(Neuron *n);
void		n_mutate(Neuron *n);
float		n_get_result(Neuron *a, float *inputs);
void		n_print(Neuron *a);


//Brain
Brain		*b_new(int _nb_inputs, int _nb_layer, int *_neuron_per_layer);
Brain		*b_copy(Brain *old);
Brain		*b_load(int fd);
Brain		*b_save(Brain *b, int fd);
void		b_delete(Brain *b);
void		b_mutate(Brain *b);
float		*b_launch(Brain *b, float *inputs);
void		n_print(Neuron *a);


//Player
Player		*p_new(int type);
Player		*p_copy(Player	*old);
Player		*p_load(int fd);
void		p_save(Player *p, int fd);
void		p_mutate(Player *p);
void		p_delete(Player *p);
TYPE_MOVE	choose_next_move(Game_player *p);
Player		*get_player(char *id);


//Mutate
int			mutate();

//Train
void		*train(void	*data);

//GAME
int			new_game(Player *p1, Player *p2);
int			print_game(Player *p1, Player *p2);


//APPLY_MOVE
int			apply_move(Game_player *me, Game_player *adv, TYPE_MOVE move);


//ID
void		get_new_id(char *id);
void		get_next_id(char *id, char *last);
void		get_next_id2(char *id);

//Graphique
void		*graph_thread(void *p_data);

//Config
int			init_config();
void		add_to_history(char *tmp);
void		add_to_last_history(char *tmp);
void		set_cmd(char *cmd);
void		new_cmd(char *cmd);
void		rm_cmd(char *cmd);
void		train_cmd(char *cmd);
void		load_cmd(char *cmd);
void		save_cmd(char *cmd);
void		list_cmd(char *cmd);

//Tools
void		Write(int fd, void *value, size_t size);
void		Read(int fd, void *buf, size_t size);




















#endif