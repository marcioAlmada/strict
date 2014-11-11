--TEST--
testing double hint disallow
--FILE--
<?php
function test(double $dbl) {
    var_dump($dbl);
}

test(11);
?>
--EXPECTF--
Fatal error: Uncaught exception 'strict\Exception' with message 'illegal implicit cast from integer to double at argument 0' in %s:2
Stack trace:
#0 %s(6): test(11)
#1 {main}
  thrown in %s on line 2



