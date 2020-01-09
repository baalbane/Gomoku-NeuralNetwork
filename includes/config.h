#ifndef CONFIG_H_
# define CONFIG_H_


# define	DEFAULT_INPUTS				(NN_MAP_SIZE*NN_MAP_SIZE+2)
# define	DEFAULT_LAYER				3
# define	DEFAULT_N_PER_L				36
# define	NN_MAP_SIZE					6

# define	DEFAULT_USLEEP				5

# define	DEFAULT_POOL_SIZE			20
# define	DEFAULT_PLAYER_TYPE			PLAYER_TYPE_NN0
# define	GAME_PER_PLAYER				2
# define	DEFAULT_GEN_PER_TRAIN		100

# define	DEFAULT_MAP_SIZE			9

# define	DEFAULT_SCORE_WIN_ALIGN		100
# define	DEFAULT_SCORE_WIN_CAPT		60
# define	DEFAULT_SCORE_LOOSE			-100
# define	DEFAULT_SCORE_TIE			-50
# define	DEFAULT_SCORE_PER_CAPT		20

# define	ID_SIZE						6


# define	TYPE_MOVE					int
# define	GET_MOVE(x, y)				(x*100+y)
# define	GET_MOVE_X(x)				(x/100)
# define	GET_MOVE_Y(x)				(x%100)


# define	WIN_WIDTH	160
# define	WIN_HEIGHT	40

enum PLAYER_TYPE {
	PLAYER_TYPE_NN0 = 1,//choose directly next move|38 inputs, 36 ouptuts
	PLAYER_TYPE_NN1,//tree search|38 inputs, 1 outputs
	PLAYER_TYPE_HUMAN,
	PLAYER_TYPE_IA
};

enum MENU_LIST {
	MENU_NONE = 0,
	MENU_1,
	MENU_2
};

enum DYN_TAB_TYPE {
	TAB_ACCEUIL = 0,
	TAB_CONFIG,
	TAB_GAME,
	TAB_TRAIN,
	TAB_HELP
};


# define	RESET_CONSOLE()		printf("\e[1;1H\e[2J")
# define	SET_CURSOR(x,y)		printf("\033[%d;%dH", x, y)	










#endif