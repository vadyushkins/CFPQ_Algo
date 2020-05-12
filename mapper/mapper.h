#pragma once

#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "../config/config.h"

typedef struct {
    GrB_Index count;
    char items[][MAX_ITEM_NAME_LEN];
} ItemMapper;

void ItemMapper_Init(ItemMapper* dict);

GrB_Index ItemMapper_GetPlaceIndex(ItemMapper* dict, const char* token);
GrB_Index ItemMapper_Insert(ItemMapper* dict, const char* token);
int ItemMapper_Find(ItemMapper* dict, const char* token);
char* ItemMapper_Map(ItemMapper* dict, GrB_Index mapperIdex);
