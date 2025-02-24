#pragma once

typedef struct Key {
	char 	*key;
	char	*value;
} Key;

typedef struct jKey {
	char 	*structure;
	char 	*key;
	char 	*value;
} jKey;

typedef struct Map {
	void	**arr;
	long 	idx;

	int		(*InMap)	(struct Map *m, const char *q);
	Key		*(*Get)		(struct Map *m, const char *q);
	char 	*(*GetValue)(struct Map *m, const char *q);
	int		(*Append)	(struct Map *m, const char *k, const char *v);
	int 	(*AppendJ)	(struct Map *m, const char *structure, const char *k, const char *v);
	void	(*Destruct)	(struct Map *m);
} Map;

//
//			| - > Create a new instanse of Map
//			| - > Returns a an empty map
//
Map 		NewMap();

//
//			| - > Check if a key exists in the Map
//			| - > Returns > 0 upon success or -1 upon failure
//
int 		InMap(Map *m, const char *q);

//
//			| - > Get a key struct with key information
//			| - > Returns key upon success or NULL upon failure
//
Key 		*GetKey(Map *m, const char *q);

//
//			| - > Get a key's value
//			| - > Returns the value upon success or NULL upon failure
//
char 		*GetKeyValue(Map *m, const char *q);

//
//			| - > Append a key with value to the map
//			| - > Returns 1 upon success and 0 upon failure
//
int 		AppendKey(Map *m, const char *k, const char *v);

//
// 			| - > Destruct the struct
//
void 		DestroyMap(Map *m);


// == [ json.c ] ==

int AppendJSONKey(Map *m, const char *structure, const char *k, const char *v);
Map Decode_OneLine_JSON(const char *data);
Map DecodeJSON(const char *data);