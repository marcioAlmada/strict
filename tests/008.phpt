--TEST--
testing boolean hint disallow
--FILE--
<?php
function test(boolean $bool) {
    var_dump($bool);
}

test(1.1);
?>
--EXPECTF--
Fatal error: Uncaught exception 'strict\Exception' with message 'illegal implicit cast from double to boolean at argument 0' in %s:2
Stack trace:
#0 %s(6): test(1.1)
#1 {main}
  thrown in %s on line 2



