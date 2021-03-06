#include "path.h"

#ifndef _WIN32
static Value realpathNative(VM *vm, int argCount, Value *args) {
    if (argCount != 1) {
        runtimeError(vm, "realpath() takes 1 argument (%d given)", argCount);
        return EMPTY_VAL;
    }

    if (!IS_STRING(args[0])) {
        runtimeError(vm, "realpath() argument must be a string");
        return EMPTY_VAL;
    }

    char *path = AS_CSTRING(args[0]);

    char tmp[PATH_MAX + 1];
    if (NULL == realpath(path, tmp))
      return NIL_VAL;

    return OBJ_VAL(copyString(vm, tmp, strlen (tmp)));
}
#endif

static Value isAbsoluteNative(VM *vm, int argCount, Value *args) {
    if (argCount != 1) {
        runtimeError(vm, "isAbsolute() takes 1 argument (%d given)", argCount);
        return EMPTY_VAL;
    }

    if (!IS_STRING(args[0])) {
        runtimeError(vm, "isAbsolute() argument must be a string");
        return EMPTY_VAL;
    }

    char *path = AS_CSTRING(args[0]);

    return (IS_DIR_SEPARATOR(path[0]) ? TRUE_VAL : FALSE_VAL);
}

static Value basenameNative(VM *vm, int argCount, Value *args) {
    if (argCount != 1) {
        runtimeError(vm, "basename() takes 1 argument (%d given)", argCount);
        return EMPTY_VAL;
    }

    if (!IS_STRING(args[0])) {
        runtimeError(vm, "basename() argument must be a string");
        return EMPTY_VAL;
    }

    ObjString *PathString = AS_STRING(args[0]);
    char *path = PathString->chars;

    int len = PathString->length;

    if (!len || (len == 1 && *path != DIR_SEPARATOR)) {
        return OBJ_VAL(copyString(vm, "", 0));
    }

    char *p = path + len - 1;
    while (p > path && (*(p - 1) != DIR_SEPARATOR)) --p;

    return OBJ_VAL(copyString(vm, p, (len - (p - path))));
}

static Value extnameNative(VM *vm, int argCount, Value *args) {
    if (argCount != 1) {
        runtimeError(vm, "extname() takes 1 argument (%d given)", argCount);
        return EMPTY_VAL;
    }

    if (!IS_STRING(args[0])) {
        runtimeError(vm, "extname() argument must be a string");
        return EMPTY_VAL;
    }

    ObjString *PathString = AS_STRING(args[0]);
    char *path = PathString->chars;

    int len = PathString->length;

    if (!len) {
        return OBJ_VAL(copyString(vm, path, len));
    }

    char *p = path + len;
    while (p > path && (*(p - 1) != '.')) --p;

    if (p == path) {
        return OBJ_VAL(copyString(vm, "", 0));
    }

    p--;

    return OBJ_VAL(copyString(vm, p, len - (p - path)));
}

static Value dirnameNative(VM *vm, int argCount, Value *args) {
    if (argCount != 1) {
        runtimeError(vm, "dirname() takes 1 argument (%d given)", argCount);
        return EMPTY_VAL;
    }

    if (!IS_STRING(args[0])) {
        runtimeError(vm, "dirname() argument must be a string");
        return EMPTY_VAL;
    }

    ObjString *PathString = AS_STRING(args[0]);
    char *path = PathString->chars;

    int len = PathString->length;

    if (!len) {
        return OBJ_VAL(copyString(vm, ".", 1));
    }

    char *sep = path + len;

    /* trailing slashes */
    while (sep != path) {
        if (0 == IS_DIR_SEPARATOR (*sep))
            break;
        sep--;
    }

    /* first found */
    while (sep != path) {
        if (IS_DIR_SEPARATOR (*sep))
            break;
        sep--;
    }

    /* trim again */
    while (sep != path) {
        if (0 == IS_DIR_SEPARATOR (*sep))
            break;
        sep--;
    }

    if (sep == path && *sep != DIR_SEPARATOR) {
        return OBJ_VAL(copyString(vm, ".", 1));
    }

    len = sep - path + 1;

    return OBJ_VAL(copyString(vm, path, len));
}

void createPathClass(VM *vm) {
    ObjString *name = copyString(vm, "Path", 4);
    push(vm, OBJ_VAL(name));
    ObjClassNative *klass = newClassNative(vm, name);
    push(vm, OBJ_VAL(klass));

    /**
     * Define Path methods
     */
#ifndef _WIN32
    defineNative(vm, &klass->methods, "realpath", realpathNative);
#endif
    defineNative(vm, &klass->methods, "isAbsolute", isAbsoluteNative);
    defineNative(vm, &klass->methods, "basename", basenameNative);
    defineNative(vm, &klass->methods, "extname", extnameNative);
    defineNative(vm, &klass->methods, "dirname", dirnameNative);

    defineNativeProperty(vm, &klass->properties, "delimiter", OBJ_VAL(
        copyString(vm, PATH_DELIMITER_AS_STRING, PATH_DELIMITER_STRLEN)));
    defineNativeProperty(vm, &klass->properties, "dirSeparator", OBJ_VAL(
        copyString(vm, DIR_SEPARATOR_AS_STRING, DIR_SEPARATOR_STRLEN)));

    tableSet(vm, &vm->globals, name, OBJ_VAL(klass));
    pop(vm);
    pop(vm);
}
