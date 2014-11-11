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
#include "Zend/zend_extensions.h"
#include "php_strict.h"

ZEND_DECLARE_MODULE_GLOBALS(strict)

zend_class_entry *ce_StrictException;

/* {{{ php_strict_init_globals
 */
static void php_strict_init_globals(zend_strict_globals *strict_globals){}
/* }}} */

static inline int php_strict_handler_recv(ZEND_OPCODE_HANDLER_ARGS) {
    const zend_function *function = EX(func);
    
    if (function->common.fn_flags & ZEND_ACC_HAS_TYPE_HINTS) {
        const zend_op       *opline   = EX(opline);
        uint32_t             arg      = opline->op1.num;
        zval                *param    = EX_VAR(opline->result.var);
        zend_arg_info       *info     = &function->common.arg_info[arg-1];

        switch (info->type_hint) {
            case _IS_BOOL:
                if (Z_TYPE_P(param) != IS_TRUE && Z_TYPE_P(param) != IS_FALSE) {
                    zend_throw_exception_ex(ce_StrictException, _IS_BOOL TSRMLS_CC,
                        "illegal implicit cast from %s to boolean at argument %d", 
                        zend_get_type_by_const(Z_TYPE_P(param)),
                        arg - 1);
                } else {
                    EX(opline)++;
                    return ZEND_USER_OPCODE_CONTINUE;
                }
            break;
            
            case IS_STRING:
            case IS_DOUBLE:
            case IS_LONG:
                if (info->type_hint != Z_TYPE_P(param)) {
                    zend_throw_exception_ex(ce_StrictException, info->type_hint TSRMLS_CC,
                        "illegal implicit cast from %s to %s at argument %d", 
                        zend_get_type_by_const(Z_TYPE_P(param)),
                        zend_get_type_by_const(info->type_hint),
                        arg - 1);
                } else {
                    EX(opline)++;
                    return ZEND_USER_OPCODE_CONTINUE;
                }
            break;
        }
    }
    
    if (EG(exception)) {
        return ZEND_USER_OPCODE_CONTINUE;
    }
    
    return ZEND_USER_OPCODE_DISPATCH;
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(strict) {
    zend_class_entry ce;    

    ZEND_INIT_MODULE_GLOBALS(strict, php_strict_init_globals, NULL);

    INIT_NS_CLASS_ENTRY(ce, "strict", "Exception", NULL);
    ce_StrictException = zend_register_internal_class_ex(
        &ce, zend_exception_get_default(TSRMLS_C) TSRMLS_CC);

    zend_set_user_opcode_handler(ZEND_RECV,           php_strict_handler_recv);
    zend_set_user_opcode_handler(ZEND_RECV_INIT,      php_strict_handler_recv);

	return SUCCESS;
} /* }}} */

static inline int zend_strict_startup(zend_extension *extension) {
    TSRMLS_FETCH();
    
    zend_startup_module(&strict_module_entry TSRMLS_CC);
}

static inline void zend_strict_compile(zend_op_array *ops) {
    TSRMLS_FETCH();
    
    if (ops->fn_flags & ZEND_ACC_HAS_TYPE_HINTS) {
        
              zend_arg_info *hint = ops->arg_info,
                            *end  = &ops->arg_info[ops->num_args];

#define NEXT() \
    hint++; \
    continue \

#define IS_TYPE(n) \
    (zend_binary_strncasecmp(hint->class_name, hint->class_name_len, ZEND_STRL(n), hint->class_name_len) == SUCCESS)

        do {
            if (IS_TYPE("integer")) {
                hint->type_hint = IS_LONG;
                NEXT();
            }
            
            if (IS_TYPE("double")) {
                hint->type_hint = IS_DOUBLE;
                NEXT();
            }
            
            if (IS_TYPE("string")) {
                hint->type_hint = IS_STRING;
                NEXT();
            }
            
            if (IS_TYPE("boolean")) {
                hint->type_hint = _IS_BOOL;
                NEXT();
            }
            
            hint++;
            
        } while (hint < end);
    }
    
#undef IS_FOR
#undef NEXT
}

#ifndef ZEND_EXT_API
#define ZEND_EXT_API ZEND_DLEXPORT
#endif

ZEND_EXTENSION();

ZEND_EXT_API zend_extension zend_extension_entry = {
	"strict",
    "0.1",
    "Joe Watkins <krakjoe@php.net>",
    "https://github.com/krakjoe/strict",
    "Copyright (c) 2014",
    zend_strict_startup,
    NULL, /* shutdown_func_t */
    NULL, /* activate_func_t */
    NULL, /* deactivate_func_t */
    NULL, /* message_handler_func_t */
    zend_strict_compile, /* op_array_handler_func_t */
    NULL, /* statement_handler_func_t */
    NULL, /* fcall_begin_handler_func_t */
    NULL, /* fcall_end_handler_func_t */
    NULL, /* op_array_ctor_func_t */
    NULL, /* op_array_dtor_func_t */
    STANDARD_ZEND_EXTENSION_PROPERTIES
};

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(strict) {
	php_info_print_table_start();
	php_info_print_table_header(2, "strict support", "enabled");
	php_info_print_table_end();
} /* }}} */

/* {{{ strict_module_entry */
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
}; /* }}} */

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
