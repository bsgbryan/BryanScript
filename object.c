#include <stdio.h>
#include <string.h>

#include "vm.h"
#include "table.h"
#include "value.h"
#include "memory.h"
#include "object.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type*) allocateObject(sizeof(type), objectType)

static uint32_t hashString(const char* key, int length) {
    uint32_t hash = 2166136261u;

    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t) key[i];
        hash *= 16777619;
    }

    return hash;
}

static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*) reallocate(NULL, 0, size);

    object->type     = type;
    object->next     = vm.objects;
    object->isMarked = false;

    vm.objects = object;

    #ifdef DEBUG_LOG_GC
        printf("%p allocate %zu for %d\n", (void*) object, size, type);
    #endif

    return object;
}

// TODO See about moving the call to hashString() in here
static ObjString* allocateString(char* chars, int length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);

    string->length = length;
    string->chars = chars;
    string->hash = hash;

    // TODO Find a way to implement the carbage collector
    // that doesn't require this.
    // This is needed to ensure that when/if the intern string
    // table gets resized (which could trigger a GC run) `string`
    // doesn't get skipped over in the marking phase (because the
    // `allocateString` call hasn't finished executing, so `string`
    // won't be referred to by anything, and the garbage collector will
    // think it needs to be removed in the `sweep` phase).
    push(OBJ_VAL(string));

    tableSet(&vm.strings, string, NIL_VAL);

    pop();

    return string;
}

ObjString* copyString(const char* chars, int length) {
    uint32_t hash = hashString(chars, length);

    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);

    if (interned != NULL)
        return interned;

    char* heapChars = ALLOCATE(char, length + 1);

    memcpy(heapChars, chars, length);

    heapChars[length] = '\0';

    return allocateString(heapChars, length, hash);
}

static void printFunction(ObjFunction* function) {
    if (function->name == NULL) {
        printf("<script>");

        return;
    }

    printf("<fn %s>", function->name->chars);
}

void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_NATIVE:
            printf("<native fn>");
            break;
        case OBJ_STRING:
            printf("%s", AS_CSTRING(value));
            break;
        case OBJ_CLOSURE:
            printFunction(AS_CLOSURE(value)->function);
            break;
        case OBJ_FUNCTION:
            printFunction(AS_FUNCTION(value));
            break;
        case OBJ_CLASS:
            printf("%s", AS_CLASS(value)->name->chars);
            break;
        case OBJ_INSTANCE:
            printf("%s instance", AS_INSTANCE(value)->klass->name->chars);
            break;
        case OBJ_UPVALUE: // Never actually used; just here to appease the compiler
            printf("upvalue");
            break;
    }
}

ObjString* takeString(char* chars, int length) {
    uint32_t hash = hashString(chars, length);

    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);

    if (interned != NULL) {
        FREE_ARRAY(char, chars, length + 1);

        return interned;
    }

    return allocateString(chars, length, hash);
}

ObjFunction* newFunction() {
    ObjFunction* function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);

    function->arity        = 0;
    function->name         = NULL;
    function->upvalueCount = 0;

    initChunk(&function->chunk);

    return function;
}

ObjNative* newNative(NativeFn function) {
    ObjNative* native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);

    native->function = function;

    return native;
}

ObjClosure* newClosure(ObjFunction* function) {
    ObjUpvalue** upvalues = ALLOCATE(ObjUpvalue*, function->upvalueCount);

    for (int i = 0; i < function->upvalueCount; i++)
        upvalues[i] = NULL;

    ObjClosure* closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);

    closure->function     = function;
    closure->upvalues     = upvalues;
    closure->upvalueCount = function->upvalueCount;

    return closure;
}

ObjUpvalue* newUpvalue(Value* slot) {
    ObjUpvalue* upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);

    upvalue->location = slot;
    upvalue->next     = NULL;
    upvalue->closed   = NIL_VAL;

    return upvalue;
}

ObjClass* newClass(ObjString* name) {
    ObjClass* klass = ALLOCATE_OBJ(ObjClass, OBJ_CLASS);

    klass->name = name;

    return klass;
}

ObjInstance* newInstance(ObjClass* klass) {
    ObjInstance* instance = ALLOCATE_OBJ(ObjInstance, OBJ_INSTANCE);

    instance->klass = klass;

    initTable(&instance->fields);

    return instance;
}
