--TEST--
testing casting (integer)
--FILE--
<?php
function cast_wrapper($value) {
    try {
        return strict_integer($value);
    } catch (strict\Exception $ex) { }
}

$values = array(
    "100",     # pass
    "100.0",   # fail
    100.0,     # pass
    100.1,     # fail
);

foreach ($values as $value) {
    var_dump(cast_wrapper($value));
}
?>
--EXPECTF--
int(100)
NULL
int(100)
NULL
