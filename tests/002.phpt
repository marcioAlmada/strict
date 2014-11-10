--TEST--
testing integer hint disallow
--FILE--
<?php
use strict\Integer;

function test(integer $int) {
    var_dump($int, (integer) $int);
}

test("120");
?>
--EXPECTF--
Fatal error: Uncaught exception 'strict\TypeException' with message 'expected integer and received string' in %s:4
Stack trace:
#0 %s(4): strict\Integer->__construct('120')
#1 %s(8): test('120')
#2 {main}
  thrown in %s on line 4


