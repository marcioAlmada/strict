--TEST--
testing integer hint disallow
--FILE--
<?php
function test(integer $int) {
    var_dump($int);
}

test("120");
?>
--EXPECTF--
Fatal error: Uncaught exception 'strict\Exception' with message 'illegal implicit cast from string to integer at argument 0' in %s:2
Stack trace:
#0 %s(6): test('120')
#1 {main}
  thrown in %s on line 2



