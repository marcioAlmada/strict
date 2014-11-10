--TEST--
testing boolean hint disallow
--FILE--
<?php
use strict\Boolean;

function test(boolean $bool) {
    var_dump($bool, (boolean) $bool);
}

test(1.1);
?>
--EXPECTF--
Fatal error: Uncaught exception 'strict\TypeException' with message 'expected boolean and received double' in %s:4
Stack trace:
#0 %s(4): strict\Boolean->__construct(1.1)
#1 %s(8): test(1.1)
#2 {main}
  thrown in %s on line 4


