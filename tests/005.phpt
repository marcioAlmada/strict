--TEST--
testing string hint allow
--FILE--
<?php
use strict\String;

function test(string $str) {
    var_dump($str, (string) $str);
}

test("hello");
?>
--EXPECTF--
object(strict\String)#%d (%d) {
  ["value"]=>
  string(5) "hello"
}
string(5) "hello"


