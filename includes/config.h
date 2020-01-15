#ifndef CONFIG_H_
# define CONFIG_H_

# define	DEFAULT_SPEC_TYPE			NN_TYPE_0
# define	DEFAULT_NEW_N_PER_L			36

# define	DEFAULT_INPUTS_NN0			(NN_MAP_SIZE*NN_MAP_SIZE+2)
# define	DEFAULT_LAYER_NN0			4
static int DEFAULT_N_PER_L_NN0[DEFAULT_LAYER_NN0] = {36, 36, 36, 36};

# define	DEFAULT_INPUTS_NN1			(NN_MAP_SIZE*NN_MAP_SIZE+2)
# define	DEFAULT_LAYER_NN1			4
static int	DEFAULT_N_PER_L_NN1[DEFAULT_LAYER_NN1] = {36, 24, 12, 1};


# define	NN_MAP_SIZE					6

# define	DEFAULT_USLEEP				5

# define	DEFAULT_POOL_SIZE			40
# define	DEFAULT_PLAYER_TYPE			PLAYER_TYPE_NN0
# define	GAME_PER_PLAYER				2
# define	DEFAULT_GEN_PER_TRAIN		100
# define	DEFAULT_MAX_FAMILY_NUMBER	10

# define	DEFAULT_MAP_SIZE			9

# define	DEFAULT_SCORE_WIN_ALIGN		100
# define	DEFAULT_SCORE_WIN_CAPT		97
# define	DEFAULT_SCORE_LOOSE			-100
# define	DEFAULT_SCORE_TIE			-99
# define	DEFAULT_SCORE_PER_CAPT		1
# define	DEFAULT_SCORE_PER_TURN		0
# define	DEFAULT_NEW_P_PER_GEN		2
# define	DEFAULT_MAX_TURN_PER_GAME	9999

# define	ID_SIZE						6


# define	TYPE_MOVE					int
# define	GET_MOVE(x, y)				(x*100+y)
# define	GET_MOVE_X(x)				(x/100)
# define	GET_MOVE_Y(x)				(x%100)


# define	WIN_WIDTH	160
# define	WIN_HEIGHT	40

enum PLAYER_TYPE {
	PLAYER_TYPE_HUMAN = 1,
	PLAYER_TYPE_IA0,
	PLAYER_TYPE_IA1,
	PLAYER_TYPE_IA2,
	PLAYER_TYPE_NN //should always be last in list
};

enum NN_TYPE {
	NN_TYPE_0 = 0,//choose directly next move|38 inputs, 36 ouptuts
	NN_TYPE_1,//choose directly next move|38 inputs, 1 outputs
	NB_NN_TYPE,
	NN_TYPE_2//tree search|38 inputs, 1 outputs
};

enum MENU_LIST {
	MENU_NONE = 0,
	MENU_1,
	MENU_2
};

enum DYN_TAB_TYPE {
	TAB_ACCEUIL = 0,
	TAB_CONFIG,
	TAB_CONFIG_NN,
	TAB_GAME,
	TAB_TRAIN,
	TAB_PLAYERS,
	TAB_PLAYER,
	TAB_HELP
};


# define	RESET_CONSOLE()		printf("\e[1;1H\e[2J")
# define	SET_CURSOR(x,y)		printf("\033[%d;%dH", x, y)	










#endif