#pragma once

typedef struct Key {
	char 	*key;
	char	*value;
} Key;

typedef struct Map {
	void	**arr;
	long 	idx;

	int		(*InMap)	(struct Map *m, const char *q);
	Key		*(*Get)		(struct Map *m, const char *q);
	char 	*(*GetValue)(struct Map *m, const char *q);
	int		(*Append)	(struct Map *m, const char *k, const char *v);
	void	(*Destruct)	(struct Map *m);
} Map;

Map NewMap();
int InMap(Map *m, const char *q);
Key *GetKey(Map *m, const char *q);
char *GetKeyValue(Map *m, const char *q);
int AppendKey(Map *m, const char *k, const char *v);
void DestroyMap(Map *m);
