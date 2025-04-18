#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "map.h"

Map NewMap() {
	Map m = { 
		.arr 		= (void **)malloc(sizeof(void *) * 1),
		.idx 		= 0,

		.InMap		= InMap,
		.Get		= GetKey,
		.GetValue	= GetKeyValue,
		.Append		= AppendKey,
		.AppendJ 	= AppendJSONKey,
		.Merge		= Map__Merge,
		.Destruct	= DestroyMap
	};

	return m;
}

int InMap(Map *m, const char *q) {
	if(!m || !q)
		return -1;

	for(int i = 0; i < m->idx; i++) {
		if(!strcmp(((Key *)m->arr[i])->key, q))
			return i;

        if(strstr(((Key *)m->arr[i])->key, q))
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

		if(!strcmp(((Key *)m->arr[i])->key, q) || strstr(((Key *)m->arr[i])->key, q))
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
	m->arr[m->idx] = NULL;
	
	return 1;
}

int Map__Merge(Map *m, Map *newm) {
	if(!m || !newm) 
		return 0;

	for(int i = 0; i < newm->idx; i++) {
		if(!newm->arr[i])
			break;

		AppendKey(m, ((Key *)newm->arr[i])->key, ((Key *)newm->arr[i])->value);
	}

	return 1;
}

void DestroyMap(Map *m) {
	if(!m)
		return;

	if(m->arr) {
		for(int i = 0; i < m->idx; i++) {
			if(!m->arr[i])
				break;
			
			(void)(!((Key *)m->arr[i])->key ? 0 : free(((Key *)m->arr[i])->key));
			(void)(!((Key *)m->arr[i])->value ? 0 : free(((Key *)m->arr[i])->value));
			free(m->arr[i]);
		}

		free(m->arr);
	}
}
