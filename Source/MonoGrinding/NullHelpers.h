#pragma once

#define MG_LOG_NULL(CategoryName, Verbosity, NullItemName)  \
    UE_LOG(CategoryName, Verbosity, TEXT("%s is nullptr."), \
           TEXT(#NullItemName));

#define MG_LOG_TEMP_WARN_NULL(NullItemName) \
    MG_LOG_NULL(LogTemp, Warning, NullItemName)

#define MG_LOG_TEMP_WARN_NULL_IF_RETURN(NullItem) \
    if (!NullItem) {                              \
        MG_LOG_TEMP_WARN_NULL(NullItem)           \
        return;                                   \
    }

#define MG_LOG_TEMP_WARN_NULL_IF_RETURN_VALUE(NullItem, ReturnValue) \
    if (!NullItem) {                                                 \
        MG_LOG_TEMP_WARN_NULL(NullItem)                              \
        return ReturnValue;                                          \
    }

#define MG_RETURN_IF(Condition) \
    if (Condition) {            \
        return;                 \
    }

#define MG_RETURN_VALUE_IF(Condition, ReturnValue) \
    if (Condition) {                               \
        return ReturnValue;                        \
    }

#define MG_CONTINUE_IF(Condition) \
    if (Condition) {              \
        continue;                 \
    }

#define MG_DEFAULT_LOG(Message) UE_LOG(CategoryName, Verbosity, TEXT(Message));

#define MG_DEFAULT_LOG_RETURN_IF(Condition, Message) \
    if (Condition) {                                 \
        MG_DEFAULT_LOG(Message)                      \
        return;                                      \
    }
