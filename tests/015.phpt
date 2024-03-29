--TEST--
testing casting (boolean)
--FILE--
<?php
function cast_wrapper($type, $value) {
    try {
        return strict_cast($type, $value);
    } catch (strict\CastException $ex) { }
}

$values = array(
    1.0,        # pass
    1.1,        # fail
    0.0,        # pass
    0.1,        # fail
    "whatever"  # fail
);

foreach ($values as $value) {
    var_dump(cast_wrapper(strict\boolean, $value));
}
?>
--EXPECT--
bool(true)
NULL
bool(false)
NULL
NULL
