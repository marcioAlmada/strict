--TEST--
testing resource hint allow
--FILE--
<?php
function test(resource $fp) {
    var_dump($fp);
}

test(STDIN);
?>
--EXPECTF--
resource(%d) of type (stream)



