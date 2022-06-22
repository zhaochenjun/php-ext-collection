/*
  +----------------------------------------------------------------------+
  | Collection Extension                                                 |
  +----------------------------------------------------------------------+
  | Copyright (c) 2016-2018 The Viest                                    |
  +----------------------------------------------------------------------+
  | http://www.viest.me                                                  |
  +----------------------------------------------------------------------+
  | Author: viest <dev@service.viest.me>                                 |
  +----------------------------------------------------------------------+
*/

#include "include.h"
#include "common.h"

/** {{{ \Vtiful\Kernel\Collection::__construct()
 */
PHP_METHOD(vtiful_collection, __construct)
{

}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::__clone()
 */
PHP_METHOD(vtiful_collection, __clone)
{

}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::init([array $collection])
 */
PHP_METHOD(vtiful_collection, init)
{
    zval *collection = NULL;

    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_ARRAY(collection)
    ZEND_PARSE_PARAMETERS_END();

    NEW_COLLECTION_OBJ(return_value, collection);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::map(closures $callback)
 */
PHP_METHOD(vtiful_collection, map)
{
    zval fcall_result, result;
    zend_fcall_info       fci       = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_FUNC(fci, fci_cache)
    ZEND_PARSE_PARAMETERS_END();

    INIT_FCALL(2, &fcall_result)

    COLLECTION_INIT(&result);

    ZEND_HASH_FOREACH_BUCKET(CURRENT_COLLECTION, Bucket* bucket)
        FCALL_TWO_ARGS(bucket)
        COLLECTION_ADD_INDEX_ZVAL(&result, &fcall_result)
    ZEND_HASH_FOREACH_END();

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::all()
 */
PHP_METHOD(vtiful_collection, all)
{
    GC_ADDREF(CURRENT_COLLECTION);

    ZVAL_ARR(return_value, CURRENT_COLLECTION);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::avg([string $key])
 */
PHP_METHOD(vtiful_collection, avg)
{
    zend_string *column = NULL;

    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_STR(column)
    ZEND_PARSE_PARAMETERS_END();

    double sum = 0;

    if (column == NULL) {
        sum = average_no_column(CURRENT_COLLECTION);
    } else {
        sum = average_column(CURRENT_COLLECTION, column);
    }

    ZVAL_DOUBLE(return_value, (sum/zend_hash_num_elements(CURRENT_COLLECTION)));
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::chunk(int $length)
 */
PHP_METHOD(vtiful_collection, chunk)
{
    zend_long length;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_LONG(length)
    ZEND_PARSE_PARAMETERS_END();

    if (length < 1) {
        zend_throw_exception(vtiful_collection_exception_ce, "Parameter cannot be less than 1", 10);
    }

    NEW_ZVAL_OBJ(return_value);

    collection_chunk(CURRENT_COLLECTION, length, return_value);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::collapse()
 */
PHP_METHOD(vtiful_collection, collapse)
{
    zval result;

    COLLECTION_INIT(&result);

    collection_collapse(CURRENT_COLLECTION, &result);

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::count()
 */
PHP_METHOD(vtiful_collection, count)
{
    ZVAL_LONG(return_value, CURRENT_COLLECTION_COUNT);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::toArray()
 */
PHP_METHOD(vtiful_collection, toArray)
{
    ZVAL_ARR(return_value, CURRENT_COLLECTION);

    GC_ZVAL_ADDREF(return_value);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::combine(array $arrVal)
 */
PHP_METHOD(vtiful_collection, combine)
{
    zval result, *val = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ARRAY(val)
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&result);

    collection_combine(CURRENT_COLLECTION, val, &result);

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::concat(array $arrVal)
 */
PHP_METHOD(vtiful_collection, concat)
{
    zval result, *val = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ARRAY(val)
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&result);

    collection_concat(CURRENT_COLLECTION, val, &result);

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::contains(string $value[, string $value])
 */
PHP_METHOD(vtiful_collection, contains)
{
    zval *key = NULL, *val = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 2)
            Z_PARAM_ZVAL(key)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL(val)
    ZEND_PARSE_PARAMETERS_END();

    if (val == NULL) {
        VAL_IN_COLLECTION(CURRENT_COLLECTION, key, return_value);
    } else {
        KV_IN_COLLECTION(CURRENT_COLLECTION, key, val, return_value);
    }
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::diff(array $array)
 */
PHP_METHOD(vtiful_collection, diff)
{
    zval result, *val = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ARRAY(val)
    ZEND_PARSE_PARAMETERS_END();

    collection_diff(CURRENT_COLLECTION, val, &result);

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::diffAssoc(array $array)
 */
PHP_METHOD(vtiful_collection, diffAssoc)
{
    zval result, *val = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ARRAY(val)
    ZEND_PARSE_PARAMETERS_END();

    collection_diff_assoc(CURRENT_COLLECTION, val, &result);

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::every(closures $callback)
 */
PHP_METHOD(vtiful_collection, every)
{
    zval fcall_result;
    zend_fcall_info       fci       = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_FUNC(fci, fci_cache)
    ZEND_PARSE_PARAMETERS_END();

    INIT_FCALL(2, &fcall_result)

    ZEND_HASH_FOREACH_BUCKET(CURRENT_COLLECTION, Bucket* bucket)
        FCALL_TWO_ARGS(bucket)

        if (Z_TYPE_INFO(fcall_result) == IS_FALSE) {
            ZVAL_FALSE(return_value);
            break;
        }
    ZEND_HASH_FOREACH_END();

    ZVAL_TRUE(return_value);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::except(array $excludedKeys)
 */
PHP_METHOD(vtiful_collection, except)
{
    zval result, *excluded_keys = NULL;

    ZEND_PARSE_PARAMETERS_START(1,1)
        Z_PARAM_ARRAY(excluded_keys)
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&result);

    collection_except(CURRENT_COLLECTION, excluded_keys, &result);

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::filter(closures $callback)
 */
PHP_METHOD(vtiful_collection, filter)
{
    zval fcall_result, result;
    zend_fcall_info       fci       = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_FUNC(fci, fci_cache)
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&result);

    if (ZEND_NUM_ARGS() < 1) {
        ZEND_HASH_FOREACH_BUCKET(CURRENT_COLLECTION, Bucket* bucket)
            ZVAL_COPY(&fcall_result, &bucket->val);
            convert_to_boolean(&fcall_result);
            if (Z_TYPE_INFO(fcall_result) == IS_FALSE) {
                continue;
            }
            add_next_index_zval(&result, &bucket->val);
        ZEND_HASH_FOREACH_END();
    } else {
        INIT_FCALL(2, &fcall_result)
        ZEND_HASH_FOREACH_BUCKET(CURRENT_COLLECTION, Bucket* bucket)
            FCALL_TWO_ARGS(bucket)
            if (Z_TYPE_INFO(fcall_result) == IS_FALSE) {
                continue;
            }
            add_next_index_zval(&result, &bucket->val);
        ZEND_HASH_FOREACH_END();
    }

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::first([closures $callback])
 */
PHP_METHOD(vtiful_collection, first)
{
    zend_fcall_info       fci       = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_FUNC(fci, fci_cache)
    ZEND_PARSE_PARAMETERS_END();

    if (ZEND_NUM_ARGS() < 1) {
        ZVAL_COPY(return_value, &(CURRENT_COLLECTION->arData[0].val));
    } else {
        zval fcall_result;

        INIT_FCALL(2, &fcall_result)

        ZEND_HASH_FOREACH_BUCKET(CURRENT_COLLECTION, Bucket* bucket)
            FCALL_TWO_ARGS(bucket)

            if (Z_TYPE_INFO(fcall_result) == IS_TRUE) {
                ZVAL_COPY(return_value, &(bucket->val));
                break;
            }
        ZEND_HASH_FOREACH_END();
    }
}
/* }}} */


/** {{{ \Vtiful\Kernel\Collection::firstWhere(mixed $column, mixed $val)
 */
PHP_METHOD(vtiful_collection, firstWhere)
{
    zval result, *find_res = NULL, *symbol = NULL;
    zend_string  *column   = NULL, *val    = NULL;

    ZEND_PARSE_PARAMETERS_START(2, 3)
        Z_PARAM_STR(column)
        Z_PARAM_STR(val)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(symbol)
    ZEND_PARSE_PARAMETERS_END();

    if (ZEND_NUM_ARGS() == 2) {
        zval zval_zstr;

        ZEND_HASH_FOREACH_VAL(CURRENT_COLLECTION,  zval *value)
            ZVAL_IS_NOT_ARRAY_SO_CONTINUE(value)

            ARRAY_STR_FIND(value, column, find_res);

            if (find_res != NULL) {
                ZVAL_STR(&zval_zstr, val);
                if (zval_equal(find_res, &zval_zstr)) {
                    ZVAL_COPY(&result, value);
                    break;
                }
            }
        ZEND_HASH_FOREACH_END();

        VC_ZVAL_DTOR(zval_zstr);
    } else if (ZEND_NUM_ARGS() == 3) {
        ZEND_HASH_FOREACH_VAL(CURRENT_COLLECTION, zval *value)
            ZVAL_IS_NOT_ARRAY_SO_CONTINUE(value)

            ARRAY_STR_FIND(value, column, find_res);

            if (find_res != NULL) {
                if (zval_comparison_operations(val, find_res, symbol)) {
                    ZVAL_COPY(&result, value);
                    break;
                }
            }
        ZEND_HASH_FOREACH_END();
    }

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::flatMap(closures $callback)
 */
PHP_METHOD(vtiful_collection, flatMap)
{
    zval fcall_result, result;
    zend_fcall_info       fci       = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_FUNC(fci, fci_cache)
    ZEND_PARSE_PARAMETERS_END();

    INIT_FCALL(1, &fcall_result);

    COLLECTION_INIT_IN_CURRENT(&result);

    ZEND_HASH_FOREACH_BUCKET(CURRENT_COLLECTION, Bucket *bucket)
        FCALL_ONE_ARGS(bucket);
        COLLECTION_UPDATE(&result, bucket, &fcall_result);
        FCALL_DTOR(1);
    ZEND_HASH_FOREACH_END();

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::flatten([int $depth])
 */
PHP_METHOD(vtiful_collection, flatten)
{
    zval result;
    zend_long depth = 1;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(depth)
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&result);

    collection_flatten(CURRENT_COLLECTION, depth, &result);

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::flip()
 */
PHP_METHOD(vtiful_collection, flip)
{
    zval result;

    collection_flip(CURRENT_COLLECTION, &result);

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::forget(string $key)
 */
PHP_METHOD(vtiful_collection, forget)
{
    zend_string *key = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_STR(key)
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_STRING_DELETE(CURRENT_COLLECTION, key);

    RETURN_THIS
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::forPage(int $page, int $number)
 */
PHP_METHOD(vtiful_collection, forPage)
{
    zval result;
    zend_long page, number;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_LONG(page)
            Z_PARAM_LONG(number)
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&result);

    collection_for_page(CURRENT_COLLECTION, page, number, &result);

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::get(mixed $key [, mixed $default])
 */
PHP_METHOD(vtiful_collection, get)
{
    zval *key, *default_val = NULL, *find_result = NULL;
    
    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_ZVAL(key)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL(default_val)
    ZEND_PARSE_PARAMETERS_END();

    if (Z_TYPE_P(key) == IS_LONG) {
        COLLECTION_INDEX_FIND(CURRENT_COLLECTION, Z_LVAL_P(key), find_result);
    }

    if (Z_TYPE_P(key) == IS_STRING) {
        COLLECTION_STR_FIND(CURRENT_COLLECTION, Z_STR_P(key), find_result);
    }

    if (find_result == NULL || Z_TYPE_P(find_result) == IS_NULL) {
        if (ZEND_NUM_ARGS() < 2) {
            ZVAL_NULL(return_value);
        } else {
            ZVAL_COPY(return_value, default_val);
        }
    } else {
        ZVAL_COPY(return_value, find_result);
    }
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::groupBy(mixed $key)
 */
PHP_METHOD(vtiful_collection, groupBy)
{
    zval result;

    zend_string *key = NULL;

    zend_fcall_info       fci       = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        _real_arg++;
        if (Z_TYPE_P(_real_arg) == IS_STRING) {
            _real_arg--;
            Z_PARAM_STR(key);
        }
        if (Z_TYPE_P(_real_arg) == IS_OBJECT) {
            _real_arg--;
            Z_PARAM_FUNC(fci, fci_cache);
        }
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&result);

    if (key == NULL) {
        zval fcall_res;

        INIT_FCALL(2, &fcall_res);

        ZEND_HASH_FOREACH_BUCKET(CURRENT_COLLECTION, Bucket *bucket)
            FCALL_TWO_ARGS(bucket);
            collection_group(&(bucket->val), return_value, NULL, &fcall_res, &result);
            VC_ZVAL_DTOR(fcall_res);
        ZEND_HASH_FOREACH_END();
    } else {
        ZEND_HASH_FOREACH_VAL(CURRENT_COLLECTION, zval *value)
            collection_group(value, return_value, key, NULL, &result);
        ZEND_HASH_FOREACH_END();
    }

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::hash(mixed $key)
 */
PHP_METHOD(vtiful_collection, has)
{
    zval *key = NULL, *find_res = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ZVAL(key)
    ZEND_PARSE_PARAMETERS_END();

    if (Z_TYPE_P(key) == IS_STRING) {
        COLLECTION_STR_FIND(CURRENT_COLLECTION, Z_STR_P(key), find_res);
    }

    if (Z_TYPE_P(key) == IS_LONG) {
        COLLECTION_INDEX_FIND(CURRENT_COLLECTION, Z_LVAL_P(key), find_res);
    }

    if (find_res == NULL) {
        ZVAL_FALSE(return_value);
        return;
    }

    ZVAL_TRUE(return_value);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::implode(mixed $key, string $str)
 */
PHP_METHOD(vtiful_collection, implode)
{
    zend_string *key = NULL, *str = NULL, *result = NULL, *z_str_p = NULL;
    size_t element_char_len = 0, elements_len = 0, new_string_len = 0;
    zval tmp_zval_array;

    ZEND_PARSE_PARAMETERS_START(1, 2)
            Z_PARAM_STR(key)
            Z_PARAM_OPTIONAL
            Z_PARAM_STR(str)
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&tmp_zval_array);

    elements_len = zend_hash_num_elements(CURRENT_COLLECTION);

    if (str == NULL) {
        ZEND_HASH_FOREACH_VAL(CURRENT_COLLECTION, zval *val)
                z_str_p = _zval_get_string_func(val);
                element_char_len += ZSTR_LEN(z_str_p);
                add_next_index_str(&tmp_zval_array, z_str_p);
        ZEND_HASH_FOREACH_END();
    } else {
        zval *find_result = NULL;

        ZEND_HASH_FOREACH_VAL(CURRENT_COLLECTION, zval *val)
                if (Z_TYPE_P(val) != IS_ARRAY) {
                    continue;
                }
                COLLECTION_STR_FIND(Z_ARR_P(val), key, find_result);
                z_str_p = _zval_get_string_func(find_result);
                element_char_len += ZSTR_LEN(z_str_p);
                add_next_index_str(&tmp_zval_array, z_str_p);
        ZEND_HASH_FOREACH_END();
    }

    if (ZEND_NUM_ARGS() < 2) {
        new_string_len = element_char_len+(ZSTR_LEN(key)*elements_len)-ZSTR_LEN(key);
        result = zend_string_safe_alloc(elements_len - 1, ZSTR_LEN(key), element_char_len, 0);
        collection_implode(Z_ARR(tmp_zval_array), key, result);
        ZSTR_VAL(result)[new_string_len] = '\0';
    } else {
        new_string_len = element_char_len+(ZSTR_LEN(str)*elements_len)-ZSTR_LEN(str);
        result = zend_string_safe_alloc(elements_len - 1, ZSTR_LEN(str), element_char_len, 0);
        collection_implode(Z_ARR(tmp_zval_array), str, result);
        ZSTR_VAL(result)[new_string_len] = '\0';
    }

    ZVAL_STR(return_value, result);

    VC_ZVAL_DTOR(tmp_zval_array);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::intersect(array $array)
 */
PHP_METHOD(vtiful_collection, intersect)
{
    zval *array = NULL, *find_res = NULL;
    zval total, screening, result;
    size_t current_length = 0;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ARRAY(array)
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT_IN_CURRENT(&total);

    current_length = CURRENT_COLLECTION_COUNT;

    ZEND_HASH_FOREACH_VAL(Z_ARR_P(array), zval *val)
        add_index_zval(&total, ++current_length, val);
        GC_ZVAL_ADDREF(val);
    ZEND_HASH_FOREACH_END();

    COLLECTION_INIT(&screening);
    COLLECTION_INIT(&result);

    ZEND_HASH_FOREACH_VAL(Z_ARR(total), zval *val)
        if (Z_TYPE_P(val) == IS_STRING) {
            zend_string *tmp = Z_STR_P(val);

            find_res = zend_hash_find(Z_ARR(screening), tmp);

            if (find_res == NULL) {
                add_assoc_long_ex(&screening, ZSTR_VAL(tmp), ZSTR_LEN(tmp), 1);
            } else {
                add_next_index_zval(&result, val);
            }
        }

        if (Z_TYPE_P(val) == IS_LONG) {
            find_res = zend_hash_index_find(Z_ARR(screening), Z_LVAL_P(val));

            if (find_res == NULL) {
                add_index_long(&screening, Z_LVAL_P(val), 1);
            } else {
                add_next_index_zval(&result, val);
            }
        }
    ZEND_HASH_FOREACH_END();

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(screening);
    VC_ZVAL_DTOR(result);
    VC_ZVAL_DTOR(total);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::intersectByKeys(array $array)
 */
PHP_METHOD(vtiful_collection, intersectByKeys)
{
    zval *array = NULL, *find_res = NULL;
    zval result;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ARRAY(array)
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&result);

    ZEND_HASH_FOREACH_KEY_VAL(Z_ARR_P(array), zend_ulong long_key, zend_string *str_key, zval *val)
        if (str_key == NULL) {
            find_res = zend_hash_index_find(CURRENT_COLLECTION, long_key);
        } else {
            find_res = zend_hash_find(CURRENT_COLLECTION, str_key);
        }

        if (find_res != NULL) {
            if (str_key == NULL) {
                add_index_zval(&result, long_key, find_res);
            } else {
                add_assoc_zval_ex(&result, ZSTR_VAL(str_key), ZSTR_LEN(str_key), find_res);
            }

            GC_ZVAL_ADDREF(find_res);
        }
    ZEND_HASH_FOREACH_END();

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::isEmpty()
 */
PHP_METHOD(vtiful_collection, isEmpty)
{
    if (CURRENT_COLLECTION_COUNT) {
        RETURN_TRUE;
    }

    RETURN_FALSE;
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::isNotEmpty()
 */
PHP_METHOD(vtiful_collection, isNotEmpty)
{
    if (CURRENT_COLLECTION_COUNT) {
        RETURN_FALSE;
    }

    RETURN_TRUE;
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::keyBy(string $key)
 */
PHP_METHOD(vtiful_collection, keyBy)
{
    zval result;

    zend_string *key = NULL;

    zend_fcall_info       fci       = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            _real_arg++;
            if (Z_TYPE_P(_real_arg) == IS_STRING) {
                _real_arg--;
                Z_PARAM_STR(key);
            }
            if (Z_TYPE_P(_real_arg) == IS_OBJECT) {
                _real_arg--;
                Z_PARAM_FUNC(fci, fci_cache);
            }
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&result);

    if (key) {
        zval *find_res = NULL;

        ZEND_HASH_FOREACH_VAL(CURRENT_COLLECTION, zval *val)
            if (Z_TYPE_P(val) != IS_ARRAY)
                continue;
            COLLECTION_STR_FIND(Z_ARR_P(val), key, find_res);
            if (find_res == NULL)
                continue;
            if (Z_TYPE_P(find_res) == IS_STRING)
                add_assoc_zval_ex(&result, Z_STRVAL_P(find_res), Z_STRLEN_P(find_res), val);
            GC_ZVAL_ADDREF(val);
        ZEND_HASH_FOREACH_END();
    } else {
        zval fcall_res;

        INIT_FCALL(1, &fcall_res);

        ZEND_HASH_FOREACH_BUCKET(CURRENT_COLLECTION, Bucket *bucket)
            if (Z_TYPE(bucket->val) != IS_ARRAY)
                continue;
            FCALL_ONE_ARGS(bucket);
            add_assoc_zval_ex(&result, Z_STRVAL(fcall_res), Z_STRLEN(fcall_res), &bucket->val);
            VC_ZVAL_DTOR(fcall_res);
            GC_ZVAL_ADDREF(&bucket->val);
        ZEND_HASH_FOREACH_END();
    }

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::keys()
 */
PHP_METHOD(vtiful_collection, keys)
{
    zval result;

    COLLECTION_INIT(&result);

    ZEND_HASH_FOREACH_KEY(CURRENT_COLLECTION, zend_ulong long_key, zend_string *str_key)
        if (str_key) {
            add_next_index_str(&result, str_key);
        } else {
            add_next_index_long(&result, long_key);
        }
    ZEND_HASH_FOREACH_END();

    NEW_COLLECTION_OBJ(return_value, &result);

    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::last([callback $callback])
 */
PHP_METHOD(vtiful_collection, last)
{
    zend_fcall_info       fci       = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_FUNC(fci, fci_cache);
    ZEND_PARSE_PARAMETERS_END();

    if (fci.size) {
        zval fcall_res;
        INIT_FCALL(2, &fcall_res);
        ZEND_HASH_FOREACH_BUCKET(CURRENT_COLLECTION, Bucket *bucket)
            FCALL_TWO_ARGS(bucket);
            if (Z_TYPE(fcall_res) == IS_TRUE)
                ZVAL_COPY(return_value, &bucket->val);
        ZEND_HASH_FOREACH_END();
    } else {
        zval *find_res;
        COLLECTION_INDEX_FIND(CURRENT_COLLECTION, CURRENT_COLLECTION_COUNT-1, find_res);
        ZVAL_COPY(return_value, find_res);
    }
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::mapToGroups(callback $callback)
 */
PHP_METHOD(vtiful_collection, mapToGroups)
{
    zval fcall_res, result;
    zval *find_res = NULL, *new_val = NULL;
    zend_string *str_key = NULL;
    zend_ulong  long_key;
    zend_fcall_info       fci       = empty_fcall_info;
    zend_fcall_info_cache fci_cache = empty_fcall_info_cache;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_FUNC(fci, fci_cache);
    ZEND_PARSE_PARAMETERS_END();

    COLLECTION_INIT(&result);
    INIT_FCALL(2, &fcall_res);

    ZEND_HASH_FOREACH_BUCKET(CURRENT_COLLECTION, Bucket *bucket)
        FCALL_TWO_ARGS(bucket);
        if (Z_TYPE(fcall_res) != IS_ARRAY)
            continue;

        zend_hash_get_current_key(Z_ARR(fcall_res), &str_key, &long_key);

        if(str_key) {
            COLLECTION_STR_FIND(Z_ARR(result), str_key, find_res);
            if (find_res == NULL) {
                COLLECTION_TMP(find_res);
            }
            new_val = COLLECTION_INDEX_ZVAL(&fcall_res, 0);
            zend_hash_next_index_insert(Z_ARR_P(find_res), new_val);
            zend_hash_add(Z_ARR(result), str_key, find_res);
            zend_string_release(str_key);
        } else {
            COLLECTION_INDEX_FIND(Z_ARR(result), long_key, find_res);
            if (find_res == NULL) {
                COLLECTION_TMP(find_res);
            }
            new_val = COLLECTION_INDEX_ZVAL(&fcall_res, long_key);
            zend_hash_next_index_insert(Z_ARR_P(find_res), new_val);
            zend_hash_index_add(Z_ARR(result), long_key, find_res);
        }
        GC_ZVAL_ADDREF(new_val);
        VC_ZVAL_DTOR(fcall_res);
    ZEND_HASH_FOREACH_END();

    NEW_COLLECTION_OBJ(return_value, &result);
    VC_ZVAL_DTOR(result);
}
/* }}} */

/** {{{ \Vtiful\Kernel\Collection::max([string $key])
 */
PHP_METHOD(vtiful_collection, max)
{
    zval *result = NULL;
    zend_string *key = NULL;

    ZEND_PARSE_PARAMETERS_START(0, 1)
            Z_PARAM_OPTIONAL
            Z_PARAM_STR(key);
    ZEND_PARSE_PARAMETERS_END();

    if (ZEND_NUM_ARGS() < 1) {
        result = zend_hash_minmax(CURRENT_COLLECTION, collection_compare, 1);
    } else {
        result = zend_hash_key_minmax(CURRENT_COLLECTION, key, collection_compare_by_key, 1);
    }

    ZVAL_COPY(return_value, result);
}
/* }}} */