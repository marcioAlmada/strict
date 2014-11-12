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
Catchable fatal error: Argument 1 passed to test must be boolean, double given in %s on line 2




