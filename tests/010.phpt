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
Catchable fatal error: Argument 1 passed to test must be resource, integer given in %s on line 2



