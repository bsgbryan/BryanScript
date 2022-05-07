#ifndef clox_object_h

#define clox_object_h

#include "chunk.h"
#include "value.h"
#include "common.h"

#define OBJ_TYPE(value)    (AS_OBJ(value)->type)

#define IS_STRING(value)   isObjType(value, OBJ_STRING)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)

#define AS_STRING(value)   ((ObjString*) AS_OBJ(value))
#define AS_CSTRING(value)  (((ObjString*) AS_OBJ(value))->chars)
#define AS_FUNCTION(value) ((ObjFunction*) AS_OBJ(value))

typedef enum {
    OBJ_STRING,
    OBJ_FUNCTION,
} ObjType;

struct Obj {
    ObjType type;
    struct Obj* next;
};

struct ObjString {
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

typedef struct {
    Obj obj;
    int arity;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);

void printObject(Value value);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjFunction* newFunction();

#endif