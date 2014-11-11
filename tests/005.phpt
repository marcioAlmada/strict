--TEST--
testing string hint allow
--FILE--
<?php
function test(string $str) {
    var_dump($str);
}

test("hello");
?>
--EXPECTF--
string(5) "hello"


