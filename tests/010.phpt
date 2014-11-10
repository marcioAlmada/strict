--TEST--
testing illegal cast
--FILE--
<?php
use strict\Integer;

function test(integer $int) {      
    var_dump((string) $int);
}

test(1000);
?>
--EXPECTF--
Fatal error: Uncaught exception 'strict\TypeException' with message 'illegal cast to string from integer' in %s:5
Stack trace:
#0 %s(8): test(Object(strict\Integer))
#1 {main}
  thrown in %s on line 5


