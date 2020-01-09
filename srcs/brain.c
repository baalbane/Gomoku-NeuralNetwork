
#include "gomoku.h"


Brain	*b_new(int _nb_inputs, int _nb_layer, int *_neuron_per_layer) {
	Brain	*new;
	int		prev_size;
	
	new = malloc(sizeof(Brain));
	new->nb_inputs = _nb_inputs;
	new->nb_layer = _nb_layer;
	
	new->neuron_per_layer = malloc(sizeof(int) * new->nb_layer);
	new->brain = malloc(sizeof(Neuron**) * new->nb_layer);
	prev_size = new->nb_inputs;
	for (int i = 0; i < new->nb_layer; i++) {
		new->neuron_per_layer[i] = _neuron_per_layer[i];
		new->brain[i] = malloc(sizeof(Neuron*) * new->neuron_per_layer[i]);
		for (int j = 0; j < new->neuron_per_layer[i]; j++) {
			new->brain[i][j] = n_new(prev_size);
		}
		prev_size = new->neuron_per_layer[i];
	}
	return (new);
}

Brain	*b_copy(Brain *old) {
	Brain	*new;
	
	if (!old) {
		return (NULL);
	}
	new = malloc(sizeof(Brain));
	new->nb_inputs = old->nb_inputs;
	new->nb_layer = old->nb_layer;
	
	new->neuron_per_layer = malloc(sizeof(int) * new->nb_layer);
	new->brain = malloc(sizeof(Neuron**) * new->nb_layer);
	for (int i = 0; i < new->nb_layer; i++) {
		new->neuron_per_layer[i] = old->neuron_per_layer[i];
		new->brain[i] = malloc(sizeof(Neuron*) * new->neuron_per_layer[i]);
		for (int j = 0; j < new->neuron_per_layer[i]; j++) {
			new->brain[i][j] = n_copy(old->brain[i][j]);
		}
	}
	return (new);
}

Brain	*b_load(int fd) {
	Brain	*new;
	int		prev_size;
	
	new = malloc(sizeof(Brain));
	Read(fd, &(new->nb_inputs), sizeof(new->nb_inputs));
	Read(fd, &(new->nb_layer), sizeof(new->nb_layer));
	new->neuron_per_layer = malloc(sizeof(int) * new->nb_layer);
	for (int i = 0; i < new->nb_layer; i++) {
		Read(fd, &(new->neuron_per_layer[i]), sizeof(int));
	}
	new->brain = malloc(sizeof(Neuron**) * new->nb_layer);
	prev_size = new->nb_inputs;
	for (int i = 0; i < new->nb_layer; i++) {
		new->brain[i] = malloc(sizeof(Neuron*) * new->neuron_per_layer[i]);
		for (int j = 0; j < new->neuron_per_layer[i]; j++) {
			new->brain[i][j] = n_load(fd, prev_size);
		}
		prev_size = new->neuron_per_layer[i];
	}
	return (new);
}

Brain	*b_save(Brain *b, int fd) {
	Write(fd, &(b->nb_inputs), sizeof(b->nb_inputs));
	Write(fd, &(b->nb_layer), sizeof(b->nb_layer));
	for (int i = 0; i < b->nb_layer; i++) {
		Write(fd, &(b->neuron_per_layer[i]), sizeof(int));
	}
	for (int i = 0; i < b->nb_layer; i++) {
		for (int j = 0; j < b->neuron_per_layer[i]; j++) {
			n_save(b->brain[i][j], fd);
		}
	}
	
}

void	b_delete(Brain *b) {
	if (!b) {
		return ;
	}
	for (int i = 0; i < b->nb_layer; i++) {
		for (int j = 0; j < b->neuron_per_layer[i]; j++) {
			n_delete(b->brain[i][j]);
		}
		free(b->brain[i]);
	}
	free(b->brain);
	free(b->neuron_per_layer);
	free(b);
}

void	b_mutate(Brain *b) {
	for (int i = 0; i < b->nb_layer; i++) {
		for (int j = 0; j < b->neuron_per_layer[i]; j++) {
			n_mutate(b->brain[i][j]);
		}
	}
}

float	*b_launch(Brain *b, float *inputs) {
	float	*outputs;

	for (int i = 0; i < b->nb_layer; i++) {
		outputs = malloc(sizeof(float) * b->neuron_per_layer[i]);
		for (int j = 0; j < b->neuron_per_layer[i]; j++) {
			outputs[j] = n_get_result(b->brain[i][j], inputs);
		}
		if (i > 0) {
			free(inputs);
		}
		inputs = outputs;
	}
	return (inputs);
}






















