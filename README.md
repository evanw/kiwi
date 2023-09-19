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
* **int64:** A 64-bit integer value stored using a variable-length encoding optimized for storing numbers with a small magnitude. Will use at most 9 bytes.
* **uint64:** A 64-bit integer value stored using a variable-length encoding optimized for storing small non-negative numbers. Will use at most 9 bytes.
* **T[]:** Any type can be made into an array using the `[]` suffix.

## User Types

* **enum:** A `uint` with a restricted set of values that are identified by name. New fields can be added to any message while maintaining backwards compatibility.
* **struct:** A compound value with a fixed set of fields that are always required and written out in order. New fields cannot be added to a struct once that struct is in use.
* **message:** A compound value with optional fields. New fields can be added to any message while maintaining backwards compatibility.

## Example Schema

```proto
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

Pick a language:

* [JavaScript](./examples/js.md)
* [C++](./examples/cpp.md)
* [Rust](./examples/rust.md)
* [Skew](./examples/skew.md)

## Performance js

`cd ./tree/benchmark/js/`

On an Apple M1 running node v20.7.0

### kiwi

```sh
> node ./kiwi.js

kiwi size: 12
┌─────────┬───────────────────┬─────────────┬────────────────────┬──────────┬─────────┐
│ (index) │     Task Name     │   ops/sec   │ Average Time (ns)  │  Margin  │ Samples │
├─────────┼───────────────────┼─────────────┼────────────────────┼──────────┼─────────┤
│    0    │ 'parse + compile' │  '38 043'   │ 26285.401617436615 │ '±0.25%' │  38044  │
│    1    │    'to binary'    │ '1 682 744' │ 594.2674273496701  │ '±2.04%' │ 1682746 │
│    2    │   'from binary'   │ '8 286 549' │ 120.67748344930484 │ '±0.26%' │ 8286550 │
└─────────┴───────────────────┴─────────────┴────────────────────┴──────────┴─────────┘
```

### protobuf

```sh
> node ./protobuf.js

protobuf size: 12
┌─────────┬───────────────────┬─────────────┬────────────────────┬──────────┬─────────┐
│ (index) │     Task Name     │   ops/sec   │ Average Time (ns)  │  Margin  │ Samples │
├─────────┼───────────────────┼─────────────┼────────────────────┼──────────┼─────────┤
│    0    │ 'parse + compile' │  '54 322'   │ 18408.504504176413 │ '±3.13%' │  54398  │
│    1    │    'to binary'    │ '3 297 715' │ 303.24020830913355 │ '±0.50%' │ 3297716 │
│    2    │   'from binary'   │ '5 120 413' │ 195.2967399870302  │ '±0.50%' │ 5120416 │
└─────────┴───────────────────┴─────────────┴────────────────────┴──────────┴─────────┘
```