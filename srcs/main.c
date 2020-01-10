
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
	} else if (!strncmp(cmd, "load", 4)) {
		load_cmd(cmd+5);
	} else if (!strncmp(cmd, "save", 4)) {
		save_cmd(cmd+5);
	} else if (!strncmp(cmd, "refresh", 7)) {
		config.update.refresh = 1;
	} else {
		add_to_history("Error: Unknow command");
	}
	
	config.update.refresh_console = 1;
}

int main() {
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
		memset(&buf, 0, sizeof(buf));
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
}