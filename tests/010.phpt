--TEST--
testing resource hint disallow
--FILE--
<?php
function test(resource $fp) {
    var_dump($fp);
}

test(1);
?>
--EXPECTF--
Fatal error: Uncaught exception 'strict\Exception' with message 'illegal implicit cast from integer to resource at argument 0' in %s:2
Stack trace:
#0 %s(6): test(1)
#1 {main}
  thrown in %s on line 2




