# Kiwi Message Format

Kiwi is a schema-based binary format for efficiently encoding trees of data.
It's inspired by Google's [Protocol Buffer](https://developers.google.com/protocol-buffers/) format but is simpler, has a more compact encoding, and has better support for optional fields.

Goals:

* **Efficient encoding of common values:** Variable-length encoding is used for numeric values where small values take up less space.
* **Efficient encoding of compound objects:** The `struct` feature supports nested objects with zero encoding overhead.
* **Presence of optional fields is detectable:** This is not possible with Protocol Buffers, especially for repeated fields.
* **Linearly serializable:** Reading and writing are both single-scan operations so they are cache-efficient and have guaranteed time complexity.
* **Backwards compatibility:** New versions of the schema can still read old data.
* **Simple implementation:** The API is very minimal and the generated C++ code only depends on a single file.

Non-goals:

* **Forwards compatibility:** Old versions of the schema cannot read new data.
* **Optimal bit-packing:** Compression can be used after encoding for more space savings if needed.

## Native Types

* **bool:** A value that stores either `true` or `false`. Will use 1 byte.
* **byte:** An unsigned 8-bit integer value. Uses 1 byte, obviously.
* **int:** A 32-bit integer value stored using a variable-length encoding optimized for storing numbers with a small magnitude. Will use at most 5 bytes.
* **uint:** A 32-bit integer value stored using a variable-length encoding optimized for storing small non-negative numbers. Will use at most 5 bytes.
* **float:** A 32-bit floating-point number. Normally uses 4 bytes but a value of zero uses 1 byte ([denormal numbers](https://en.wikipedia.org/wiki/Denormal_number) become zero when encoded).
* **string:** A UTF-8 null-terminated string. Will use at least 1 byte.
* **T[]:** Any type can be made into an array using the `[]` suffix.

## User Types

* **enum:** A `uint` with a restricted set of values that are identified by name. New fields can be added to any message while maintaining backwards compatibility.
* **struct:** A compound value with a fixed set of fields that are always required and written out in order. New fields cannot be added to a struct once that struct is in use.
* **message:** A compound value with optional fields. A field can be made required using the `required` keyword. New fields can be added to any message while maintaining backwards compatibility.

## Example Schema

```
enum Type {
  FLAT = 0;
  ROUND = 1;
  POINTED = 2;
}

struct Color {
  byte red;
  byte green;
  byte blue;
  byte alpha;
}

message Example {
  required uint clientID = 1;
  Type type = 2;
  Color[] colors = 3;
}
```

## Live Demo

See [http://evanw.github.io/kiwi/](http://evanw.github.io/kiwi/) for a live demo of the schema compiler.

## JavaScript Usage

Make sure to install the kiwi package beforehand using `npm install kiwi-schema`.

```js
var kiwi = require('kiwi-schema');
var schema = kiwi.compileSchema([
  'message Test {',
  '  int x = 1;',
  '}',
].join('\n'));

var buffer = schema.encodeTest({x: 123});
var test = schema.decodeTest(buffer);

if (test.x !== undefined) {
  console.log('x is %d', test.x);
}
```

## C++ Usage

Make sure to generate the C++ code beforehand using something like `kiwic --schema test.kiwi --cpp test.h`.

```cpp
#define IMPLEMENT_KIWI_H
#define IMPLEMENT_SCHEMA_H

#include <stdio.h>
#include "test.h"

int main() {
  Test test;
  test.set_x(123);

  kiwi::ByteBuffer buffer;
  bool encode_success = test.encode(buffer);

  Test test2;
  kiwi::MemoryPool pool;
  bool decode_success = test2.decode(buffer, pool);

  if (test2.x()) {
    printf("x is %d\n", *test2.x());
  }

  return 0;
}
```

## [Skew](http://skew-lang.org/) Usage

Make sure to generate the Skew code beforehand using something like `kiwic --schema test.kiwi --skew test.sk`.

```
@import
var console dynamic

@entry
def main int {
  var test = Test.new
  test.x = 123

  var buffer = test.encode
  var test2 = Test.decode(buffer)

  if test2.has_x {
    console.log("x is \(test2.x)")
  }

  return 0
}
```
