strict
======

[![Build Status](https://travis-ci.org/krakjoe/strict.svg?branch=master)](https://travis-ci.org/krakjoe/strict)
[![Average time to resolve an issue](http://isitmaintained.com/badge/resolution/krakjoe/strict.svg)](http://isitmaintained.com/project/krakjoe/strict "Average time to resolve an issue")
[![Percentage of issues still open](http://isitmaintained.com/badge/open/krakjoe/strict.svg)](http://isitmaintained.com/project/krakjoe/strict "Percentage of issues still open")

The strict extension introduces support for strict scalar parameter type hints without changing Zend.

Support for the following types is introduced:

  * string
  * integer, int
  * float, double
  * boolean, bool
  * resource

Variadic support for all of the above types is also included.

Passing the wrong type parameter will result in a recoverable error being emitted, 
    which is consistent with the way Zend currently deals with type mismatches.

Casting
=======

The `strict` extension introduces losless-or-exception casting, inspiration taken from the failed safe cast RFC.

The following function is introduced:

 * mixed strict_cast(integer type, mixed value)

| Type      | Value                  | Result |
|:----------|:----------------------:|:------:|
| integer   | `"0xFF"`               | Pass   |
| integer   | `"0b11001"`            | Pass   |
| integer   | `"100"`                | Pass   |
| integer   | `"+100"`               | Pass   |
| integer   | `"-100"`               | Pass   |
| integer   | `0.1`                  | Fail   |
| integer   | `"100 "`               | Fail   |
| double    | `"1.1"`                | Pass   |
| double    | `+1.1`                 | Pass   |
| double    | `".01"`                | Pass   |
| double    | `"0.01 "`              | Fail   |

The aim is that if the value is coherent and casting to the specified type does not cause a loss in precision, then the cast should succeed.

Limitations
===========

  * Internal functions cannot use strict hints, only user code is supported.
  * Because the parser detects scalar hints as classes, they cannot have default values.

*The parser would allow a default value of null, 
    but it would be logically inconsistent to support `null` (you cannot hint for `null`).*

**This extension supports PHP5 and PHP7**
