#ifndef PARSE_H
#define PARSE_H

enum ParseValueType
{
    PARSE_VALUE_TYPE_STRING = 0,
    PARSE_VALUE_TYPE_1 = 1,
    PARSE_VALUE_TYPE_INT = 2,
    PARSE_VALUE_TYPE_FLOAT = 3,
    PARSE_VALUE_TYPE_VEC3 = 4
};

void parseInit(const char * text, u32 length); // 801a6e9c
// getTag 801a6ebc
bool parsePush(const char * tag); // 801a6f40
void parsePop(); // 801a70f4
void parsePopNext(); // 801a710c
bool parseGet1Next(int type, void * ret); // 801a7140
bool parseTagGet1(const char * tag, int type, void * ret); // 801a7488

#endif
