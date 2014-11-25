--TEST--
testing casting (resource)
--FILE--
<?php
function cast_wrapper($type, $value) {
    try {
        return strict_cast($type, $value);
    } catch (strict\CastException $ex) { }
}

class Test {
    public function __toString() {
        return __CLASS__;
    }
}

$resources = array(
   STDIN,
   STDOUT
);

foreach ($resources as $idx => $resource) {
    $handles[$idx] = strict_cast(
        strict\integer, $resource);
}

var_dump($handles);

foreach ($handles as $idx => $handle) {
    var_dump(strict_cast(
        strict\resource, $handle));
}
?>
--EXPECT--
array(2) {
  [0]=>
  int(1)
  [1]=>
  int(2)
}
resource(1) of type (stream)
resource(2) of type (stream)
