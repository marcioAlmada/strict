--TEST--
testing boolean hint allow
--FILE--
<?php
use strict\Boolean;

function test(boolean $bool) {
    var_dump($bool, (boolean) $bool);
}

test(true);
?>
--EXPECTF--
object(strict\Boolean)#%d (%d) {
  ["value"]=>
  bool(true)
}
bool(true)


