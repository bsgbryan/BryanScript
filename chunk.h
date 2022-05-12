#ifndef clox_chunk_h

#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_ADD,
    OP_NOT,
    OP_NIL,
    OP_POP,
    OP_CALL,
    OP_TRUE,
    OP_LESS,
    OP_JUMP,
    OP_LOOP,
    OP_CLASS,
    OP_EQUAL,
    OP_FALSE,
    OP_PRINT,
    OP_DIVIDE,
    OP_INVOKE,
    OP_METHOD,
    OP_NEGATE,
    OP_RETURN,
    OP_CLOSURE,
    OP_GREATER,
    OP_INHERIT,
    OP_CONSTANT,
    OP_MULTIPLY,
    OP_SUBSTRACT,
    OP_GET_SUPER,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_SET_GLOBAL,
    OP_GET_GLOBAL,
    OP_GET_UPVALUE,
    OP_SET_UPVALUE,
    OP_GET_PROPERTY,
    OP_SET_PROPERTY,
    OP_SUPER_INVOKE,
    OP_CLOSE_UPVALUE,
    OP_DEFINE_GLOBAL,
    OP_JUMP_IF_FALSE,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);

#endif
