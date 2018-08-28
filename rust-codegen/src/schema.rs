//! Module to decode and encode kiwi Schema headers
//!
//! Kiwi allows for encoding schemas alongside messages.
//! This module provides methods of validating and creating those Schemas.

use std::collections::HashMap;
use reader::BytesReader;
use writer::Writer;
use errors::{Error, Result};

/// Field of kind bool
pub const TYPE_BOOL: i32 = -1;
/// Field of kind byte
pub const TYPE_BYTE: i32 = -2;
/// Field of kind int32
pub const TYPE_INT: i32 = -3;
/// Field of kind uint32
pub const TYPE_UINT: i32 = -4;
/// Field of kind float
pub const TYPE_FLOAT: i32 = -5;
/// Field of kind String
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

/// DefKind is an enum of all posible structure types within a Schema,
/// Enum, Struct, Message
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


/// Def of type Enum
pub const DEF_ENUM: u8 = 0;
/// Def of type Struct
pub const DEF_STRUCT: u8 = 1;
/// Def of type Message
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

    /// Maps the `value` members of each [Field](struct.Field.html) in
    /// the `fields` array to its index in that array. This is helpful when
    /// decoding and encoding a field to be able to quickly get to the field
    /// metadata.
    pub field_value_to_index: HashMap<u32, usize>,

    /// Maps the `name` members of each [Field](struct.Field.html) in
    /// the `fields` array to its index in that array. This is helpful when
    /// decoding and encoding a field to be able to quickly get to the field
    /// metadata.
    pub field_name_to_index: HashMap<String, usize>,
}

impl Def {

    /// Define a new structure in schema with name, kind Defkind, and fields
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
/// let schema = quick_kiwi::schema::Schema::decode(&schema_bytes).unwrap();
///
/// let def = schema.def("ABC").unwrap();
/// assert_eq!(def.kind, quick_kiwi::schema::DefKind::Message);
///
/// let field = def.field("xyz").unwrap();
/// assert_eq!(field.type_id, quick_kiwi::schema::TYPE_INT);
/// assert_eq!(field.is_array, true);
/// assert_eq!(field.value, 1);
/// ```
#[derive(Debug, PartialEq)]
pub struct Schema {
    /// List of definitions [Def](struct.Def.html)
    pub defs: Vec<Def>,

    /// Maps the `name` member of each [Def](struct.Def.html) in the `defs` array
    /// to its index in that array. This is helpful when decoding and encoding a
    /// field to be able to quickly get to the field metadata.
    pub def_name_to_index: HashMap<String, usize>,
}

impl Schema {

    /// Constructs a new Schema from a vec of Def definitions
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
    pub fn decode(bytes: &[u8]) -> Result<Schema> {
        let mut defs = Vec::new();
        let mut bb = BytesReader::from_bytes(bytes);
        let definition_count = bb.read_uint32(bytes)?;

        for _ in 0..definition_count {
            let name = bb.read_string(bytes)?.into_owned();
            let kind = match bb.read_u8(bytes)? {
                DEF_ENUM => DefKind::Enum,
                DEF_STRUCT => DefKind::Struct,
                DEF_MESSAGE => DefKind::Message,
                _ => return Err(Error::Message("Invalid schema field kind".to_string())),
            };
            let field_count = bb.read_uint32(bytes)?;
            let mut fields = Vec::new();

            for _ in 0..field_count {
                let name = bb.read_string(bytes)?.into_owned();
                let type_id = bb.read_int32(bytes)?;
                let is_array = bb.read_bool(bytes)?;
                let value = bb.read_uint32(bytes)?;
                fields.push(Field {name, type_id, is_array, value});
            }

            defs.push(Def::new(name, kind, fields));
        }

        Ok(Schema::new(defs))
    }

    /// The opposite of [decode](#method.decode). Turns this schema back into a
    /// binary file.
    pub fn encode(&self) -> Vec<u8> {
        let mut buffer = Vec::new();
        {
            let mut bb = Writer::new(&mut buffer);
            bb.write_uint32(self.defs.len() as u32);

            for def in &self.defs {
                bb.write_string(def.name.as_str());
                bb.write_byte(match def.kind {
                    DefKind::Enum => DEF_ENUM,
                    DefKind::Struct => DEF_STRUCT,
                    DefKind::Message => DEF_MESSAGE,
                });
                bb.write_uint32(def.fields.len() as u32);

                for field in &def.fields {
                    bb.write_string(field.name.as_str());
                    bb.write_int32(field.type_id);
                    bb.write_bool(field.is_array);
                    bb.write_uint32(field.value);
                }
            }
        }

        buffer
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
    pub fn skip(&self, bb: &mut BytesReader, bytes: &[u8], type_id: i32) -> Result<()> {
        match type_id {
            TYPE_BOOL => { bb.read_bool(bytes)?; },
            TYPE_BYTE => { bb.read_u8(bytes)?; },
            TYPE_INT => { bb.read_int32(bytes)?; },
            TYPE_UINT => { bb.read_uint32(bytes)?; },
            TYPE_FLOAT => { bb.read_float(bytes)?; },
            TYPE_STRING => { bb.read_string(bytes)?; },

            _ => {
                let def = &self.defs[type_id as usize];

                match def.kind {
                    DefKind::Enum => {
                        if !def.field_value_to_index.contains_key(&bb.read_uint32(bytes)?) {
                            return Err(Error::Message("Could not locate Enum".to_string()));
                        }
                    },

                    DefKind::Struct => {
                        for field in &def.fields {
                            self.skip_field(bb, bytes, field)?;
                        }
                    },

                    DefKind::Message => {
                        loop {
                            let value = bb.read_uint32(bytes)?;
                            if value == 0 {
                                break;
                            }
                            if let Some(index) = def.field_value_to_index.get(&value) {
                                self.skip_field(bb, bytes, &def.fields[*index])?;
                            } else {
                                return Err(Error::Message("Could not locate Message".to_string()));
                            }
                        }
                    },
                }
            },
        }

        Ok(())
    }

    /// Advances the current index of the provided [ByteBuffer](struct.ByteBuffer.html)
    /// by the size of the provided field. This is used by [skip](#method.skip)
    /// but may also be useful by itself.
    pub fn skip_field(&self, bb: &mut BytesReader, bytes: &[u8], field: &Field) -> Result<()> {
        if field.is_array {
            let len = bb.read_uint32(bytes)? as usize;
            for _ in 0..len {
                self.skip(bb, bytes, field.type_id)?;
            }
        } else {
            self.skip(bb, bytes, field.type_id)?;
        }
        Ok(())
    }
}
