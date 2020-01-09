
#include "gomoku.h"

void	handle_command(char *cmd) {
	add_to_history(cmd);
	
	if (!strncmp(cmd, "config", 6)) {
		config.update.dyn_tab = TAB_CONFIG;
		config.update.refresh_dyn_tab = 1;
	} else if (!strncmp(cmd, "set", 3)) {
		set_cmd(cmd+4);
	} else if (!strncmp(cmd, "help", 4)) {
		config.update.dyn_tab = TAB_HELP;
		config.update.refresh_dyn_tab = 1;
	} else if (!strncmp(cmd, "new", 3)) {
		new_cmd(cmd+4);
	} else if (!strncmp(cmd, "rm", 2)) {
		rm_cmd(cmd+3);
	} else if (!strncmp(cmd, "train", 5)) {
		train_cmd(cmd+6);
	} else {
		add_to_history("Error: Unknow command");
	}
	
	//config.update.refresh = 1;
	config.update.refresh_console = 1;
}

int main() {
	/*srand(time(NULL));
	init_config();
	Player *p1 = p_new(PLAYER_TYPE_NN0);
	Player *p2 = p_new(PLAYER_TYPE_NN0);
	new_game(p1, p2);
	return (1);*/
	
	srand(time(NULL));
	init_config();
	pthread_t	graph;
	
	int ret = pthread_create(&graph, NULL, graph_thread, (void*)1);
	if (ret) {
		fprintf(stderr, "Error creating graph thread\n");
		exit(0);
	}
	
	char buf[65];
	
	while (1) {
		ret = read(STDIN_FILENO, buf, 65);
		if (ret > 64) {
			while (read(STDIN_FILENO, buf, 65) == 65){;}
			add_to_history("Error: maximum command lenght:64");
			config.update.refresh = 1;
			continue;
		} else if (ret == 0) {
			continue;
		}
		buf[ret-1] = '\0';
		handle_command(buf);
	}
	
	/*if (config.train) {
		add_to_history("stoping the training...");
		config.update.refresh_console = 1;
		config.train = 0;
	}
	pthread_join(graph, NULL);*/
	return (1);
	
	
	
	int *neuron_per_layer = malloc(sizeof(int) * 5);
	for (int i = 0; i < 5; i++) {
		neuron_per_layer[i] = 5;
	}
	Brain *b = b_new(3, 5, neuron_per_layer);
	
	int fd = open("testbrain", O_RDWR | O_CREAT | O_TRUNC, 0666);
	b_save(b, fd);
	close(fd);
	
	fd = open("testbrain", O_RDONLY);
	Brain *b2 = b_load(fd);
	close(fd);
	
	float *inputs = malloc(sizeof(float) * 3);
	inputs[0] = 0.0;
	inputs[1] = 1.0;
	inputs[2] = 0.0;
	float *outputs = b_launch(b, inputs);
	
	
	
	float *inputs2 = malloc(sizeof(float) * 3);
	inputs2[0] = 0.0;
	inputs2[1] = 1.0;
	inputs2[2] = 0.0;
	float *outputs2 = b_launch(b2, inputs2);
	for (int i = 0; i < 5; i++) {
		if (outputs[i] != outputs2[i]) {
			printf("ERROR %f != %f\n", outputs[i], outputs2[i]);
		}
	}
	free(inputs);
	free(outputs);
	free(inputs2);
	free(outputs2);
	
	b_delete(b2);
	b_delete(b);
	
	return (1);
}