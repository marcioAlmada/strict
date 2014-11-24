--TEST--
testing casting (double)
--FILE--
<?php
function cast_wrapper($value) {
    try {
        return strict_double($value);
    } catch (strict\Exception $ex) { }
}

$values = array(
    "100.0",   # pass
    "100",     # pass
    100.0,     # pass
    100.1,     # pass
    STDIN      # fail
);

foreach ($values as $value) {
    var_dump(cast_wrapper($value));
}
?>
--EXPECTF--
float(100)
float(100)
float(100)
float(100.1)
NULL
