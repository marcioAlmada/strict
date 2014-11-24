--TEST--
testing casting (string)
--FILE--
<?php
function cast_wrapper($value) {
    try {
        return strict_string($value);
    } catch (strict\CastException $ex) { }
}

class Test {
    public function __toString() {
        return __CLASS__;
    }
}

$values = array(
    0.0,            # pass
    0,              # pass
    1.1,            # pass
    1,              # pass
    true,           # fail
    false,          # fail
    new Test(),     # pass
    new stdClass()  # fail
);

foreach ($values as $value) {
    var_dump(cast_wrapper($value));
}
?>
--EXPECT--
string(1) "0"
string(1) "0"
string(3) "1.1"
string(1) "1"
NULL
NULL
string(4) "Test"
NULL
