
#include "gomoku.h"

static char next_id = 'a';

void	switch_next_id() {
	if (next_id == 'z') {
		next_id = 'a';
	} else {
		next_id++;
	}
}

void	get_new_id(char *id) {
	memset(id, 0, sizeof(char)*(ID_SIZE+1));
	*id = next_id + 'A' - 'a';
	switch_next_id();
	add_to_history("Creating player: ");
	add_to_last_history(id);
}


void	get_next_id(char *id, char *last) {
	int		i;
	
	memcpy(id, last, sizeof(char)*(ID_SIZE+1));
	i = 0;
	while (id[++i]) {;}
	if (i == ID_SIZE) {
		for (i = 1; i < ID_SIZE-1; i++) {
			id[i] = id[i+1];
		}
	}
	id[i] = next_id;
	switch_next_id();
	add_to_history("Creating player: ");
	add_to_last_history(id);
}