
#include "gomoku.h"

float	get_rand_value() {
	return ((float)((rand() % (100 * 2)) - 100) / 100.0);
}

Neuron	*n_new(int _nb_inputs) {
	Neuron *new;
	
	new = malloc(sizeof(Neuron));
	new->Nweight = get_rand_value();
	new->nb_inputs = _nb_inputs;
	new->weights = malloc(sizeof(float) * new->nb_inputs);
	for (int i = 0; i < new->nb_inputs; i++) {
		new->weights[i] = get_rand_value();
	}
	return (new);
}

Neuron	*n_copy(Neuron *old) {
	Neuron *new;
	
	new = malloc(sizeof(Neuron));
	new->Nweight = old->Nweight;
	new->nb_inputs = old->nb_inputs;
	new->weights = malloc(sizeof(float) * new->nb_inputs);
	for (int i = 0; i < new->nb_inputs; i++) {
		new->weights[i] = old->weights[i];
	}
	return (new);
}

Neuron	*n_load(int fd, int _nb_inputs) {
	Neuron *new;
	
	new = malloc(sizeof(Neuron));
	Read(fd, &(new->Nweight), sizeof(new->Nweight));
	new->nb_inputs = _nb_inputs;
	new->weights = malloc(sizeof(float) * new->nb_inputs);
	for (int i = 0; i < new->nb_inputs; i++) {
		Read(fd, &(new->weights[i]), sizeof(float));
	}
	return (new);
}

void	n_save(Neuron *n, int fd) {
	Write(fd, &(n->Nweight), sizeof(n->Nweight));
	for (int i = 0; i < n->nb_inputs; i++) {
		Write(fd, &(n->weights[i]), sizeof(float));
	}
}


void	n_delete(Neuron *n) {
	free(n->weights);
	free(n);
}

void	n_mutate(Neuron *n) {
	for (int i = 0; i < n->nb_inputs; i++) {
		n->weights[i] += (float)((rand() % 2000) - 1000) / 20000.0;
	}
	n->Nweight += (float)((rand() % 2000) - 1000) / 20000.0;
}

float	sigmoid(float a) {
	return (1 / (1 + pow(2.71828, (a * -1))));
}

float	n_get_result(Neuron *a, float *inputs) {
	float	result = 0.0;
	
	for (int i = 0; i < a->nb_inputs; i++) {
		result += inputs[i] * a->weights[i];
	}
	return (sigmoid(result + a->Nweight));
}



void	n_print(Neuron *a) {	
	printf("NWeight: %f syn weight: |", a->Nweight);
	for (int i = 0; i < a->nb_inputs; i++) {
		printf("%f|", a->weights[i]);
	}
	printf("|\n");
}