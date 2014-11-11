--TEST--
testing string hint disallow
--FILE--
<?php
function test(string $str) {
    var_dump($str);
}

test(1.1);
?>
--EXPECTF--
Fatal error: Uncaught exception 'strict\Exception' with message 'illegal implicit cast from double to string at argument 0' in %s:2
Stack trace:
#0 %s(6): test(1.1)
#1 {main}
  thrown in %s on line 2


