#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "map.h"
#include <str.h>
#include <arr.h>

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
    raw.TrimAt(&raw, 0);
    raw.TrimAt(&raw, 0);
    raw.TrimAt(&raw, raw.idx - 1);

    Array fields = NewArray(NULL);
    fields.Merge(&fields, (void **)raw.Split(&raw, ","));

    Map json = NewMap();

    for(int i = 0; i < fields.idx; i++) {
        if(!fields.arr[i])
            break;

        String field = NewString(fields.arr[i]);
        int pos = -1;
        while((pos = field.FindChar(&field, '"')) != -1) 
            field.Trim(&field, '"');

        Array args = NewArray(NULL);
        args.Merge(&args, (void **)field.Split(&field, ":"));

        if(args.idx < 2) {
            args.Destruct(&args);
            field.Destruct(&field);
            continue;
        }

        if(args.idx > 2) {
            String new_value = NewString(field.data);
            int len = strlen(args.arr[0]);

            for(int q = 0; q < len; q++)
                new_value.TrimAt(&new_value, 0);

            AppendJSONKey(&json, "/", args.arr[0], new_value.data);
            args.Destruct(&args);
            field.Destruct(&field);
            new_value.Destruct(&new_value);
            continue;
        }

        AppendJSONKey(&json, "/", args.arr[0], args.arr[1]);
        args.Destruct(&args);
        field.Destruct(&field);
    }

    if(json.idx > 0)
        return json;

    json.Destruct(&json);
    return NewMap();
}

Map DecodeJSON(const char *data) {
	Map json = NewMap();

    String raw = NewString(data);
    Array lines = NewArray(NULL);
    lines.Merge(&lines, (void **)raw.Split(&raw, "\n"));

    if(lines.idx < 1)
        return (Map){0};

    String structure_path = NewString("parent");
    String structure = NewString("/");
    for(int i = 0; i < lines.idx; i++) {
        if(!lines.arr[i])
            break;

        String line = NewString(lines.arr[i]);
        line.Strip(&line);

        if(line.Contains(&line, "}") || line.Is(&line, "}")) {
            structure_path.Replace(&structure_path, structure.data, "/");
            structure.Clear(&structure);
            structure.AppendString(&structure, "/");
        }

        Array args = NewArray(NULL);
        args.Merge(&args, (void **)line.Split(&line, ":"));
        printf("%ld\n", args.idx);
        if(args.idx != 2) {
            line.Destruct(&line);
            args.Destruct(&args);
            continue;
        }

        String key = NewString(args.arr[0]), value = NewString(args.arr[1]);

        key.TrimAt(&key, 0);
        key.TrimAt(&key, key.idx - 1);
        key.Strip(&key);

        value.Trim(&value, ','); // @ EOL
        value.Strip(&value);

        if(value.Is(&value, "{") || value.data[value.idx - 2] == '{') {
            structure_path.AppendArray(&structure_path, (const char *[]){"/", key.data, NULL});
            structure.Clear(&structure);
            structure.AppendString(&structure, "/");
            structure.AppendString(&structure, key.data);
        }

        // Type Detection

        AppendJSONKey(&json, structure_path.data, key.data, value.data);

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