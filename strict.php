<?php

function test(integer $int, string $str, double $dbl, boolean $cast) {
     var_dump($int, $str, $dbl, $cast);
}

test(1000, "string", 2.2, true);
test(1000, "string", 2.2, false);
test("1000", "string", 2.2, false);
?>
