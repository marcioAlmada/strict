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
Catchable fatal error: Argument 1 passed to test must be integer, string given in %s on line 2




