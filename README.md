# Kiwi Message Format

This is a binary encoding format inspired by Google's [Protocol Buffer](https://developers.google.com/protocol-buffers/) format.
It is pretty similar but also includes support for efficient compound messages using the `struct` keyword.

## Native Types

* *bool*: A value that stores either `true` or `false`. Will use 1 byte.
* *byte*: An unsigned 8-bit integer value. Uses 1 byte, obviously.
* *int*: A 32-bit integer value stored using a variable-length encoding optimized for storing numbers with a small magnitude. Will use at most 5 bytes.
* *uint*: A 32-bit integer value stored using a variable-length encoding optimized for storing small non-negative numbers. Will use at most 5 bytes.
* *float*: A 32-bit floating-point number. Will always use 4 bytes.
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

message RootMessage {
  required uint clientID = 1;
  Type type = 2;
  Color[] colors = 3;
}
```
