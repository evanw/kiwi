//! This is a Rust library with some helper routines for parsing files in the Kiwi
//! serialization format. Note that it's currently incomplete. See
//! https://github.com/evanw/kiwi for documentation about the format.

use std::f32;
use std::str;
use std::collections::HashMap;

/// A Kiwi byte buffer meant for reading.
///
/// Example usage:
///
/// ```
/// let mut bb = kiwi_schema::ByteBuffer::new(&[240, 159, 141, 149, 0, 133, 242, 210, 237]);
/// assert_eq!(bb.read_string(), Ok("🍕"));
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
  pub fn read_string(&mut self) -> Result<&'a str, ()> {
    let start = self.index;

    while self.index < self.data.len() {
      if self.data[self.index] == 0 {
        self.index += 1;
        return Ok(str::from_utf8(&self.data[start..self.index - 1]).or(Err(()))?);
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
  assert_eq!(try(&[0]), Ok(""));
  assert_eq!(try(&[97]), Err(()));
  assert_eq!(try(&[97, 0]), Ok("a"));
  assert_eq!(try(&[97, 98, 99, 0]), Ok("abc"));
  assert_eq!(try(&[240, 159, 141, 149, 0]), Ok("🍕"));
}

#[test]
fn read_sequence() {
  let mut bb = ByteBuffer::new(&[0, 133, 242, 210, 237, 240, 159, 141, 149, 0, 149, 154, 239, 58]);
  assert_eq!(bb.read_var_float(), Ok(0.0));
  assert_eq!(bb.read_var_float(), Ok(123.456));
  assert_eq!(bb.read_string(), Ok("🍕"));
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
#[derive(Debug)]
pub struct Field {
  /// The name of this field from the textual Kiwi schema.
  pub name: String,

  /// For user-defined types, this is the index of the [Def](struct.Def.html)
  /// in the `defs` array of the [BinarySchema](struct.BinarySchema.html).
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

/// Represents a single definition in a [BinarySchema](struct.BinarySchema.html).
/// Kiwi enums, structs, and messages are all represented using this object.
#[derive(Debug)]
pub struct Def {
  /// The name of this field from the textual Kiwi schema.
  pub name: String,

  /// The index of this Def in the `defs` field of the parent
  /// [BinarySchema](struct.BinarySchema.html). This is used as the type
  /// identifier for any fields with this type.
  pub index: i32,

  /// Either [Enum](enum.DefKind.html#variant.Enum),
  /// [Struct](enum.DefKind.html#variant.Struct), or
  /// [Message](enum.DefKind.html#variant.Message).
  pub kind: DefKind,

  /// All fields in this definition. The order matters for struct definitions.
  pub fields: Vec<Field>,

  /// Maps the `value` member of each [Field](struct.Field.html) in the `fields`
  /// array to its index in that array. This is helpful when decoding a field
  /// to be able to quickly get to the field metadata.
  pub field_value_to_index: HashMap<u32, usize>,
}

impl Def {
  /// Returns the [Field](struct.Field.html) with the provided name if one exists.
  pub fn field(&self, name: &str) -> Option<&Field> {
    for field in &self.fields {
      if field.name == name {
        return Some(&field)
      }
    }
    None
  }
}

/// Holds the parsed contents of an encoded Kiwi schema.
///
/// Example usage:
///
/// ```
/// // This is the encoding of the Kiwi schema "message ABC { int[] xyz = 1; }"
/// let schema_bytes = [1, 65, 66, 67, 0, 2, 1, 120, 121, 122, 0, 5, 1, 1];
/// let schema = kiwi_schema::BinarySchema::parse(&schema_bytes).unwrap();
///
/// let def = schema.def("ABC").unwrap();
/// assert_eq!(def.kind, kiwi_schema::DefKind::Message);
///
/// let field = def.field("xyz").unwrap();
/// assert_eq!(field.type_id, kiwi_schema::TYPE_INT);
/// assert_eq!(field.is_array, true);
/// assert_eq!(field.value, 1);
/// ```
///
/// This object is mainly useful as part of a decoder or encoder. It doesn't
/// implement decoding or encoding itself.
#[derive(Debug)]
pub struct BinarySchema {
  pub defs: Vec<Def>,
}

impl BinarySchema {
  /// Parses a Kiwi schema encoded in the binary format and returns the parsed
  /// schema if successful. A textual schema can be compiled into a binary
  /// schema using the command-line tools:
  ///
  /// ```text
  /// kiwic --schema example.kiwi --binary example.bkiwi
  /// ```
  pub fn parse(bytes: &[u8]) -> Result<BinarySchema, ()> {
    let mut defs = Vec::new();
    let mut bb = ByteBuffer::new(bytes);
    let definition_count = bb.read_var_uint()?;

    for _ in 0..definition_count {
      let name = bb.read_string()?.to_owned();
      let kind = match bb.read_byte()? {
        DEF_ENUM => DefKind::Enum,
        DEF_STRUCT => DefKind::Struct,
        DEF_MESSAGE => DefKind::Message,
        _ => return Err(()),
      };
      let field_count = bb.read_var_uint()?;
      let mut fields = Vec::new();
      let mut field_value_to_index = HashMap::new();

      for _ in 0..field_count {
        let name = bb.read_string()?.to_owned();
        let type_id = bb.read_var_int()?;
        let is_array = bb.read_bool()?;
        let value = bb.read_var_uint()?;
        field_value_to_index.insert(value, fields.len());
        fields.push(Field{name, type_id, is_array, value});
      }

      let index = defs.len() as i32;
      defs.push(Def{name, index, kind, fields, field_value_to_index});
    }

    Ok(BinarySchema{defs})
  }

  /// Returns the [Def](struct.Def.html) with the provided name if one exists.
  pub fn def(&self, name: &str) -> Option<&Def> {
    for def in &self.defs {
      if def.name == name {
        return Some(&def)
      }
    }
    None
  }

  /// Advances the current index of the provided [ByteBuffer](struct.ByteBuffer.html)
  /// by the size of a field with the provided type information. The Kiwi format
  /// doesn't support seeking around to arbitrary points (it must be read from
  /// start to end) so this method is helpful when you need to to skip past
  /// unimportant fields.
  pub fn skip(&self, bb: &mut ByteBuffer, type_id: i32, is_array: bool) -> Result<(), ()> {
    if is_array {
      let len = bb.read_var_uint()? as usize;
      for _ in 0..len {
        self.skip(bb, type_id, false)?;
      }
      return Ok(())
    }

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
            let value = bb.read_var_uint()?;
            if !def.field_value_to_index.contains_key(&value) {
              return Err(());
            }
          },

          DefKind::Struct => {
            for field in &def.fields {
              self.skip(bb, field.type_id, field.is_array)?;
            }
          },

          DefKind::Message => {
            loop {
              let value = bb.read_var_uint()?;
              if value == 0 {
                break;
              }
              if let Some(index) = def.field_value_to_index.get(&value) {
                let field = &def.fields[*index];
                self.skip(bb, field.type_id, field.is_array)?;
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
}
