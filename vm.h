#ifndef clox_vm_h

#define clox_vm_h

#include "chunk.h"
#include "table.h"
#include "value.h"
#include "object.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

typedef struct {
    // NOTE Keeping this around for when I come back and
    // update the VM
    // ObjFunction* function;
    ObjClosure* closure;
    uint8_t* ip;
    Value* slots;
} CallFrame;

typedef struct {
    CallFrame frames[FRAMES_MAX];
    int frameCount;

    Value stack[STACK_MAX];
    Value* stackTop;
    Table strings;
    Table globals;
    Obj* objects;
    ObjUpvalue* openUpvalues;

    int greyCount;
    int greyCapacity;
    Obj** greyStack;
    size_t bytesAllocated;
    size_t nextGC;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();

InterpretResult interpret(const char* source);

void push(Value value);
Value pop();

#endif