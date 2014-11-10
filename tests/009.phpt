--TEST--
testing user autobox
--FILE--
<?php
use strict\Autobox;

class Integer extends Autobox {

    public function __construct($value) {
        $this->setValue(
            Autobox::integer, $value * 2);
    }
}

class Double extends Autobox {

    public function __construct($value) {
        $this->setValue(
            Autobox::double, $value * 2);
    }
}

function test(integer $int, double $dbl) {
    var_dump(
        $int, (integer) $int,
        $dbl, (double) $dbl);
}

test(1000, 10.101);
?>
--EXPECTF--
object(Integer)#%d (%d) {
  ["value"]=>
  int(2000)
}
int(2000)
object(Double)#%d (%d) {
  ["value"]=>
  float(20.202)
}
float(20.202)



