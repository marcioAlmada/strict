--TEST--
testing boolean hint allow
--FILE--
<?php
function test(boolean $bool) {
    var_dump($bool);
}

test(true);
?>
--EXPECT--
bool(true)


