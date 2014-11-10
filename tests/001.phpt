--TEST--
testing integer hint allow
--FILE--
<?php
use strict\Integer;

function test(integer $int) {
    var_dump($int, (integer) $int);
}

test(120);
?>
--EXPECTF--
object(strict\Integer)#%d (%d) {
  ["value"]=>
  int(120)
}
int(120)

