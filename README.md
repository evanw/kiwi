# Kiwi Message Format

This is a binary encoding format inspired by Google's [Protocol Buffer](https://developers.google.com/protocol-buffers/) format.

## Native Types

* *bool*: A value that stores either `true` or `false`. Will use 1 byte.
* *byte*: An unsigned 8-bit integer value. Uses 1 byte, obviously.
* *int*: A 32-bit integer value stored using a variable-length encoding optimized for storing numbers with a small magnitude. Will use at most 5 bytes.
* *uint*: A 32-bit integer value stored using a variable-length encoding optimized for storing small non-negative numbers. Will use at most 5 bytes.
* *float*: A 32-bit floating-point number. Normally uses 4 bytes but a value of zero uses 1 byte ([denormal numbers](https://en.wikipedia.org/wiki/Denormal_number) become zero when encoded).
* *string*: A UTF-8 length-prefixed string. Will use at least 1 byte.
* *T[]*: Any type can be made into an array using the `[]` suffix.

## User Types

* *enum*: A `uint` with a restricted set of values that are identified by name.
* *struct*: A compound value with a fixed set of fields that are always required and written out in order.
* *message*: A compound value with optional fields. A field can be made required using the `required` keyword.

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

## Differences from Protocol Buffers

* Kiwi adds support for efficient compound messages using the `struct` keyword
* Enums are scoped to their type instead of dumping everything into the global scope like C
* It's always possible to check for field presence, even for fields that hold arrays
* The generated C++ code is a lot simpler and only depends on a single file, `kiwi.h`

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
