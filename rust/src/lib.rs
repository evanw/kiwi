//! This is a Rust library with some helper routines for parsing files in the
//! Kiwi serialization format. See [https://github.com/evanw/kiwi](https://github.com/evanw/kiwi)
//! for documentation about the format.
//!
//! ```
//! use kiwi_schema::*;
//!
//! let schema = Schema::new(vec![
//!   Def::new("Point".to_owned(), DefKind::Struct, vec![
//!     Field {name: "x".to_owned(), type_id: TYPE_FLOAT, is_array: false, value: 0},
//!     Field {name: "y".to_owned(), type_id: TYPE_FLOAT, is_array: false, value: 0},
//!   ]),
//! ]);
//!
//! let value = Value::decode(&schema, 0, &[126, 0, 0, 0, 126, 1, 0, 0]).unwrap();
//! assert_eq!(format!("{:?}", value), "Point {x: 0.5, y: -0.5}");
//! assert_eq!(value.encode(&schema), [126, 0, 0, 0, 126, 1, 0, 0]);
//! ```

use std::borrow::Cow;
use std::collections::HashMap;
use std::f32;
use std::fmt;
use std::ops::Index;
use std::str;

/// A Kiwi byte buffer meant for reading.
///
/// Example usage:
///
/// ```
/// use std::borrow::Cow;
/// let mut bb = kiwi_schema::ByteBuffer::new(&[240, 159, 141, 149, 0, 133, 242, 210, 237]);
/// assert_eq!(bb.read_string(), Ok(Cow::Borrowed("🍕")));
/// assert_eq!(bb.read_var_float(), Ok(123.456));
/// ```
///
pub struct ByteBuffer<'a> {
  data: &'a [u8],
  index: usize,
}

impl<'a> ByteBuffer<'a> {
  /// Create a new ByteBuffer that wraps the provided byte slice. The lifetime
  /// of the returned ByteBuffer must not outlive the lifetime of the byte
  /// slice.
  pub fn new(data: &[u8]) -> ByteBuffer {
    ByteBuffer{data, index: 0}
  }

  /// Retrieves the underlying byte slice.
  pub fn data(&self) -> &'a [u8] {
    self.data
  }

  /// Retrieves the current index into the underlying byte slice. This starts
  /// off as 0 and ends up as `self.data().len()` when everything has been
  /// read.
  pub fn index(&self) -> usize {
    self.index
  }

  /// Try to read a boolean value starting at the current index.
  pub fn read_bool(&mut self) -> Result<bool, ()> {
    match self.read_byte() {
      Ok(0) => Ok(false),
      Ok(1) => Ok(true),
      _ => Err(()),
    }
  }

  /// Try to read a byte starting at the current index.
  pub fn read_byte(&mut self) -> Result<u8, ()> {
    if self.index >= self.data.len() {
      Err(())
    } else {
      let value = self.data[self.index];
      self.index = self.index + 1;
      Ok(value)
    }
  }

  /// Try to read a byte starting at the current index.
  pub fn read_bytes(&mut self, len: usize) -> Result<&'a [u8], ()> {
    if self.index + len > self.data.len() {
      Err(())
    } else {
      let value = &self.data[self.index..self.index + len];
      self.index = self.index + len;
      Ok(value)
    }
  }

  /// Try to read a variable-length signed 32-bit integer starting at the
  /// current index.
  pub fn read_var_int(&mut self) -> Result<i32, ()> {
    let value = self.read_var_uint()?;
    Ok((if (value & 1) != 0 { !(value >> 1) } else { value >> 1 }) as i32)
  }

  /// Try to read a variable-length unsigned 32-bit integer starting at the
  /// current index.
  pub fn read_var_uint(&mut self) -> Result<u32, ()> {
    let mut shift: u8 = 0;
    let mut result: u32 = 0;

    loop {
      let byte = self.read_byte()?;
      result |= ((byte & 127) as u32) << shift;
      shift += 7;

      if (byte & 128) == 0 || shift >= 35 {
        break;
      }
    }

    Ok(result)
  }

  /// Try to read a variable-length 32-bit floating-point number starting at
  /// the current index.
  pub fn read_var_float(&mut self) -> Result<f32, ()> {
    let first = self.read_byte()?;

    // Optimization: use a single byte to store zero
    if first == 0 {
      Ok(0.0)
    }

    else if self.index + 3 > self.data.len() {
      Err(())
    }

    // Endian-independent 32-bit read
    else {
      let mut bits: u32 =
        first as u32 |
        ((self.data[self.index] as u32) << 8) |
        ((self.data[self.index + 1] as u32) << 16) |
        ((self.data[self.index + 2] as u32) << 24);
      self.index += 3;

      // Move the exponent back into place
      bits = (bits << 23) | (bits >> 9);

      Ok(f32::from_bits(bits))
    }
  }

  /// Try to read a UTF-8 string starting at the current index. This string is
  /// returned as a slice so it just aliases the underlying memory.
  pub fn read_string(&mut self) -> Result<Cow<'a, str>, ()> {
    let start = self.index;

    while self.index < self.data.len() {
      if self.data[self.index] == 0 {
        self.index += 1;
        return Ok(String::from_utf8_lossy(&self.data[start..self.index - 1]));
      }

      self.index += 1;
    }

    Err(())
  }
}

#[test]
fn read_bool() {
  let try = |bytes| { ByteBuffer::new(bytes).read_bool() };
  assert_eq!(try(&[]), Err(()));
  assert_eq!(try(&[0]), Ok(false));
  assert_eq!(try(&[1]), Ok(true));
  assert_eq!(try(&[2]), Err(()));
}

#[test]
fn read_byte() {
  let try = |bytes| { ByteBuffer::new(bytes).read_byte() };
  assert_eq!(try(&[]), Err(()));
  assert_eq!(try(&[0]), Ok(0));
  assert_eq!(try(&[1]), Ok(1));
  assert_eq!(try(&[254]), Ok(254));
  assert_eq!(try(&[255]), Ok(255));
}

#[test]
fn read_bytes() {
  let try = |bytes, len| { ByteBuffer::new(bytes).read_bytes(len) };
  assert_eq!(try(&[], 0), Ok(vec![].as_slice()));
  assert_eq!(try(&[], 1), Err(()));
  assert_eq!(try(&[0], 0), Ok(vec![].as_slice()));
  assert_eq!(try(&[0], 1), Ok(vec![0].as_slice()));
  assert_eq!(try(&[0], 2), Err(()));

  let mut bb = ByteBuffer::new(&[1, 2, 3, 4, 5]);
  assert_eq!(bb.read_bytes(3), Ok(vec![1, 2, 3].as_slice()));
  assert_eq!(bb.read_bytes(2), Ok(vec![4, 5].as_slice()));
  assert_eq!(bb.read_bytes(1), Err(()));
}

#[test]
fn read_var_int() {
  let try = |bytes| { ByteBuffer::new(bytes).read_var_int() };
  assert_eq!(try(&[]), Err(()));
  assert_eq!(try(&[0]), Ok(0));
  assert_eq!(try(&[1]), Ok(-1));
  assert_eq!(try(&[2]), Ok(1));
  assert_eq!(try(&[3]), Ok(-2));
  assert_eq!(try(&[4]), Ok(2));
  assert_eq!(try(&[127]), Ok(-64));
  assert_eq!(try(&[128]), Err(()));
  assert_eq!(try(&[128, 0]), Ok(0));
  assert_eq!(try(&[128, 1]), Ok(64));
  assert_eq!(try(&[128, 2]), Ok(128));
  assert_eq!(try(&[129, 0]), Ok(-1));
  assert_eq!(try(&[129, 1]), Ok(-65));
  assert_eq!(try(&[129, 2]), Ok(-129));
  assert_eq!(try(&[253, 255, 7]), Ok(-65535));
  assert_eq!(try(&[254, 255, 7]), Ok(65535));
  assert_eq!(try(&[253, 255, 255, 255, 15]), Ok(-2147483647));
  assert_eq!(try(&[254, 255, 255, 255, 15]), Ok(2147483647));
  assert_eq!(try(&[255, 255, 255, 255, 15]), Ok(-2147483648));
}

#[test]
fn read_var_uint() {
  let try = |bytes| { ByteBuffer::new(bytes).read_var_uint() };
  assert_eq!(try(&[]), Err(()));
  assert_eq!(try(&[0]), Ok(0));
  assert_eq!(try(&[1]), Ok(1));
  assert_eq!(try(&[2]), Ok(2));
  assert_eq!(try(&[3]), Ok(3));
  assert_eq!(try(&[4]), Ok(4));
  assert_eq!(try(&[127]), Ok(127));
  assert_eq!(try(&[128]), Err(()));
  assert_eq!(try(&[128, 0]), Ok(0));
  assert_eq!(try(&[128, 1]), Ok(128));
  assert_eq!(try(&[128, 2]), Ok(256));
  assert_eq!(try(&[129, 0]), Ok(1));
  assert_eq!(try(&[129, 1]), Ok(129));
  assert_eq!(try(&[129, 2]), Ok(257));
  assert_eq!(try(&[253, 255, 7]), Ok(131069));
  assert_eq!(try(&[254, 255, 7]), Ok(131070));
  assert_eq!(try(&[253, 255, 255, 255, 15]), Ok(4294967293));
  assert_eq!(try(&[254, 255, 255, 255, 15]), Ok(4294967294));
  assert_eq!(try(&[255, 255, 255, 255, 15]), Ok(4294967295));
}

#[test]
fn read_var_float() {
  let try = |bytes| { ByteBuffer::new(bytes).read_var_float() };
  assert_eq!(try(&[]), Err(()));
  assert_eq!(try(&[0]), Ok(0.0));
  assert_eq!(try(&[133, 242, 210, 237]), Ok(123.456));
  assert_eq!(try(&[133, 243, 210, 237]), Ok(-123.456));
  assert_eq!(try(&[254, 255, 255, 255]), Ok(f32::MIN));
  assert_eq!(try(&[254, 254, 255, 255]), Ok(f32::MAX));
  assert_eq!(try(&[1, 1, 0, 0]), Ok(-f32::MIN_POSITIVE));
  assert_eq!(try(&[1, 0, 0, 0]), Ok(f32::MIN_POSITIVE));
  assert_eq!(try(&[255, 1, 0, 0]), Ok(f32::NEG_INFINITY));
  assert_eq!(try(&[255, 0, 0, 0]), Ok(f32::INFINITY));
  assert_eq!(try(&[255, 0, 0, 128]).map(|f| f.is_nan()), Ok(true));
}

#[test]
fn read_string() {
  let try = |bytes| { ByteBuffer::new(bytes).read_string() };
  assert_eq!(try(&[]), Err(()));
  assert_eq!(try(&[0]), Ok(Cow::Borrowed("")));
  assert_eq!(try(&[97]), Err(()));
  assert_eq!(try(&[97, 0]), Ok(Cow::Borrowed("a")));
  assert_eq!(try(&[97, 98, 99, 0]), Ok(Cow::Borrowed("abc")));
  assert_eq!(try(&[240, 159, 141, 149, 0]), Ok(Cow::Borrowed("🍕")));
  assert_eq!(try(&[97, 237, 160, 188, 99, 0]), Ok(Cow::Owned("a���c".to_owned())));
}

#[test]
fn read_sequence() {
  let mut bb = ByteBuffer::new(&[0, 133, 242, 210, 237, 240, 159, 141, 149, 0, 149, 154, 239, 58]);
  assert_eq!(bb.read_var_float(), Ok(0.0));
  assert_eq!(bb.read_var_float(), Ok(123.456));
  assert_eq!(bb.read_string(), Ok(Cow::Borrowed("🍕")));
  assert_eq!(bb.read_var_uint(), Ok(123456789));
}

/// A Kiwi byte buffer meant for writing.
///
/// Example usage:
///
/// ```
/// let mut bb = kiwi_schema::ByteBufferMut::new();
/// bb.write_string("🍕");
/// bb.write_var_float(123.456);
/// assert_eq!(bb.data(), [240, 159, 141, 149, 0, 133, 242, 210, 237]);
/// ```
///
pub struct ByteBufferMut {
  data: Vec<u8>,
}

impl ByteBufferMut {
  /// Creates an empty ByteBufferMut ready for writing.
  pub fn new() -> ByteBufferMut {
    ByteBufferMut{data: vec![]}
  }

  /// Consumes this buffer and returns the underlying backing store. Use this
  /// to get the data out when you're done writing to the buffer.
  pub fn data(self) -> Vec<u8> {
    self.data
  }

  /// Returns the number of bytes written so far.
  pub fn len(&self) -> usize {
    self.data.len()
  }

  /// Write a boolean value to the end of the buffer.
  pub fn write_bool(&mut self, value: bool) {
    self.data.push(if value { 1 } else { 0 });
  }

  /// Write a byte to the end of the buffer.
  pub fn write_byte(&mut self, value: u8) {
    self.data.push(value);
  }

  /// Write a raw byte slice to the end of the buffer.
  pub fn write_bytes(&mut self, value: &[u8]) {
    self.data.extend_from_slice(value);
  }

  /// Write a variable-length signed 32-bit integer to the end of the buffer.
  pub fn write_var_int(&mut self, value: i32) {
    self.write_var_uint(((value << 1) ^ (value >> 31)) as u32);
  }

  /// Write a variable-length unsigned 32-bit integer to the end of the buffer.
  pub fn write_var_uint(&mut self, mut value: u32) {
    loop {
      let byte = value as u8 & 127;
      value >>= 7;

      if value == 0 {
        self.write_byte(byte);
        return;
      }

      self.write_byte(byte | 128);
    }
  }

  /// Write a variable-length 32-bit floating-point number to the end of the
  /// buffer.
  pub fn write_var_float(&mut self, value: f32) {
    // Reinterpret as an integer
    let mut bits = value.to_bits();

    // Move the exponent to the first 8 bits
    bits = (bits >> 23) | (bits << 9);

    // Optimization: use a single byte to store zero and denormals (try for an exponent of 0)
    if (bits & 255) == 0 {
      self.data.push(0);
      return;
    }

    // Endian-independent 32-bit write
    self.data.extend_from_slice(&[
      bits as u8,
      (bits >> 8) as u8,
      (bits >> 16) as u8,
      (bits >> 24) as u8,
    ]);
  }

  /// Write a UTF-8 string to the end of the buffer.
  pub fn write_string(&mut self, value: &str) {
    self.data.extend_from_slice(value.as_bytes());
    self.data.push(0);
  }
}

#[cfg(test)]
fn write_once(cb: fn(&mut ByteBufferMut)) -> Vec<u8> {
  let mut bb = ByteBufferMut::new();
  cb(&mut bb);
  bb.data()
}

#[test]
fn write_bool() {
  assert_eq!(write_once(|bb| bb.write_bool(false)), [0]);
  assert_eq!(write_once(|bb| bb.write_bool(true)), [1]);
}

#[test]
fn write_byte() {
  assert_eq!(write_once(|bb| bb.write_byte(0)), [0]);
  assert_eq!(write_once(|bb| bb.write_byte(1)), [1]);
  assert_eq!(write_once(|bb| bb.write_byte(254)), [254]);
  assert_eq!(write_once(|bb| bb.write_byte(255)), [255]);
}

#[test]
fn write_bytes() {
  let mut bb = ByteBufferMut::new();
  bb.write_bytes(&[1, 2, 3]);
  bb.write_bytes(&[]);
  bb.write_bytes(&[4, 5]);
  assert_eq!(bb.data(), [1, 2, 3, 4, 5]);
}

#[test]
fn write_var_int() {
  assert_eq!(write_once(|bb| bb.write_var_int(0)), [0]);
  assert_eq!(write_once(|bb| bb.write_var_int(-1)), [1]);
  assert_eq!(write_once(|bb| bb.write_var_int(1)), [2]);
  assert_eq!(write_once(|bb| bb.write_var_int(-2)), [3]);
  assert_eq!(write_once(|bb| bb.write_var_int(2)), [4]);
  assert_eq!(write_once(|bb| bb.write_var_int(-64)), [127]);
  assert_eq!(write_once(|bb| bb.write_var_int(64)), [128, 1]);
  assert_eq!(write_once(|bb| bb.write_var_int(128)), [128, 2]);
  assert_eq!(write_once(|bb| bb.write_var_int(-129)), [129, 2]);
  assert_eq!(write_once(|bb| bb.write_var_int(-65535)), [253, 255, 7]);
  assert_eq!(write_once(|bb| bb.write_var_int(65535)), [254, 255, 7]);
  assert_eq!(write_once(|bb| bb.write_var_int(-2147483647)), [253, 255, 255, 255, 15]);
  assert_eq!(write_once(|bb| bb.write_var_int(2147483647)), [254, 255, 255, 255, 15]);
  assert_eq!(write_once(|bb| bb.write_var_int(-2147483648)), [255, 255, 255, 255, 15]);
}

#[test]
fn write_var_uint() {
  assert_eq!(write_once(|bb| bb.write_var_uint(0)), [0]);
  assert_eq!(write_once(|bb| bb.write_var_uint(1)), [1]);
  assert_eq!(write_once(|bb| bb.write_var_uint(2)), [2]);
  assert_eq!(write_once(|bb| bb.write_var_uint(3)), [3]);
  assert_eq!(write_once(|bb| bb.write_var_uint(4)), [4]);
  assert_eq!(write_once(|bb| bb.write_var_uint(127)), [127]);
  assert_eq!(write_once(|bb| bb.write_var_uint(128)), [128, 1]);
  assert_eq!(write_once(|bb| bb.write_var_uint(256)), [128, 2]);
  assert_eq!(write_once(|bb| bb.write_var_uint(129)), [129, 1]);
  assert_eq!(write_once(|bb| bb.write_var_uint(257)), [129, 2]);
  assert_eq!(write_once(|bb| bb.write_var_uint(131069)), [253, 255, 7]);
  assert_eq!(write_once(|bb| bb.write_var_uint(131070)), [254, 255, 7]);
  assert_eq!(write_once(|bb| bb.write_var_uint(4294967293)), [253, 255, 255, 255, 15]);
  assert_eq!(write_once(|bb| bb.write_var_uint(4294967294)), [254, 255, 255, 255, 15]);
  assert_eq!(write_once(|bb| bb.write_var_uint(4294967295)), [255, 255, 255, 255, 15]);
}

#[test]
fn write_var_float() {
  assert_eq!(write_once(|bb| bb.write_var_float(0.0)), [0]);
  assert_eq!(write_once(|bb| bb.write_var_float(-0.0)), [0]);
  assert_eq!(write_once(|bb| bb.write_var_float(123.456)), [133, 242, 210, 237]);
  assert_eq!(write_once(|bb| bb.write_var_float(-123.456)), [133, 243, 210, 237]);
  assert_eq!(write_once(|bb| bb.write_var_float(f32::MIN)), [254, 255, 255, 255]);
  assert_eq!(write_once(|bb| bb.write_var_float(f32::MAX)), [254, 254, 255, 255]);
  assert_eq!(write_once(|bb| bb.write_var_float(-f32::MIN_POSITIVE)), [1, 1, 0, 0]);
  assert_eq!(write_once(|bb| bb.write_var_float(f32::MIN_POSITIVE)), [1, 0, 0, 0]);
  assert_eq!(write_once(|bb| bb.write_var_float(f32::NEG_INFINITY)), [255, 1, 0, 0]);
  assert_eq!(write_once(|bb| bb.write_var_float(f32::INFINITY)), [255, 0, 0, 0]);
  assert_eq!(write_once(|bb| bb.write_var_float(f32::NAN)), [255, 0, 0, 128]);
  assert_eq!(write_once(|bb| bb.write_var_float(1.0e-40)), [0]);
}

#[test]
fn write_string() {
  assert_eq!(write_once(|bb| bb.write_string("")), [0]);
  assert_eq!(write_once(|bb| bb.write_string("a")), [97, 0]);
  assert_eq!(write_once(|bb| bb.write_string("abc")), [97, 98, 99, 0]);
  assert_eq!(write_once(|bb| bb.write_string("🍕")), [240, 159, 141, 149, 0]);
}

#[test]
fn write_sequence() {
  let mut bb = ByteBufferMut::new();
  bb.write_var_float(0.0);
  bb.write_var_float(123.456);
  bb.write_string("🍕");
  bb.write_var_uint(123456789);
  assert_eq!(bb.data(), [0, 133, 242, 210, 237, 240, 159, 141, 149, 0, 149, 154, 239, 58]);
}

pub const TYPE_BOOL: i32 = -1;
pub const TYPE_BYTE: i32 = -2;
pub const TYPE_INT: i32 = -3;
pub const TYPE_UINT: i32 = -4;
pub const TYPE_FLOAT: i32 = -5;
pub const TYPE_STRING: i32 = -6;

/// Represents a single field in a [Def](struct.Def.html).
#[derive(Debug, PartialEq)]
pub struct Field {
  /// The name of this field from the textual Kiwi schema.
  pub name: String,

  /// For user-defined types, this is the index of the [Def](struct.Def.html)
  /// in the `defs` array of the [Schema](struct.Schema.html).
  /// Built-in types use special constants:
  ///
  /// * [TYPE_BOOL](constant.TYPE_BOOL.html)
  /// * [TYPE_BYTE](constant.TYPE_BYTE.html)
  /// * [TYPE_INT](constant.TYPE_INT.html)
  /// * [TYPE_UINT](constant.TYPE_UINT.html)
  /// * [TYPE_FLOAT](constant.TYPE_FLOAT.html)
  /// * [TYPE_STRING](constant.TYPE_STRING.html)
  pub type_id: i32,

  /// True if this field was declared as an array (e.g. `int[]` instead of
  /// `int` in the textual Kiwi schema). Arrays are encoded using a length
  /// prefix followed by that many items.
  pub is_array: bool,

  /// The identifier corresponding to this field. This is the enum value for
  /// enum definitions and the field id for message definitions. This value has
  /// no meaning for struct definitions.
  pub value: u32,
}

#[derive(Debug, PartialEq, Eq)]
pub enum DefKind {
  /// Enums are encoded as variable-length unsigned integers under the hood.
  /// Declaring one in the textual Kiwi format looks like this:
  ///
  /// ```text
  /// enum Example {
  ///   A = 100;
  ///   B = 200;
  /// }
  /// ```
  Enum,

  /// Structs are tuples of fields and are encoded by encoding each field in
  /// order. All fields are guaranteed to be present. Declaring one in the
  /// textual Kiwi format looks like this:
  ///
  /// ```text
  /// struct Example {
  ///   int a;
  ///   int b;
  /// }
  /// ```
  Struct,

  /// Messages are objects filled with optional fields and are encoded as a
  /// sequence of (id, value) pairs followed by a zero byte. All fields are
  /// optional and may not be present. Declaring one in the textual Kiwi format
  /// looks like this:
  ///
  /// ```text
  /// message Example {
  ///   int a = 1;
  ///   int b = 2;
  /// }
  /// ```
  Message,
}

pub const DEF_ENUM: u8 = 0;
pub const DEF_STRUCT: u8 = 1;
pub const DEF_MESSAGE: u8 = 2;

/// Represents a single definition in a [Schema](struct.Schema.html).
/// Kiwi enums, structs, and messages are all represented using this object.
#[derive(Debug, PartialEq)]
pub struct Def {
  /// The name of this field from the textual Kiwi schema.
  pub name: String,

  /// The index of this Def in the `defs` field of the parent
  /// [Schema](struct.Schema.html). This is used as the type
  /// identifier for any fields with this type.
  pub index: i32,

  /// Either [Enum](enum.DefKind.html#variant.Enum),
  /// [Struct](enum.DefKind.html#variant.Struct), or
  /// [Message](enum.DefKind.html#variant.Message).
  pub kind: DefKind,

  /// All fields in this definition. The order matters for struct definitions.
  pub fields: Vec<Field>,

  /// Maps the `value` and `name` members of each [Field](struct.Field.html) in
  /// the `fields` array to its index in that array. This is helpful when
  /// decoding and encoding a field to be able to quickly get to the field
  /// metadata.
  pub field_value_to_index: HashMap<u32, usize>,
  pub field_name_to_index: HashMap<String, usize>,
}

impl Def {
  pub fn new(name: String, kind: DefKind, fields: Vec<Field>) -> Def {
    let mut field_value_to_index = HashMap::new();
    let mut field_name_to_index = HashMap::new();
    for (i, field) in fields.iter().enumerate() {
      field_value_to_index.insert(field.value, i);
      field_name_to_index.insert(field.name.clone(), i);
    }
    Def {name, index: 0, kind, fields, field_value_to_index, field_name_to_index}
  }

  /// Returns the [Field](struct.Field.html) with the provided name if one exists.
  pub fn field(&self, name: &str) -> Option<&Field> {
    self.field_name_to_index.get(name).map(|i| &self.fields[*i])
  }
}

#[derive(Debug, PartialEq)]
pub struct SchemaOptions {
  // The Rust implementation is the only one that validates enums while skipping fields.
  // Ideally all implementations should end up validating enums, but to maintain
  // compatibility with other clients this can be used to allow enums to be decoded
  // even if they aren't valid.
  pub validate_enums: bool,
}


/// Holds the contents of a Kiwi schema.
///
/// Each schema consists of a list of definitions, each of which has a list of
/// fields. It can either be constructed in memory or decoded from a file in
/// the binary Kiwi schema format.
///
/// Example usage:
///
/// ```
/// // This is the encoding of the Kiwi schema "message ABC { int[] xyz = 1; }"
/// let schema_bytes = [1, 65, 66, 67, 0, 2, 1, 120, 121, 122, 0, 5, 1, 1];
/// let schema = kiwi_schema::Schema::decode(&schema_bytes).unwrap();
///
/// let def = schema.def("ABC").unwrap();
/// assert_eq!(def.kind, kiwi_schema::DefKind::Message);
///
/// let field = def.field("xyz").unwrap();
/// assert_eq!(field.type_id, kiwi_schema::TYPE_INT);
/// assert_eq!(field.is_array, true);
/// assert_eq!(field.value, 1);
/// ```
#[derive(Debug, PartialEq)]
pub struct Schema {
  pub defs: Vec<Def>,

  /// Maps the `name` member of each [Def](struct.Def.html) in the `defs` array
  /// to its index in that array. This is helpful when decoding and encoding a
  /// field to be able to quickly get to the field metadata.
  pub def_name_to_index: HashMap<String, usize>,
}

impl Schema {
  pub fn new(mut defs: Vec<Def>) -> Schema {
    let mut def_name_to_index = HashMap::new();
    for (i, def) in defs.iter_mut().enumerate() {
      def.index = i as i32;
      def_name_to_index.insert(def.name.clone(), i);
    }
    Schema {defs, def_name_to_index}
  }

  /// Parses a Kiwi schema encoded in the binary format and returns the parsed
  /// schema if successful. A textual schema can be compiled into a binary
  /// schema using the command-line tools:
  ///
  /// ```text
  /// kiwic --schema example.kiwi --binary example.bkiwi
  /// ```
  pub fn decode(bytes: &[u8]) -> Result<Schema, ()> {
    let mut defs = Vec::new();
    let mut bb = ByteBuffer::new(bytes);
    let definition_count = bb.read_var_uint()?;

    for _ in 0..definition_count {
      let name = bb.read_string()?.into_owned();
      let kind = match bb.read_byte()? {
        DEF_ENUM => DefKind::Enum,
        DEF_STRUCT => DefKind::Struct,
        DEF_MESSAGE => DefKind::Message,
        _ => return Err(()),
      };
      let field_count = bb.read_var_uint()?;
      let mut fields = Vec::new();

      for _ in 0..field_count {
        let name = bb.read_string()?.into_owned();
        let type_id = bb.read_var_int()?;
        let is_array = bb.read_bool()?;
        let value = bb.read_var_uint()?;
        fields.push(Field {name, type_id, is_array, value});
      }

      defs.push(Def::new(name, kind, fields));
    }

    Ok(Schema::new(defs))
  }

  /// The opposite of [decode](#method.decode). Turns this schema back into a
  /// binary file.
  pub fn encode(&self) -> Vec<u8> {
    let mut bb = ByteBufferMut::new();
    bb.write_var_uint(self.defs.len() as u32);

    for def in &self.defs {
      bb.write_string(def.name.as_str());
      bb.write_byte(match def.kind {
        DefKind::Enum => DEF_ENUM,
        DefKind::Struct => DEF_STRUCT,
        DefKind::Message => DEF_MESSAGE,
      });
      bb.write_var_uint(def.fields.len() as u32);

      for field in &def.fields {
        bb.write_string(field.name.as_str());
        bb.write_var_int(field.type_id);
        bb.write_bool(field.is_array);
        bb.write_var_uint(field.value);
      }
    }

    bb.data()
  }

  /// Returns the [Def](struct.Def.html) with the provided name if one exists.
  pub fn def(&self, name: &str) -> Option<&Def> {
    self.def_name_to_index.get(name).map(|i| &self.defs[*i])
  }

  /// Advances the current index of the provided [ByteBuffer](struct.ByteBuffer.html)
  /// by the size of a field with the provided type information. The Kiwi format
  /// doesn't support seeking around to arbitrary points (it must be read from
  /// start to end) so this method is helpful when you need to to skip past
  /// unimportant fields.
  pub fn skip_with_options(&self, bb: &mut ByteBuffer, type_id: i32, options: &SchemaOptions) -> Result<(), ()> {
    match type_id {
      TYPE_BOOL => { bb.read_bool()?; },
      TYPE_BYTE => { bb.read_byte()?; },
      TYPE_INT => { bb.read_var_int()?; },
      TYPE_UINT => { bb.read_var_uint()?; },
      TYPE_FLOAT => { bb.read_var_float()?; },
      TYPE_STRING => { bb.read_string()?; },

      _ => {
        let def = &self.defs[type_id as usize];

        match def.kind {
          DefKind::Enum => {
            if !def.field_value_to_index.contains_key(&bb.read_var_uint()?) && options.validate_enums {
              return Err(());
            }
          },

          DefKind::Struct => {
            for field in &def.fields {
              self.skip_field_with_options(bb, field, options)?;
            }
          },

          DefKind::Message => {
            loop {
              let value = bb.read_var_uint()?;
              if value == 0 {
                break;
              }
              if let Some(index) = def.field_value_to_index.get(&value) {
                self.skip_field_with_options(bb, &def.fields[*index], options)?;
              } else {
                return Err(());
              }
            }
          },
        }
      },
    }

    Ok(())
  }

  pub fn skip(&self, bb: &mut ByteBuffer, type_id: i32) -> Result<(), ()> {
    self.skip_with_options(bb, type_id, &SchemaOptions {validate_enums: true})
  }

  /// Advances the current index of the provided [ByteBuffer](struct.ByteBuffer.html)
  /// by the size of the provided field. This is used by [skip](#method.skip)
  /// but may also be useful by itself.
  pub fn skip_field_with_options(&self, bb: &mut ByteBuffer, field: &Field, options: &SchemaOptions) -> Result<(), ()> {
    if field.is_array {
      let len = bb.read_var_uint()? as usize;
      for _ in 0..len {
        self.skip_with_options(bb, field.type_id, options)?;
      }
    } else {
      self.skip_with_options(bb, field.type_id, options)?;
    }
    Ok(())
  }

  pub fn skip_field(&self, bb: &mut ByteBuffer, field: &Field) -> Result<(), ()> {
    self.skip_field_with_options(bb, field, &SchemaOptions {validate_enums: true})
  }
}

#[test]
fn schema_decode_and_encode() {
  // This is the encoding of the Kiwi schema "message ABC { int[] xyz = 1; }"
  let schema_bytes = [1, 65, 66, 67, 0, 2, 1, 120, 121, 122, 0, 5, 1, 1];
  let schema = Schema::decode(&schema_bytes).unwrap();
  assert_eq!(schema, Schema::new(vec![
    Def::new("ABC".to_owned(), DefKind::Message, vec![
      Field {name: "xyz".to_owned(), type_id: TYPE_INT, is_array: true, value: 1},
    ]),
  ]));
  assert_eq!(schema.encode(), schema_bytes);
}

/// This type holds dynamic Kiwi data.
///
/// Values can represent anything in a Kiwi schema and can be converted to and
/// from byte arrays using the corresponding [Schema](struct.Schema.html).
/// Enums and field names are stored using string slices from their Schema
/// for efficiency. This means that a Value can outlive the buffer it was parsed
/// from but can't outlive the schema.
#[derive(Clone, PartialEq)]
pub enum Value<'a> {
  Bool(bool),
  Byte(u8),
  Int(i32),
  UInt(u32),
  Float(f32),
  String(String),
  Array(Vec<Value<'a>>),
  Enum(&'a str, &'a str),
  Object(&'a str, HashMap<&'a str, Value<'a>>),
}

impl<'a> Value<'a> {
  /// A convenience method to extract the value out of a [Bool](#variant.Bool).
  /// Returns `false` for other value kinds.
  pub fn as_bool(&self) -> bool {
    match *self {
      Value::Bool(value) => value,
      _ => false,
    }
  }

  /// A convenience method to extract the value out of a [Byte](#variant.Byte).
  /// Returns `0` for other value kinds.
  pub fn as_byte(&self) -> u8 {
    match *self {
      Value::Byte(value) => value,
      _ => 0,
    }
  }

  /// A convenience method to extract the value out of an [Int](#variant.Int).
  /// Returns `0` for other value kinds.
  pub fn as_int(&self) -> i32 {
    match *self {
      Value::Int(value) => value,
      _ => 0,
    }
  }

  /// A convenience method to extract the value out of a [UInt](#variant.UInt).
  /// Returns `0` for other value kinds.
  pub fn as_uint(&self) -> u32 {
    match *self {
      Value::UInt(value) => value,
      _ => 0,
    }
  }

  /// A convenience method to extract the value out of a [Float](#variant.Float).
  /// Returns `0.0` for other value kinds.
  pub fn as_float(&self) -> f32 {
    match *self {
      Value::Float(value) => value,
      _ => 0.0,
    }
  }

  /// A convenience method to extract the value out of a [String](#variant.String).
  /// Returns `""` for other value kinds.
  pub fn as_string(&self) -> &str {
    match *self {
      Value::String(ref value) => value.as_str(),
      _ => "",
    }
  }

  /// A convenience method to extract the length out of an [Array](#variant.Array).
  /// Returns `0` for other value kinds.
  pub fn len(&self) -> usize {
    match *self {
      Value::Array(ref values) => values.len(),
      _ => 0,
    }
  }

  /// A convenience method to append to an [Array](#variant.Array). Does
  /// nothing for other value kinds.
  pub fn push(&mut self, value: Value<'a>) {
    if let Value::Array(ref mut values) = *self {
      values.push(value);
    }
  }

  /// A convenience method to extract a field out of an [Object](#variant.Object).
  /// Returns `None` for other value kinds or if the field isn't present.
  pub fn get(&self, name: &str) -> Option<&Value<'a>> {
    match *self {
      Value::Object(_, ref fields) => fields.get(name),
      _ => None,
    }
  }

  /// A convenience method to update a field on an [Object](#variant.Object).
  /// Does nothing for other value kinds.
  pub fn set(&mut self, name: &'a str, value: Value<'a>) {
    if let Value::Object(_, ref mut fields) = *self {
      fields.insert(name, value);
    }
  }

  /// A convenience method to remove a field on an [Object](#variant.Object).
  /// Does nothing for other value kinds.
  pub fn remove(&mut self, name: &'a str) {
    if let Value::Object(_, ref mut fields) = *self {
      fields.remove(name);
    }
  }

  /// Decodes the type specified by `type_id` and `schema` from `bytes`.
  pub fn decode(schema: &'a Schema, type_id: i32, bytes: &[u8]) -> Result<Value<'a>, ()> {
    Value::decode_bb(schema, type_id, &mut ByteBuffer::new(bytes))
  }

  /// Encodes this value into an array of bytes using the provided `schema`.
  pub fn encode(&self, schema: &Schema) -> Vec<u8> {
    let mut bb = ByteBufferMut::new();
    self.encode_bb(schema, &mut bb);
    bb.data()
  }

  /// Decodes the type specified by `type_id` and `schema` from `bb` starting
  /// at the current index. After this function returns, the current index will
  /// be advanced by the amount of data that was successfully parsed. This is
  /// mainly useful as a helper routine for [decode](#method.decode), which you
  /// probably want to use instead.
  pub fn decode_bb(schema: &'a Schema, type_id: i32, bb: &mut ByteBuffer) -> Result<Value<'a>, ()> {
    match type_id {
      TYPE_BOOL => { Ok(Value::Bool(bb.read_bool()?)) },
      TYPE_BYTE => { Ok(Value::Byte(bb.read_byte()?)) },
      TYPE_INT => { Ok(Value::Int(bb.read_var_int()?)) },
      TYPE_UINT => { Ok(Value::UInt(bb.read_var_uint()?)) },
      TYPE_FLOAT => { Ok(Value::Float(bb.read_var_float()?)) },
      TYPE_STRING => { Ok(Value::String(bb.read_string()?.into_owned())) },

      _ => {
        let def = &schema.defs[type_id as usize];

        match def.kind {
          DefKind::Enum => {
            if let Some(index) = def.field_value_to_index.get(&bb.read_var_uint()?) {
              Ok(Value::Enum(def.name.as_str(), def.fields[*index].name.as_str()))
            } else {
              Err(())
            }
          },

          DefKind::Struct => {
            let mut fields = HashMap::new();
            for field in &def.fields {
              fields.insert(field.name.as_str(), Value::decode_field_bb(schema, field, bb)?);
            }
            Ok(Value::Object(def.name.as_str(), fields))
          },

          DefKind::Message => {
            let mut fields = HashMap::new();
            loop {
              let value = bb.read_var_uint()?;
              if value == 0 {
                return Ok(Value::Object(def.name.as_str(), fields));
              }
              if let Some(index) = def.field_value_to_index.get(&value) {
                let field = &def.fields[*index];
                fields.insert(field.name.as_str(), Value::decode_field_bb(schema, field, bb)?);
              } else {
                return Err(());
              }
            }
          },
        }
      },
    }
  }

  /// Decodes the field specified by `field` and `schema` from `bb` starting
  /// at the current index. This is used by [decode_bb](#method.decode_bb) but
  /// may also be useful by itself.
  pub fn decode_field_bb(schema: &'a Schema, field: &Field, bb: &mut ByteBuffer) -> Result<Value<'a>, ()> {
    if field.is_array {
      let len = bb.read_var_uint()? as usize;
      let mut array = Vec::with_capacity(len);
      for _ in 0..len {
        array.push(Value::decode_bb(schema, field.type_id, bb)?);
      }
      Ok(Value::Array(array))
    } else {
      Value::decode_bb(schema, field.type_id, bb)
    }
  }

  /// Encodes the current value to the end of `bb` using the provided `schema`.
  /// This is mainly useful as a helper routine for [encode](#method.encode),
  /// which you probably want to use instead.
  pub fn encode_bb(&self, schema: &Schema, bb: &mut ByteBufferMut) {
    match *self {
      Value::Bool(value) => bb.write_byte(if value { 1 } else { 0 }),
      Value::Byte(value) => bb.write_byte(value),
      Value::Int(value) => bb.write_var_int(value),
      Value::UInt(value) => bb.write_var_uint(value),
      Value::Float(value) => bb.write_var_float(value),
      Value::String(ref value) => bb.write_string(value.as_str()),

      Value::Array(ref values) => {
        bb.write_var_uint(values.len() as u32);
        for value in values {
          value.encode_bb(schema, bb);
        }
        return;
      },

      Value::Enum(name, value) => {
        let def = &schema.defs[*schema.def_name_to_index.get(name).unwrap()];
        let index = *def.field_name_to_index.get(value).unwrap();
        bb.write_var_uint(def.fields[index].value);
      },

      Value::Object(name, ref fields) => {
        let def = &schema.defs[*schema.def_name_to_index.get(name).unwrap()];
        match def.kind {
          DefKind::Enum => panic!(),
          DefKind::Struct => {
            for field in &def.fields {
              fields.get(field.name.as_str()).unwrap().encode_bb(schema, bb);
            }
          },
          DefKind::Message => {
            // Loop over all fields to ensure consistent encoding order
            for field in &def.fields {
              if let Some(value) = fields.get(field.name.as_str()) {
                bb.write_var_uint(field.value);
                value.encode_bb(schema, bb);
              }
            }
            bb.write_byte(0);
          },
        }
      },
    }
  }
}

impl<'a> Index<usize> for Value<'a> {
  type Output = Value<'a>;

  /// A convenience method that adds support for `self[index]` expressions.
  /// It will panic if this value isn't an [Array](#variant.Array) or if the
  /// provided index is out of bounds.
  fn index(&self, index: usize) -> &Value<'a> {
    match *self {
      Value::Array(ref values) => &values[index],
      _ => panic!(),
    }
  }
}

impl<'a> fmt::Debug for Value<'a> {
  fn fmt(&self, f: &mut fmt::Formatter) -> Result<(), fmt::Error> {
    match *self {
      Value::Bool(value) => value.fmt(f),
      Value::Byte(value) => value.fmt(f),
      Value::Int(value) => value.fmt(f),
      Value::UInt(value) => value.fmt(f),
      Value::Float(value) => value.fmt(f),
      Value::String(ref value) => value.fmt(f),
      Value::Array(ref values) => values.fmt(f),
      Value::Enum(name, ref value) => write!(f, "{}::{}", name, value),

      Value::Object(name, ref fields) => {
        let mut keys: Vec<_> = fields.keys().collect();
        let mut first = true;
        keys.sort();
        write!(f, "{} {{", name)?;

        for key in keys {
          if first {
            first = false;
          } else {
            write!(f, ", ")?;
          }
          write!(f, "{}: {:?}", key, fields[key])?;
        }

        write!(f, "}}")
      },
    }
  }
}

#[test]
fn value_basic() {
  let value = Value::Array(vec![
    Value::Bool(true),
    Value::Byte(255),
    Value::Int(-1),
    Value::UInt(1),
    Value::Float(0.5),
    Value::String("abc".to_owned()),
    Value::Enum("Foo", "FOO"),
    Value::Object("Obj", {
      let mut map = HashMap::new();
      map.insert("key1", Value::String("value1".to_owned()));
      map.insert("key2", Value::String("value2".to_owned()));
      map
    }),
  ]);

  assert_eq!(value.len(), 8);

  assert_eq!(value[0], Value::Bool(true));
  assert_eq!(value[1], Value::Byte(255));
  assert_eq!(value[2], Value::Int(-1));
  assert_eq!(value[3], Value::UInt(1));
  assert_eq!(value[4], Value::Float(0.5));
  assert_eq!(value[5], Value::String("abc".to_owned()));
  assert_eq!(value[6], Value::Enum("Foo", "FOO"));
  assert_eq!(value[7], Value::Object("Obj", {
    let mut map = HashMap::new();
    map.insert("key1", Value::String("value1".to_owned()));
    map.insert("key2", Value::String("value2".to_owned()));
    map
  }));

  assert_eq!(value[0].as_bool(), true);
  assert_eq!(value[1].as_byte(), 255);
  assert_eq!(value[2].as_int(), -1);
  assert_eq!(value[3].as_uint(), 1);
  assert_eq!(value[4].as_float(), 0.5);
  assert_eq!(value[5].as_string(), "abc");
  assert_eq!(value.get("key1"), None);
  assert_eq!(value[7].get("key1"), Some(&Value::String("value1".to_owned())));

  assert_eq!(format!("{:?}", value), "[true, 255, -1, 1, 0.5, \"abc\", Foo::FOO, Obj {key1: \"value1\", key2: \"value2\"}]");
}

#[test]
fn value_push() {
  let mut value = Value::Array(vec![]);
  assert_eq!(value.len(), 0);

  value.push(Value::Int(123));
  assert_eq!(value.len(), 1);
  assert_eq!(value[0], Value::Int(123));

  value.push(Value::Int(456));
  assert_eq!(value.len(), 2);
  assert_eq!(value[0], Value::Int(123));
  assert_eq!(value[1], Value::Int(456));
}

#[test]
fn value_set() {
  let mut value = Value::Object("Foo", HashMap::new());
  assert_eq!(value.get("x"), None);

  value.set("x", Value::Int(123));
  assert_eq!(value.get("x"), Some(&Value::Int(123)));

  value.set("y", Value::Int(456));
  assert_eq!(value.get("x"), Some(&Value::Int(123)));
  assert_eq!(value.get("y"), Some(&Value::Int(456)));

  value.set("x", Value::Int(789));
  assert_eq!(value.get("x"), Some(&Value::Int(789)));
  assert_eq!(value.get("y"), Some(&Value::Int(456)));
}

#[test]
fn value_remove() {
  let mut value = Value::Object("Foo", HashMap::new());
  assert_eq!(value.get("x"), None);

  value.set("x", Value::Int(123));
  assert_eq!(value.get("x"), Some(&Value::Int(123)));

  value.set("y", Value::Int(456));
  assert_eq!(value.get("x"), Some(&Value::Int(123)));
  assert_eq!(value.get("y"), Some(&Value::Int(456)));

  value.remove("x");
  assert_eq!(value.get("x"), None);
  assert_eq!(value.get("y"), Some(&Value::Int(456)));

  value.remove("y");
  assert_eq!(value.get("x"), None);
  assert_eq!(value.get("y"), None);
}

#[test]
fn value_encode_and_decode() {
  let schema = Schema::new(vec![
    Def::new("Enum".to_owned(), DefKind::Enum, vec![
      Field {name: "FOO".to_owned(), type_id: 0, is_array: false, value: 100},
      Field {name: "BAR".to_owned(), type_id: 0, is_array: false, value: 200},
    ]),

    Def::new("Struct".to_owned(), DefKind::Struct, vec![
      Field {name: "v_enum".to_owned(), type_id: 0, is_array: true, value: 0},
      Field {name: "v_message".to_owned(), type_id: 2, is_array: false, value: 0},
    ]),

    Def::new("Message".to_owned(), DefKind::Message, vec![
      Field {name: "v_bool".to_owned(), type_id: TYPE_BOOL, is_array: false, value: 1},
      Field {name: "v_byte".to_owned(), type_id: TYPE_BYTE, is_array: false, value: 2},
      Field {name: "v_int".to_owned(), type_id: TYPE_INT, is_array: false, value: 3},
      Field {name: "v_uint".to_owned(), type_id: TYPE_UINT, is_array: false, value: 4},
      Field {name: "v_float".to_owned(), type_id: TYPE_FLOAT, is_array: false, value: 5},
      Field {name: "v_string".to_owned(), type_id: TYPE_STRING, is_array: false, value: 6},
      Field {name: "v_enum".to_owned(), type_id: 0, is_array: false, value: 7},
      Field {name: "v_struct".to_owned(), type_id: 1, is_array: false, value: 8},
      Field {name: "v_message".to_owned(), type_id: 2, is_array: false, value: 9},

      Field {name: "a_bool".to_owned(), type_id: TYPE_BOOL, is_array: true, value: 10},
      Field {name: "a_byte".to_owned(), type_id: TYPE_BYTE, is_array: true, value: 11},
      Field {name: "a_int".to_owned(), type_id: TYPE_INT, is_array: true, value: 12},
      Field {name: "a_uint".to_owned(), type_id: TYPE_UINT, is_array: true, value: 13},
      Field {name: "a_float".to_owned(), type_id: TYPE_FLOAT, is_array: true, value: 14},
      Field {name: "a_string".to_owned(), type_id: TYPE_STRING, is_array: true, value: 15},
      Field {name: "a_enum".to_owned(), type_id: 0, is_array: true, value: 16},
      Field {name: "a_struct".to_owned(), type_id: 1, is_array: true, value: 17},
      Field {name: "a_message".to_owned(), type_id: 2, is_array: true, value: 18},
    ]),
  ]);

  assert_eq!(Value::decode(&schema, TYPE_BOOL, &[0]), Ok(Value::Bool(false)));
  assert_eq!(Value::decode(&schema, TYPE_BOOL, &[1]), Ok(Value::Bool(true)));
  assert_eq!(Value::decode(&schema, TYPE_BOOL, &[2]), Err(()));
  assert_eq!(Value::decode(&schema, TYPE_BYTE, &[255]), Ok(Value::Byte(255)));
  assert_eq!(Value::decode(&schema, TYPE_INT, &[1]), Ok(Value::Int(-1)));
  assert_eq!(Value::decode(&schema, TYPE_UINT, &[1]), Ok(Value::UInt(1)));
  assert_eq!(Value::decode(&schema, TYPE_FLOAT, &[126, 0, 0, 0]), Ok(Value::Float(0.5)));
  assert_eq!(Value::decode(&schema, TYPE_STRING, &[240, 159, 141, 149, 0]), Ok(Value::String("🍕".to_owned())));
  assert_eq!(Value::decode(&schema, 0, &[0]), Err(()));
  assert_eq!(Value::decode(&schema, 0, &[100]), Ok(Value::Enum("Enum", "FOO")));
  assert_eq!(Value::decode(&schema, 0, &[200, 1]), Ok(Value::Enum("Enum", "BAR")));

  assert_eq!(Value::Bool(false).encode(&schema), [0]);
  assert_eq!(Value::Bool(true).encode(&schema), [1]);
  assert_eq!(Value::Byte(255).encode(&schema), [255]);
  assert_eq!(Value::Int(-1).encode(&schema), [1]);
  assert_eq!(Value::UInt(1).encode(&schema), [1]);
  assert_eq!(Value::Float(0.5).encode(&schema), [126, 0, 0, 0]);
  assert_eq!(Value::String("🍕".to_owned()).encode(&schema), [240, 159, 141, 149, 0]);
  assert_eq!(Value::Enum("Enum", "FOO").encode(&schema), [100]);
  assert_eq!(Value::Enum("Enum", "BAR").encode(&schema), [200, 1]);

  fn insert<'a>(mut map: HashMap<&'a str, Value<'a>>, key: &'a str, value: Value<'a>) -> HashMap<&'a str, Value<'a>> {
    map.insert(key, value);
    map
  }

  let empty_struct = Value::Object("Struct", insert(insert(HashMap::new(),
    "v_enum", Value::Array(vec![])),
    "v_message", Value::Object("Message", HashMap::new()))
  );

  assert_eq!(Value::decode(&schema, 1, &[0, 0]), Ok(empty_struct.clone()));
  assert_eq!(empty_struct.encode(&schema), [0, 0]);

  let full_struct = Value::Object("Struct", insert(insert(HashMap::new(),
    "v_enum", Value::Array(vec![
      Value::Enum("Enum", "FOO"),
      Value::Enum("Enum", "BAR"),
    ])),
    "v_message", Value::Object("Message", insert(HashMap::new(),
      "v_string", Value::String("🍕".to_owned()))
    ))
  );

  assert_eq!(Value::decode(&schema, 1, &[2, 100, 200, 1, 6, 240, 159, 141, 149, 0, 0]), Ok(full_struct.clone()));
  assert_eq!(full_struct.encode(&schema), [2, 100, 200, 1, 6, 240, 159, 141, 149, 0, 0]);

  assert_eq!(Value::Object("Message", insert(HashMap::new(), "v_bool", Value::Bool(false))).encode(&schema), [1, 0, 0]);
  assert_eq!(Value::Object("Message", insert(HashMap::new(), "v_bool", Value::Bool(true))).encode(&schema), [1, 1, 0]);
  assert_eq!(Value::Object("Message", insert(HashMap::new(), "v_byte", Value::Byte(255))).encode(&schema), [2, 255, 0]);
  assert_eq!(Value::Object("Message", insert(HashMap::new(), "v_int", Value::Int(-1))).encode(&schema), [3, 1, 0]);
  assert_eq!(Value::Object("Message", insert(HashMap::new(), "v_uint", Value::UInt(1))).encode(&schema), [4, 1, 0]);
  assert_eq!(Value::Object("Message", insert(HashMap::new(), "v_float", Value::Float(0.0))).encode(&schema), [5, 0, 0]);
  assert_eq!(Value::Object("Message", insert(HashMap::new(), "v_string", Value::String("".to_owned()))).encode(&schema), [6, 0, 0]);
  assert_eq!(Value::Object("Message", insert(HashMap::new(), "v_enum", Value::Enum("Enum", "FOO"))).encode(&schema), [7, 100, 0]);
  assert_eq!(Value::Object("Message", insert(HashMap::new(), "v_struct", empty_struct.clone())).encode(&schema), [8, 0, 0, 0]);
  assert_eq!(Value::Object("Message", insert(HashMap::new(), "v_message", Value::Object("Message", HashMap::new()))).encode(&schema), [9, 0, 0]);

  assert_eq!(Value::decode(&schema, 2, &[1, 0, 0]), Ok(Value::Object("Message", insert(HashMap::new(), "v_bool", Value::Bool(false)))));
  assert_eq!(Value::decode(&schema, 2, &[1, 1, 0]), Ok(Value::Object("Message", insert(HashMap::new(), "v_bool", Value::Bool(true)))));
  assert_eq!(Value::decode(&schema, 2, &[2, 255, 0]), Ok(Value::Object("Message", insert(HashMap::new(), "v_byte", Value::Byte(255)))));
  assert_eq!(Value::decode(&schema, 2, &[3, 1, 0]), Ok(Value::Object("Message", insert(HashMap::new(), "v_int", Value::Int(-1)))));
  assert_eq!(Value::decode(&schema, 2, &[4, 1, 0]), Ok(Value::Object("Message", insert(HashMap::new(), "v_uint", Value::UInt(1)))));
  assert_eq!(Value::decode(&schema, 2, &[5, 0, 0]), Ok(Value::Object("Message", insert(HashMap::new(), "v_float", Value::Float(0.0)))));
  assert_eq!(Value::decode(&schema, 2, &[6, 0, 0]), Ok(Value::Object("Message", insert(HashMap::new(), "v_string", Value::String("".to_owned())))));
  assert_eq!(Value::decode(&schema, 2, &[7, 100, 0]), Ok(Value::Object("Message", insert(HashMap::new(), "v_enum", Value::Enum("Enum", "FOO")))));
  assert_eq!(Value::decode(&schema, 2, &[8, 0, 0, 0]), Ok(Value::Object("Message", insert(HashMap::new(), "v_struct", empty_struct.clone()))));
  assert_eq!(Value::decode(&schema, 2, &[9, 0, 0]), Ok(Value::Object("Message", insert(HashMap::new(), "v_message", Value::Object("Message", HashMap::new())))));
}

// This test case is for a bug where rustc was silently inferring an incorrect
// lifetime. This is the specific error:
//
//   error[E0597]: `value` does not live long enough
//       --> src/lib.rs:1307:40
//        |
//   1307 |     if let Some(Value::Array(items)) = value.get("items") {
//        |                                        ^^^^^ borrowed value does not live long enough
//   ...
//   1312 |   }
//        |   - borrowed value only lives until here
//        |
//        = note: borrowed value must be valid for the static lifetime...
//
// The fix was to change this:
//
//   pub fn get(&self, name: &str) -> Option<&Value> {
//
// Into this:
//
//   pub fn get(&self, name: &str) -> Option<&Value<'a>> {
//
#[test]
fn value_get_bad_lifetime_inference_in_rustc() {
  fn use_item<'a>(_: &'a Value<'static>) {
  }

  fn use_items(value: Value<'static>) {
    if let Some(Value::Array(items)) = value.get("items") {
      for item in items {
        use_item(item);
      }
    }
  }

  use_items(Value::Array(vec![]));
}
