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

#ifndef PHP_STRICT_H
#define PHP_STRICT_H

extern zend_module_entry strict_module_entry;
#define phpext_strict_ptr &strict_module_entry

#define PHP_STRICT_VERSION "0.4.1"
#define PHP_STRICT_EXTNAME "strict"
#define PHP_STRICT_AUTHOR  "Joe Watkins <krakjoe@php.net>"
#define PHP_STRICT_URL     "https://github.com/krakjoe/strict"
#define PHP_STRICT_COPY    "Copyright (c) 2014"

#ifdef PHP_WIN32
#	define PHP_STRICT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_STRICT_API __attribute__ ((visibility("default")))
#else
#	define PHP_STRICT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#endif	/* PHP_STRICT_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
