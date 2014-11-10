--TEST--
testing double hint allow
--FILE--
<?php
use strict\Double;

function test(double $dbl) {
    var_dump($dbl, (double) $dbl);
}

test(1.1);
?>
--EXPECTF--
object(strict\Double)#%d (%d) {
  ["value"]=>
  float(1.1)
}
float(1.1)

