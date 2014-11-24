--TEST--
testing casting (integer)
--FILE--
<?php
function cast_wrapper($type, $value) {
    try {
        return strict_cast($type, $value);
    } catch (strict\CastException $ex) { }
}

$values = array(
    "100",     # pass
    "100.0",   # fail
    100.0,     # pass
    100.1,     # fail
);

foreach ($values as $value) {
    var_dump(cast_wrapper(strict\integer, $value));
}
?>
--EXPECTF--
int(100)
NULL
int(100)
NULL
