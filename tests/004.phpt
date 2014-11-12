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
Catchable fatal error: Argument 1 passed to test must be double, integer given in %s on line 2




