--TEST--
testing integer hint allow
--FILE--
<?php
function test(integer $int) {
    var_dump($int);
}

test(120);
?>
--EXPECT--
int(120)

