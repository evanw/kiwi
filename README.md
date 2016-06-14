# Kiwi Message Format

Kiwi is a schema-based binary format for efficiently encoding trees of data.
It's inspired by Google's [Protocol Buffer](https://developers.google.com/protocol-buffers/) format but is simpler, has a more compact encoding, and has better support for optional fields.

Goals:

* **Efficient encoding of common values:** Variable-length encoding is used for numeric values where small values take up less space.
* **Efficient encoding of compound objects:** The `struct` feature supports nested objects with zero encoding overhead.
* **Presence of optional fields is detectable:** This is not possible with Protocol Buffers, especially for repeated fields.
* **Linearly serializable:** Reading and writing are both single-scan operations so they are cache-efficient and have guaranteed time complexity.
* **Backwards compatibility:** New versions of the schema can still read old data.
* **Forwards compatibility:** Old versions of the schema can optionally read new data if a copy of the new schema is bundled with the data (the new schema lets the decoder skip over unknown fields).
* **Simple implementation:** The API is very minimal and the generated C++ code only depends on a single file.

Non-goals:

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
* **message:** A compound value with optional fields. New fields can be added to any message while maintaining backwards compatibility.

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
  uint clientID = 1;
  Type type = 2;
  Color[] colors = 3;
}
```

## Live Demo

See [http://evanw.github.io/kiwi/](http://evanw.github.io/kiwi/) for a live demo of the schema compiler.

## Usage Examples

The examples below all assume there's a file called `test.kiwi` that looks like this:

```
package test;

message Test {
  int x = 1;
}
```

### JavaScript

Make sure to install the kiwi package beforehand using `npm install kiwi-schema`.

```js
var kiwi = require('kiwi-schema');
var fs = require('fs');
var schema = kiwi.compileSchema(fs.readFileSync('test.kiwi', 'utf8'));

var buffer = schema.encodeTest({x: 123});
var test = schema.decodeTest(buffer);

if (test.x !== undefined) {
  console.log('x is', test.x);
}
```

### C++

The file below can be compiled using something like `c++ main.cpp -std=c++11`.
Make sure to generate the C++ code beforehand using something like `kiwic --schema test.kiwi --cpp test.h`.
The `kiwic` command can be installed using `npm install -g kiwi-schema`.

```cpp
#define IMPLEMENT_KIWI_H
#define IMPLEMENT_SCHEMA_H

#include <stdio.h>
#include "test.h"

int main() {
  test::Test message;
  message.set_x(123);

  kiwi::ByteBuffer buffer;
  bool encode_success = message.encode(buffer);

  test::Test message2;
  kiwi::MemoryPool pool;
  bool decode_success = message2.decode(buffer, pool);

  if (message2.x()) {
    printf("x is %d\n", *message2.x());
  }

  return 0;
}
```

### Skew (http://skew-lang.org/)

The file below can be compiled using something like `skewc *.sk --output-file=out.js`.
Make sure to generate the Skew code beforehand using something like `kiwic --schema test.kiwi --skew test.sk`.
The `kiwic` command can be installed using `npm install -g kiwi-schema`.

```coffee
@import
var console dynamic

@entry
def main int {
  var message = test.Test.new
  message.x = 123

  var buffer = message.encode
  var message2 = test.Test.decode(buffer)

  if message2.has_x {
    console.log("x is \(message2.x)")
  }

  return 0
}
```

## Forwards Compatibility

Forwards compatibility means using an older schema to read data generated with a newer schema.
It can be is achieved by bundling the schema along with the data and is always possible as long as the newer schema is backwards compatible with the older schema.
Separating out the schema instead of mixing it into every message helps reduce the overhead when encoding repeated messages because the schema only needs to be sent once.

The textual schema should be converted to a binary schema when doing this.
An example binary schema can be generated using `kiwic --schema testv2.kiwi --binary testv2.bkiwi` assuming there's a file called `testv2.kiwi` that looks like this:

```
package testv2;

message Test {
  int x = 1;
  int y = 2;
}
```

### JavaScript

Forwards compatibility can be achieved in JavaScript by bundling a copy of the schema along with your data.

```js
var kiwi = require('kiwi-schema');
var fs = require('fs');
var schemaV1 = kiwi.compileSchema(fs.readFileSync('test.kiwi', 'utf8'));
var schemaV2 = kiwi.compileSchema(fs.readFileSync('testv2.kiwi', 'utf8'));

var buffer = schemaV2.encodeTest({x: 123, y: 234});

// Decoding new data using the old version of the schema will fail
// var test = schemaV1.decodeTest(buffer);

// Decoding new data must be done with the new version of the schema
var binarySchemaV2 = kiwi.compileSchema(new Uint8Array(fs.readFileSync('testv2.bkiwi')));
var test = binarySchemaV2.decodeTest(buffer);

console.log('test is', test);
```

### C++

Forwards compatibility can be achieved in C++ by passing the binary schema to the decoder.
Make sure to generate the C++ code beforehand using something like `kiwic --schema testv2.kiwi --cpp testv2.h`.

```cpp
#define IMPLEMENT_KIWI_H
#define IMPLEMENT_SCHEMA_H

#include <stdio.h>
#include "test.h"
#include "testv2.h"

static void readFile(const char *name, kiwi::ByteBuffer &bb) {
  auto f = fopen(name, "rb");
  assert(f);
  int c;
  while ((c = fgetc(f)) != -1) bb.writeByte(c);
  fclose(f);
}

int main() {
  testv2::Test messageV2;
  messageV2.set_x(123);
  messageV2.set_y(234);
  kiwi::ByteBuffer bufferV2;
  bool encode_success = messageV2.encode(bufferV2);
  assert(encode_success);

  // Decoding new data using the old version of the schema will fail
  // test::Test messageV1;
  // kiwi::MemoryPool pool;
  // bool decode_success = messageV1.decode(bufferV2, pool);
  // assert(decode_success);

  // Decoding old data from new data is possible with the new schema
  kiwi::ByteBuffer data;
  readFile("testv2.bkiwi", data);
  test::BinarySchema schema;
  bool parse_success = schema.parse(data);
  assert(parse_success);
  test::Test messageV1;
  kiwi::MemoryPool pool;
  bool decode_success = messageV1.decode(bufferV2, pool, &schema);
  assert(decode_success);

  printf("x is %d\n", *messageV1.x());
  return 0;
}
```

### Skew

Forwards compatibility can be achieved in Skew by passing the binary schema to the decoder.
Make sure to generate the Skew code beforehand using something like `kiwic --schema testv2.kiwi --skew testv2.sk`.

```coffee
@import {
  var console dynamic
  var require dynamic
}

@entry
def main int {
  var messageV2 = testv2.Test.new
  messageV2.x = 123
  messageV2.y = 123
  var bufferV2 = messageV2.encode

  # Decoding new data using the old version of the schema will fail
  # var messageV1 = test.Test.decode(bufferV2)

  # Decoding old data from new data is possible with the new schema
  var fs = require("fs")
  var schema = test.BinarySchema.new
  schema.parse(Uint8Array.new(fs.readFileSync("testv2.bkiwi")))
  var messageV1 = test.Test.decode(bufferV2, schema)

  console.log("x is \(messageV1.x)")
  return 0
}
```
