--TEST--
testing variadics hint allow
--FILE--
<?php
function test(int ... $ints) {
    var_dump($ints);
}

test(1, 2, 3, 4, 5);
?>
--EXPECT--
array(5) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
  [3]=>
  int(4)
  [4]=>
  int(5)
}




