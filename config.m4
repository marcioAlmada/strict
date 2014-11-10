PHP_ARG_ENABLE(strict, whether to enable strict support,
[  --enable-strict           Enable strict support])

if test "$PHP_STRICT" != "no"; then
  PHP_NEW_EXTENSION(strict, strict.c, $ext_shared)
fi
