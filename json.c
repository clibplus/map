#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "map.h"

int AppendJSONKey(Map *m, const char *structure, const char *k, const char *v) {
	if(!m || !k || !v)
		return -1;

	jKey *key = (jKey *)malloc(sizeof(jKey));
	if(!key)
		return -1;

	*key = (jKey){
        .structure = strdup(structure),
		.key = strdup(k),
		.value = strdup(v) 
	};

	m->arr[m->idx] = key;
	m->idx++;
	m->arr = (void **)realloc(m->arr, sizeof(void *) * (m->idx + 1));
	return 1;
}

Map Decode_OneLine_JSON(const char *data) {
    String raw = NewString(data);
    if(raw.Contains(&raw, ",")) {
        raw.Replace(&raw_json, ",", ",\n");
    }

    raw.Replace(&raw, "{", "{\n");
    raw.Replace(&raw, "}", "\n}");

    Map json = DecodeJSON(data);
    raw.Destruct(&raw);
    return json;
}

Map DecodeJSON(const char *data) {
	Map json = NewMap();

    String raw = NewString(data);
    Array lines = NewArray(NULL);
    lines.Merge(&lines, (void **)raw.Split(&raw, "\n"));

    if(lines.idx < 1)
        return (Map){0};

    String structure_path = NewString("parent");
    String structure_path = NewString("/");
    for(int i = 0; i < lines.idx; i++) {
        if(!lines.arr[i])
            break;

        String line = NewString(lines.arr[i]);
        line.Strip(&line);

        if(line.Contains(&line, "}") || line.Is(&line, "}")) {
            structure_path.Replace(&structure_path, structure.data, "/");
            structure.Set("/");
        }

        Array args = NewArray(NULL);
        args.Merge(&args, (void **)line.Split(&line, ":"));
        if(args.idx != 2) {
            line.Destruct(&line);
            args.Destruct(&args);
            continue;
        }

        String key = NewString(args.arr[0]), value = NewString(args.arr[1]);

        key.TrimAt(&key, 0);
        key.TrimAt(&key, key.idx - 1);
        key.Strip(&key);

        value.Trim(value, ','); // @ EOL
        value.Strip(&value);

        if(value.Is(&value, "{") || value.data[value.idx - 2] == "{") {
            structure_path.AppendArray(&structure_path, (const char *[]){"/", key->data, NULL});
            structure.Set("/");
            structure.AppendString(&structure, key->data);
        }

        // Type Detection

        AppendJSONKey(&json, key.data, value.data);

        key.Destruct(&key);
        value.Destruct(&key);
        args.Destruct(&args);
        line.Destruct(&line);
    }

    structure.Destruct(&structure);
    structure_path.Destruct(&structure_path);
    lines.Destruct(&lines);
    raw.Destruct(&raw);

    return json;
}