#ifndef dictu_object_h
#define dictu_object_h

#include <stdint.h>
#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "table.h"
#include "value.h"

#define OBJ_TYPE(value)         (AS_OBJ(value)->type)

#define IS_BOUND_METHOD(value)  isObjType(value, OBJ_BOUND_METHOD)
#define IS_CLASS(value)         isObjType(value, OBJ_CLASS)
#define IS_NATIVE_CLASS(value)  isObjType(value, OBJ_NATIVE_CLASS)
#define IS_TRAIT(value)         isObjType(value, OBJ_TRAIT)
#define IS_CLOSURE(value)       isObjType(value, OBJ_CLOSURE)
#define IS_FUNCTION(value)      isObjType(value, OBJ_FUNCTION)
#define IS_INSTANCE(value)      isObjType(value, OBJ_INSTANCE)
#define IS_NATIVE(value)        isObjType(value, OBJ_NATIVE)
#define IS_STRING(value)        isObjType(value, OBJ_STRING)
#define IS_LIST(value)          isObjType(value, OBJ_LIST)
#define IS_DICT(value)          isObjType(value, OBJ_DICT)
#define IS_SET(value)           isObjType(value, OBJ_SET)
#define IS_FILE(value)          isObjType(value, OBJ_FILE)

#define AS_BOUND_METHOD(value)  ((ObjBoundMethod*)AS_OBJ(value))
#define AS_CLASS(value)         ((ObjClass*)AS_OBJ(value))
#define AS_CLASS_NATIVE(value)  ((ObjClassNative*)AS_OBJ(value))
#define AS_TRAIT(value)         ((ObjTrait*)AS_OBJ(value))
#define AS_CLOSURE(value)       ((ObjClosure*)AS_OBJ(value))
#define AS_FUNCTION(value)      ((ObjFunction*)AS_OBJ(value))
#define AS_INSTANCE(value)      ((ObjInstance*)AS_OBJ(value))
#define AS_NATIVE(value)        (((ObjNative*)AS_OBJ(value))->function)
#define AS_STRING(value)        ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)       (((ObjString*)AS_OBJ(value))->chars)
#define AS_LIST(value)          ((ObjList*)AS_OBJ(value))
#define AS_DICT(value)          ((ObjDict*)AS_OBJ(value))
#define AS_SET(value)           ((ObjSet*)AS_OBJ(value))
#define AS_FILE(value)          ((ObjFile*)AS_OBJ(value))

typedef enum {
    OBJ_BOUND_METHOD,
    OBJ_CLASS,
    OBJ_NATIVE_CLASS,
    OBJ_TRAIT,
    OBJ_CLOSURE,
    OBJ_FUNCTION,
    OBJ_INSTANCE,
    OBJ_NATIVE,
    OBJ_STRING,
    OBJ_LIST,
    OBJ_DICT,
    OBJ_SET,
    OBJ_FILE,
    OBJ_UPVALUE
} ObjType;

struct sObj {
    ObjType type;
    bool isDark;
    struct sObj *next;
};

typedef struct {
    Obj obj;
    int arity;
    int arityOptional;
    int upvalueCount;
    Chunk chunk;
    ObjString *name;
    bool staticMethod;
} ObjFunction;

typedef Value (*NativeFn)(VM *vm, int argCount, Value *args);

typedef struct {
    Obj obj;
    NativeFn function;
} ObjNative;

struct sObjString {
    Obj obj;
    int length;
    char *chars;
    uint32_t hash;
};

struct sObjList {
    Obj obj;
    ValueArray values;
};

typedef struct {
    Value key;
    Value value;
    uint32_t psl;
} DictItem;

struct sObjDict {
    Obj obj;
    int count;
    int capacityMask;
    DictItem *entries;
};

typedef struct {
    Value value;
    bool deleted;
} SetItem;

struct sObjSet {
    Obj obj;
    int count;
    int capacityMask;
    SetItem *entries;
};

struct sObjFile {
    Obj obj;
    FILE *file;
    char *path;
    char *openType;
};

typedef struct sUpvalue {
    Obj obj;

    // Pointer to the variable this upvalue is referencing.
    Value *value;

    // If the upvalue is closed (i.e. the local variable it was pointing
    // to has been popped off the stack) then the closed-over value is
    // hoisted out of the stack into here. [value] is then be changed to
    // point to this.
    Value closed;

    // Open upvalues are stored in a linked list. This points to the next
    // one in that list.
    struct sUpvalue *next;
} ObjUpvalue;

typedef struct {
    Obj obj;
    ObjFunction *function;
    ObjUpvalue **upvalues;
    int upvalueCount;
} ObjClosure;

typedef struct sObjClass {
    Obj obj;
    ObjString *name;
    struct sObjClass *superclass;
    Table methods;
} ObjClass;

typedef struct sObjClassNative {
    Obj obj;
    ObjString *name;
    Table methods;
    Table properties;
} ObjClassNative;

typedef struct sObjTrait {
    Obj obj;
    ObjString *name;
    Table methods;
} ObjTrait;

typedef struct {
    Obj obj;
    ObjClass *klass;
    Table fields;
} ObjInstance;

typedef struct {
    Obj obj;
    Value receiver;
    ObjClosure *method;
} ObjBoundMethod;

ObjBoundMethod *newBoundMethod(VM *vm, Value receiver, ObjClosure *method);

ObjClass *newClass(VM *vm, ObjString *name, ObjClass *superclass);

ObjClassNative *newClassNative(VM *vm, ObjString *name);

ObjTrait *newTrait(VM *vm, ObjString *name);

ObjClosure *newClosure(VM *vm, ObjFunction *function);

ObjFunction *newFunction(VM *vm, bool isStatic);

ObjInstance *newInstance(VM *vm, ObjClass *klass);

ObjNative *newNative(VM *vm, NativeFn function);

ObjString *takeString(VM *vm, char *chars, int length);

ObjString *copyString(VM *vm, const char *chars, int length);

ObjList *initList(VM *vm);

ObjDict *initDict(VM *vm);

ObjSet *initSet(VM *vm);

ObjFile *initFile(VM *vm);

ObjUpvalue *newUpvalue(VM *vm, Value *slot);

char *setToString(Value value);
char *dictToString(Value value);
char *listToString(Value value);
char *classToString(Value value);
char *instanceToString(Value value);
char *objectToString(Value value);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

static inline ObjType getObjType(Value value) {
    return AS_OBJ(value)->type;
}

#endif
