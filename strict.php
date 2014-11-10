<?php
use strict\Integer,
    strict\Double,
    strict\String,
    strict\Boolean;

function test(integer $int, string $str, double $dbl, boolean $cast) {
    if ($cast) {
        var_dump((integer) $int,
                 (string) $str,
                 (double) $dbl, 
                 (boolean) $cast);
    } else {
        var_dump($int, $str, $dbl, $cast);
    }
}

function add(integer $one, integer $two) {
    var_dump($one + $two);
}

function test2(double $int) {
    var_dump($int);
}

test(1000, "string", 2.2, true);
test(1000, "string", 2.2, false);
test2(1.1);

add(1, 5);
?>
