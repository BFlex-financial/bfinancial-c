#include <cjson/cJSON.h>

#define VALUE_TYPE_STRING  1
#define VALUE_TYPE_NUMBER  2
#define VALUE_TYPE_BOOL    3
#define VALUE_TYPE_OBJECT  4
#define VALUE_TYPE_ARRAY   5

#define JSON_GETOBJ(pData, key, type) \
    ({ \
        cJSON *item = cJSON_GetObjectItem(pData, key); \
        void *result = NULL; \
        if (item) { \
            switch (type) { \
                case VALUE_TYPE_STRING: \
                    if (cJSON_IsString(item)) { \
                        result = (void *)item->valuestring; \
                    } \
                    break; \
                case VALUE_TYPE_NUMBER: \
                    if (cJSON_IsNumber(item)) { \
                        result = (void *)&item->valuedouble; \
                    } \
                    break; \
                case VALUE_TYPE_BOOL: \
                    if (cJSON_IsBool(item)) { \
                        result = (void *)&item->valueint; \
                    } \
                    break; \
                case VALUE_TYPE_OBJECT: \
                    if (cJSON_IsObject(item)) { \
                        result = (void *)item; \
                    } \
                    break; \
                case VALUE_TYPE_ARRAY: \
                    if (cJSON_IsArray(item)) { \
                        result = (void *)item; \
                    } \
                    break; \
                default: \
                    result = NULL; \
            } \
        } \
        result; \
    })