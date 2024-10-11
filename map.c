#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "map.h"

Map NewMap() {
	Map m = { 
		.arr 	= (void **)malloc(sizeof(void *) * 1),
		.idx 	= 0
	};

	return m;
}

int InMap(Map *m, const char *q) {
	if(!m || !q)
		return -1;

	for(int i = 0; i < m->idx; i++) {
		if(!strcmp(((Key *)m->arr[i])->key, q))
			return i;
	}

	return -1;
}

Key *GetKey(Map *m, const char *q) {
	if(!m || !q)
		return NULL;

	for(int i = 0; i < m->idx; i++) {
		if(!m->arr[i])
			break;

		if(!strcmp(((Key *)m->arr[i])->key, q))
			return m->arr[i];
	}

	return NULL;
}

char *GetKeyValue(Map *m, const char *q) {
	if(!m || !q)
		return NULL;

	for(int i = 0; i < m->idx; i++) {
		if(!m->arr[i])
			break;

		if(!strcmp(((Key *)m->arr[i])->key, q))
			return ((Key *)m->arr[i])->value;
	}

	return NULL;
}


int AppendKey(Map *m, const char *k, const char *v) {
	if(!m || !k || !v)
		return -1;

	Key *key = (Key *)malloc(sizeof(Key));
	if(!key)
		return -1;

	*key = (Key){
		.key = strdup(k),
		.value = strdup(v) 
	};

	m->arr[m->idx] = key;
	m->idx++;
	m->arr = (void **)realloc(m->arr, sizeof(void *) * (m->idx + 1));
	return 1;
}
