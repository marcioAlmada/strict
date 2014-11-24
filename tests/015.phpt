--TEST--
testing casting (boolean)
--FILE--
<?php
function cast_wrapper($value) {
    try {
        return strict_boolean($value);
    } catch (strict\Exception $ex) { }
}

$values = array(
    1.0,        # pass
    1.1,        # fail
    0.0,        # pass
    0.1,        # fail
    "whatever"  # fail
);

foreach ($values as $value) {
    var_dump(cast_wrapper($value));
}
?>
--EXPECT--
bool(true)
NULL
bool(false)
NULL
NULL
