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
#include "Zend/zend_extensions.h"
#include "php_strict.h"

static inline int php_strict_handler_recv(ZEND_OPCODE_HANDLER_ARGS) {
    const zend_function *function = EX(func);

    if (function->common.fn_flags & ZEND_ACC_HAS_TYPE_HINTS) {
        const zend_op       *opline   = EX(opline);
        uint32_t             arg      = opline->op1.num;
        zval                *param    = EX_VAR(opline->result.var);
        zend_arg_info       *info     = &function->common.arg_info[arg-1];

        switch (info->type_hint) {
            case _IS_BOOL: {
                if (Z_TYPE_P(param) != IS_TRUE && Z_TYPE_P(param) != IS_FALSE) {
                    zend_error(E_RECOVERABLE_ERROR, 
                        "Argument %d passed to %s%s%s must be boolean, %s given",
                        arg, 
                        function->common.scope ? function->common.scope->name->val : "",
                        function->common.scope ? "::" : "",
                        function->common.function_name->val,
                        zend_get_type_by_const(Z_TYPE_P(param)));
                } else EX(opline)++;
                return ZEND_USER_OPCODE_CONTINUE;
            } break;

            case IS_RESOURCE:
            case IS_STRING:
            case IS_DOUBLE:
            case IS_LONG: {
                if (info->type_hint != Z_TYPE_P(param)) {
                    zend_error(E_RECOVERABLE_ERROR, 
                        "Argument %d passed to %s%s%s must be %s, %s given",
                        arg, 
                        function->common.scope ? function->common.scope->name->val : "",
                        function->common.scope ? "::" : "",
                        function->common.function_name->val,
                        zend_get_type_by_const(info->type_hint),
                        zend_get_type_by_const(Z_TYPE_P(param)));
                } else EX(opline)++;
                return ZEND_USER_OPCODE_CONTINUE;
            } break;
        }
    }

    return ZEND_USER_OPCODE_DISPATCH;
}

static inline int zend_strict_startup(zend_extension *extension) {
    TSRMLS_FETCH();
    zend_startup_module(&strict_module_entry TSRMLS_CC);
}

static inline void zend_strict_compile(zend_op_array *ops) {
    TSRMLS_FETCH();
    
    if (ops->fn_flags & ZEND_ACC_HAS_TYPE_HINTS) {
        zend_arg_info *hint = ops->arg_info,
                      *end  = &ops->arg_info[ops->num_args];

#define IS_TYPE(n) \
    (zend_binary_strncasecmp(hint->class_name, hint->class_name_len, ZEND_STRL(n), sizeof(n)-1) == SUCCESS)

#define SET_TYPE(n) \
    hint->type_hint = n; \
    hint++; \
    continue

        do {
            if (IS_TYPE("integer") || IS_TYPE("int")) {
                SET_TYPE(IS_LONG);
            }
            
            if (IS_TYPE("double") || IS_TYPE("float")) {
                SET_TYPE(IS_DOUBLE);
            }
            
            if (IS_TYPE("string")) {
                SET_TYPE(IS_STRING);
            }
            
            if (IS_TYPE("boolean") || IS_TYPE("bool")) {
                SET_TYPE(_IS_BOOL);
            }
            
            if (IS_TYPE("resource")) {
                SET_TYPE(IS_RESOURCE);
            }

            hint++;
        } while (hint < end);
    }

#undef IS_TYPE
#undef SET_TYPE
}

#ifndef ZEND_EXT_API
#define ZEND_EXT_API ZEND_DLEXPORT
#endif

ZEND_EXTENSION();

ZEND_EXT_API zend_extension zend_extension_entry = {
	PHP_STRICT_EXTNAME,
    PHP_STRICT_VERSION,
    PHP_STRICT_AUTHOR,
    PHP_STRICT_URL,
    PHP_STRICT_COPY,
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

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(strict) {
    if (!zend_get_extension("strict")) {
        zend_extension_entry.startup = NULL;
        zend_register_extension(
            &zend_extension_entry, NULL TSRMLS_CC);
    }

    zend_set_user_opcode_handler(ZEND_RECV,           php_strict_handler_recv);
    zend_set_user_opcode_handler(ZEND_RECV_INIT,      php_strict_handler_recv);
	return SUCCESS;
} /* }}} */

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
