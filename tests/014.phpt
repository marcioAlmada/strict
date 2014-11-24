--TEST--
testing casting (double)
--FILE--
<?php
function cast_wrapper($type, $value) {
    try {
        return strict_cast($type, $value);
    } catch (strict\CastException $ex) { }
}

$values = array(
    "100.0",   # pass
    "100",     # pass
    100.0,     # pass
    100.1,     # pass
    STDIN      # fail
);

foreach ($values as $value) {
    var_dump(cast_wrapper(strict\double, $value));
}
?>
--EXPECTF--
float(100)
float(100)
float(100)
float(100.1)
NULL
