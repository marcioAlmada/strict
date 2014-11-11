--TEST--
testing double hint allow
--FILE--
<?php
function test(double $dbl) {
    var_dump($dbl);
}

test(1.1);
?>
--EXPECTF--
float(1.1)

