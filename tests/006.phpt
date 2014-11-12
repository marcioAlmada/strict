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
Catchable fatal error: Argument 1 passed to test must be string, double given in %s on line 2



