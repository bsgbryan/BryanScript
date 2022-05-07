#ifndef clox_value_h

#define clox_value_h

#include <stdbool.h>

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {
    VAL_NIL,
    VAL_OBJ,
    VAL_BOOL,
    VAL_NUMBER,
} ValueType;

typedef struct {
    ValueType type;
    union {
        Obj* obj;
        bool boolean;
        double number;
    } as;
} Value;

#define IS_OBJ(value)    ((value).type == VAL_OBJ   )
#define IS_NIL(value)    ((value).type == VAL_NIL   )
#define IS_BOOL(value)   ((value).type == VAL_BOOL  )
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)

#define AS_OBJ(value)    ((value).as.obj    )
#define AS_BOOL(value)   ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number )

#define NIL_VAL           ((Value) {VAL_NIL,    {.number  = 0           }})
#define OBJ_VAL(value)    ((Value) {VAL_OBJ,    {.obj     = (Obj*) value}})
#define BOOL_VAL(value)   ((Value) {VAL_BOOL,   {.boolean = value       }})
#define NUMBER_VAL(value) ((Value) {VAL_NUMBER, {.number  = value       }})

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void freeValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);

void printValue(Value value);

bool valuesEqual(Value a, Value b);

#endif