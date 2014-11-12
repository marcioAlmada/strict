strict
======

The strict extension introduces support for strict scalar parameter type hints without changing Zend.

Support for the following hints is introduced:

  * string
  * int, integer
  * float, double
  * boolean
  * resource

Passing the wrong type parameter will result in a recoverable error being emitted.

*This extension requires PHP7*
