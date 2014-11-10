/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe                                                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "Zend/zend_exceptions.h"
#include "php_strict.h"

ZEND_DECLARE_MODULE_GLOBALS(strict)

zend_class_entry *ce_Autobox;
zend_class_entry *ce_Integer;
zend_class_entry *ce_Double;
zend_class_entry *ce_Boolean;
zend_class_entry *ce_String;
zend_class_entry *ce_TypeException;

/* {{{ php_strict_init_globals
 */
static void php_strict_init_globals(zend_strict_globals *strict_globals){}
/* }}} */

typedef struct _php_strict_autobox_t {
    zend_object std;
    zend_uchar  type;
    zval        value;
} php_strict_autobox_t;

zend_object_handlers php_strict_autobox_handlers;

static inline int php_strict_handler_recv(ZEND_OPCODE_HANDLER_ARGS) {
    const zend_function *function = EX(func);
    
    if (function->common.fn_flags & ZEND_ACC_HAS_TYPE_HINTS) {
        const zend_op       *opline   = EX(opline);
        uint32_t             arg      = opline->op1.num;
        zval                *param    = EX_VAR(opline->result.var);
        zend_arg_info       *info     = &function->common.arg_info[arg-1];
        
        if (info->type_hint == IS_OBJECT && Z_TYPE_P(param) != IS_OBJECT) {
            zend_string      *cname = 
                zend_string_init(info->class_name, info->class_name_len, 0);
            zend_class_entry *wants = zend_lookup_class(cname TSRMLS_CC);
            
            if (wants && instanceof_function(wants, ce_Autobox TSRMLS_CC)) {
                zval local;
                zend_function *constructor;
                zend_fcall_info fci;
                zend_fcall_info_cache fcc;
                zval retval;
                
                object_init_ex(&local, wants);
                
                constructor = Z_OBJ_HT(local)
                    ->get_constructor(Z_OBJ(local) TSRMLS_CC);
                
                memset(&fci, 0, sizeof(zend_fcall_info));
                memset(&fcc, 0, sizeof(zend_fcall_info_cache));
                
                fci.size = sizeof(zend_fcall_info);
                fci.object = Z_OBJ(local);
                fci.retval = &retval;
                
                zend_fcall_info_argn(&fci TSRMLS_CC, 1, param);
                
                fcc.initialized = 1;
                fcc.function_handler = constructor;
                fcc.object = Z_OBJ(local);
                
                if (zend_call_function(&fci, &fcc TSRMLS_CC) == SUCCESS) {
                    zend_fcall_info_args_clear(&fci, 1);
                    zval_dtor(&retval);
                    ZVAL_ZVAL(param, &local, 0, 1);
                } else zval_dtor(&local);
            }
            
            zend_string_release(cname);
        }
    }
    
    if (EG(exception)) {
        return ZEND_USER_OPCODE_CONTINUE;
    }
    
    return ZEND_USER_OPCODE_DISPATCH;
}

#define php_strict_autobox_alloc()  (php_strict_autobox_t*) emalloc(sizeof(php_strict_autobox_t))
#define php_strict_autobox_fetch(t) ((php_strict_autobox_t*) Z_OBJ_P(t))
#define php_strict_autobox_this()   php_strict_autobox_fetch(getThis())

static inline int php_strict_autobox_cast(zval *read, zval *write, int type TSRMLS_DC) {
    php_strict_autobox_t *autobox = php_strict_autobox_fetch(read);
    
    switch (type) {
        case IS_STRING:
            if (instanceof_function(Z_OBJCE_P(read), ce_Autobox TSRMLS_CC)) {
                *write = autobox->value;
                zval_copy_ctor(write);
                
                return SUCCESS;
            }
        break;
        
        case IS_LONG:
        case IS_DOUBLE:
        case _IS_BOOL:
            if (instanceof_function(Z_OBJCE_P(read), ce_Autobox TSRMLS_CC)) {
                *write = autobox->value;
                return SUCCESS;
            }
        break;
    }
 
    return FAILURE;
}

static inline HashTable* php_strict_autobox_debug(zval *object, int *temp TSRMLS_DC) {
    HashTable *table = NULL;
    php_strict_autobox_t *autobox = php_strict_autobox_fetch(object);
    
    if (Z_TYPE(autobox->value)) {
        ALLOC_HASHTABLE(table);
        zend_hash_init(table, 8, NULL, ZVAL_PTR_DTOR, 0);  
        
        zend_hash_str_update(
            table, "value", sizeof("value")-1, &autobox->value);

        *temp = 1; 
    } 

    return table;
}

static inline void php_strict_autobox_dtor(zend_object *object TSRMLS_DC) {
    php_strict_autobox_t *autobox = (php_strict_autobox_t*) object;

    zval_dtor(&autobox->value);
    zend_objects_destroy_object(object TSRMLS_CC);
}

static inline zend_object* php_strict_autobox_new(zend_class_entry *ce TSRMLS_DC) {
    php_strict_autobox_t *autobox = php_strict_autobox_alloc();
    
    zend_object_std_init
        (&autobox->std, ce TSRMLS_CC);
 
    autobox->std.handlers = &php_strict_autobox_handlers;

    return (zend_object*) autobox;
}

static inline void php_strict_autobox_ctor(php_strict_autobox_t *autobox, zend_uchar type, zval *value TSRMLS_DC) {
    
    switch (type) {
        case _IS_BOOL:
            if (Z_TYPE_P(value) != IS_TRUE && Z_TYPE_P(value) != IS_FALSE) {
                zend_throw_exception_ex(ce_TypeException, _IS_BOOL TSRMLS_CC,
                    "expected boolean and received %s", zend_get_type_by_const(Z_TYPE_P(value)));
                return;
            }
                
        break;
        
        case IS_STRING:
            if (Z_TYPE_P(value) != IS_STRING) {
                 zend_throw_exception_ex(ce_TypeException, IS_STRING TSRMLS_CC,
                    "expected string and received %s", zend_get_type_by_const(Z_TYPE_P(value)));
                 return;
            }
        break;
        
        case IS_LONG:
            if (Z_TYPE_P(value) != IS_LONG) {
                 zend_throw_exception_ex(ce_TypeException, IS_LONG TSRMLS_CC,
                    "expected integer and received %s", zend_get_type_by_const(Z_TYPE_P(value)));
                 return;
            }
        break;
        
        case IS_DOUBLE:
            if (Z_TYPE_P(value) != IS_DOUBLE) {
                 zend_throw_exception_ex(ce_TypeException, IS_DOUBLE TSRMLS_CC,
                    "expected double and received %s", zend_get_type_by_const(Z_TYPE_P(value)));
                 return;
            }
        break;
    }
    
    //if (Z_TYPE(autobox->value)) {
       // zval_dtor(&autobox->value);
    //}
    
    autobox->value = *value;
    autobox->type  = type;

    zval_copy_ctor(&autobox->value);
}

#define AUTOBOX_CONSTRUCTOR(c, t) PHP_METHOD(c, __construct) { \
    php_strict_autobox_t *autobox = php_strict_autobox_this(); \
    zval *value; \
    \
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &value) != SUCCESS) { \
        return; \
    } \
    \
    php_strict_autobox_ctor(autobox, t, value TSRMLS_CC); \
}

AUTOBOX_CONSTRUCTOR(Integer, IS_LONG)
AUTOBOX_CONSTRUCTOR(String,  IS_STRING)
AUTOBOX_CONSTRUCTOR(Double,  IS_DOUBLE)
AUTOBOX_CONSTRUCTOR(Boolean, _IS_BOOL)

ZEND_BEGIN_ARG_INFO_EX(php_strict_autobox_construct_arginfo, 0, 0, 1)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(php_strict_autobox_setValue_arginfo, 0, 0, 2)
    ZEND_ARG_INFO(0, type)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

PHP_METHOD(Autobox, setValue) {
    uint32_t type = 0;
    zval     *value = NULL;
    php_strict_autobox_t *autobox;
    
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &type, &value) != SUCCESS) {
        return;
    }
    
    autobox = 
        php_strict_autobox_this();
    
    php_strict_autobox_ctor(autobox, (zend_uchar) type, value TSRMLS_CC);
}

zend_function_entry php_strict_autobox_methods[] = {
    ZEND_ME(Autobox, setValue, php_strict_autobox_setValue_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

zend_function_entry php_strict_integer_methods[] = {
    ZEND_ME(Integer, __construct, php_strict_autobox_construct_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

zend_function_entry php_strict_string_methods[] = {
    ZEND_ME(String, __construct, php_strict_autobox_construct_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

zend_function_entry php_strict_double_methods[] = {
    ZEND_ME(Double, __construct, php_strict_autobox_construct_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

zend_function_entry php_strict_boolean_methods[] = {
    ZEND_ME(Boolean, __construct, php_strict_autobox_construct_arginfo, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};

#define PHP_STRICT_AUTOBOX_REGISTER(t, n, m) do {\
    INIT_NS_CLASS_ENTRY(ce, "strict", n, m); \
    t = zend_register_internal_class_ex(&ce, ce_Autobox TSRMLS_CC); \
} while(0)

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(strict)
{
    zend_class_entry ce;    

    ZEND_INIT_MODULE_GLOBALS(strict, php_strict_init_globals, NULL);

    INIT_NS_CLASS_ENTRY(ce, "strict", "TypeException", NULL);
    ce_TypeException = zend_register_internal_class_ex(
        &ce, zend_exception_get_default(TSRMLS_C) TSRMLS_CC);

    INIT_NS_CLASS_ENTRY(ce, "strict", "Autobox", php_strict_autobox_methods);
    ce_Autobox = zend_register_internal_class(&ce TSRMLS_CC);
    ce_Autobox->create_object = php_strict_autobox_new;
    zend_declare_class_constant_long(ce_Autobox, ZEND_STRL("integer"), IS_LONG TSRMLS_CC);
    zend_declare_class_constant_long(ce_Autobox, ZEND_STRL("double"),  IS_DOUBLE TSRMLS_CC);
    zend_declare_class_constant_long(ce_Autobox, ZEND_STRL("string"),  IS_STRING TSRMLS_CC);
    zend_declare_class_constant_long(ce_Autobox, ZEND_STRL("boolean"), _IS_BOOL TSRMLS_CC);

    PHP_STRICT_AUTOBOX_REGISTER(ce_Integer, "Integer", php_strict_integer_methods);
    PHP_STRICT_AUTOBOX_REGISTER(ce_String,  "String",  php_strict_string_methods);
    PHP_STRICT_AUTOBOX_REGISTER(ce_Double,  "Double",  php_strict_double_methods);
    PHP_STRICT_AUTOBOX_REGISTER(ce_Boolean, "Boolean", php_strict_boolean_methods);

    memcpy(
        &php_strict_autobox_handlers, 
        zend_get_std_object_handlers(), 
        sizeof(zend_object_handlers));

    php_strict_autobox_handlers.dtor_obj       = php_strict_autobox_dtor;
    php_strict_autobox_handlers.get_debug_info = php_strict_autobox_debug;
    php_strict_autobox_handlers.cast_object    = php_strict_autobox_cast;

    zend_set_user_opcode_handler(ZEND_RECV,           php_strict_handler_recv);
    zend_set_user_opcode_handler(ZEND_RECV_INIT,      php_strict_handler_recv);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(strict)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "strict support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ strict_module_entry
 */
zend_module_entry strict_module_entry = {
	STANDARD_MODULE_HEADER,
	"strict",
	NULL,
	PHP_MINIT(strict),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(strict),
	PHP_STRICT_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STRICT
ZEND_GET_MODULE(strict)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
