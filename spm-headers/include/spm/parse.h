#pragma once

#include <common.h>

CPP_WRAPPER(spm::parse)

enum ParseValueType
{
/* 0x0 */ PARSE_VALUE_TYPE_STRING,
/* 0x1 */ PARSE_VALUE_TYPE_1,
/* 0x2 */ PARSE_VALUE_TYPE_INT,
/* 0x3 */ PARSE_VALUE_TYPE_FLOAT,
/* 0x4 */ PARSE_VALUE_TYPE_VEC3
};

void parseInit(const char * text, u32 length);
DECOMP_STATIC(const char * parse_getTag(const char * text, s32 startIdx, s32 endIdx))
bool parsePush(const char * tag);
void parsePop();
void parsePopNext();
bool parseGet1Next(s32 type, void * ret);
bool parseTagGet1(const char * tag, s32 type, void * ret);

CPP_WRAPPER_END()
