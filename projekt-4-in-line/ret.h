#ifndef RET_H
#define RET_H

#include <assert.h>

#define RET_SUCCESS 0
#define RET_INTERNAL_ERROR 1

#define CALL(F) \
{ \
    ret = (F); \
    if (ret != 0) goto CLEANUP; \
}

#define ERROR(CODE) \
{ \
    ret = (CODE); \
    goto CLEANUP; \
}

#define CHECK(CONDITION, ERROR_CODE_FOR_FAILURE) \
{ \
    if (!(CONDITION)) ERROR(ERROR_CODE_FOR_FAILURE); \
}

#define CHECK_ASSERT(CONDITION, ERROR_CODE_FOR_FAILURE) \
{ \
    assert(CONDITION); \
    CHECK(CONDITION, ERROR_CODE_FOR_FAILURE); \
}

#endif // RET_H
