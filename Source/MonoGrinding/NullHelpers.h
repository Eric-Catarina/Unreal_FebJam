#define MG_LOG_NULL(CategoryName, Verbosity, NullItemName) \
    UE_LOG(CategoryName, Verbosity, TEXT("%s is nullptr."), TEXT(#NullItemName));

#define MG_LOG_TEMP_WARN_NULL(NullItemName) MG_LOG_NULL(LogTemp, Warning, NullItemName)

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
