--TEST--
testing string hint disallow
--FILE--
<?php
use strict\String;

function test(string $str) {
    var_dump($str, (string) $str);
}

test(1.1);
?>
--EXPECTF--
Fatal error: Uncaught exception 'strict\TypeException' with message 'expected string and received double' in %s:4
Stack trace:
#0 %s(4): strict\String->__construct(1.1)
#1 %s(8): test(1.1)
#2 {main}
  thrown in %s on line 4


