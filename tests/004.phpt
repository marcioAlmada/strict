--TEST--
testing double hint disallow
--FILE--
<?php
use strict\Double;

function test(double $dbl) {
    var_dump($dbl, (double) $dbl);
}

test(11);
?>
--EXPECTF--
Fatal error: Uncaught exception 'strict\TypeException' with message 'expected double and received integer' in %s:4
Stack trace:
#0 %s(4): strict\Double->__construct(11)
#1 %s(8): test(11)
#2 {main}
  thrown in %s on line 4


