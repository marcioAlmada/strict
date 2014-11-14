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

Limitations
===========

  * This extension only provides support for userland code, internal functions cannot use strict hints.
  * Because the parser detects scalar hints as classes, they cannot have default values.

*This extension supports PHP5 and PHP7*
