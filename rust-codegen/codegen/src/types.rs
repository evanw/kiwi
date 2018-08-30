use std::io::{BufReader, BufWriter, Read, Write};
use std::path::{Path, PathBuf};
use std::fs::File;
use std::collections::BTreeSet;

use errors::{Error, Result};
use parser::file_descriptor;
use keywords::sanitize_keyword;

use std::str;
use heck::SnakeCase;

pub fn snake_case(s: &String) -> String {
    let s = str::replace(&s, "ID", "Id");
    let s = str::replace(&s, "GUId", "Guid");
    s.to_snake_case()
}

#[derive(Debug, Clone)]
pub enum Frequency {
    Optional,
    Repeated,
    Required,
}

#[derive(PartialOrd, Ord, Clone, PartialEq, Eq, Hash, Debug)]
pub enum Derives {
    Copy,
    Hash
}

impl Derives {
    fn to_string(&self) -> String {
        match *self {
            // TODO: add serde json serialization option here
            Derives::Copy => "Copy".to_string(),
            Derives::Hash => "Hash, PartialOrd, Ord, Eq".to_string(),
        }
    }
}

fn get_modules(module: &str, imported: bool, desc: &FileDescriptor) -> String {
    let skip = if desc.package.is_empty() && !imported {1} else {0};
    module
        .split('.')
        .filter(|p| !p.is_empty())
        .skip(skip)
        .map(|p| format!("{}::", p))
        .collect()
}

#[derive(Debug, Clone, PartialEq, Eq, Hash)]
pub enum FieldType {
    Int32,
    Uint32,
    Bool,
    String_,
    Bytes,
    Byte,
    Enum(String),
    Message(String),
    ReferenceType(String),
    Float
}

impl FieldType {
    fn has_cow(&self) -> bool {
        match *self {
            FieldType::Bytes | FieldType::String_ => true,
            _ => false,
        }
    }

    fn get_derives(&self, desc: &FileDescriptor) -> BTreeSet<Derives> {
        match *self {
            FieldType::Int32 => [Derives::Copy, Derives::Hash].iter().cloned().collect(),
            FieldType::Uint32 => [Derives::Copy, Derives::Hash].iter().cloned().collect(),
            FieldType::Bool => [Derives::Copy, Derives::Hash].iter().cloned().collect(),
            FieldType::Byte => [Derives::Copy, Derives::Hash].iter().cloned().collect(),
            FieldType::Enum(_) => [Derives::Copy, Derives::Hash].iter().cloned().collect(),
            FieldType::Float => BTreeSet::new(),
            FieldType::String_ => [Derives::Hash].iter().cloned().collect(),
            FieldType::Bytes => [Derives::Hash].iter().cloned().collect(),
            FieldType::Message(_) => match self.find_message(&desc.messages) {
                Some(m) => m.get_derives(desc),
                None => BTreeSet::new(),
            },
            FieldType::ReferenceType(_) => [Derives::Copy, Derives::Hash].iter().cloned().collect()
        }
    }

    fn proto_type(&self) -> &str {
        match *self {
            FieldType::Int32 => "int32",
            FieldType::Uint32 => "uint32",
            FieldType::Bool => "bool",
            FieldType::Enum(_) => "enum",
            FieldType::Float => "float",
            FieldType::String_ => "string",
            FieldType::Byte => "u8",
            FieldType::Bytes => "bytes",
            FieldType::Message(_) => "message",
            FieldType::ReferenceType(_) => "ref"
        }
    }

    /// Searches for message corresponding to the current type
    fn find_message<'a, 'b>(&'a self, msgs: &'b [Message]) -> Option<&'b Message> {
        match *self {
            FieldType::ReferenceType(ref m) |
            FieldType::Message(ref m) => {
                let mut found = match m.rfind('.') {
                    Some(p) => {
                        let package = &m[..p];
                        let name = &m[(p + 1)..];
                        msgs.iter().find(|m| m.package == package && m.name == name)
                    }
                    None => msgs.iter().find(|m2| m2.name == &m[..]),
                };

                found
            }
            _ => None,
        }
    }


    /// Searches for enum corresponding to the current type
    pub fn find_enum<'a, 'b>(
        &'a self,
        enums: &'b [Enumerator],
    ) -> Option<&'b Enumerator> {
        match *self {
            FieldType::ReferenceType(ref m) |
            FieldType::Enum(ref m) => {
                let mut found = match m.rfind('.') {
                    Some(p) => {
                        let package = &m[..p];
                        let name = &m[(p + 1)..];
                        enums
                            .iter()
                            .find(|m| m.package == package && m.name == name)
                    }
                    None => enums.iter().find(|m2| m2.name == &m[..]),
                };

                found
            }
            _ => None,
        }
    }

    fn has_lifetime(&self, desc: &FileDescriptor) -> bool {
        match *self {
            FieldType::String_ | FieldType::Bytes => true, // Cow<[u8]>
            FieldType::Message(_) => self.find_message(&desc.messages)
                .map_or(false, |m| m.has_lifetime(desc)),
            _ => false,
        }
    }

    fn rust_type(&self, desc: &FileDescriptor) -> Result<String> {
        Ok(match *self {
            FieldType::Int32 => "i32".to_string(),
            FieldType::Uint32 => "u32".to_string(),
            FieldType::Float => "f32".to_string(),
            FieldType::Byte => "u8".to_string(),
            FieldType::String_ => "Cow<'a, str>".to_string() ,
            FieldType::Bytes => "Cow<'a, [u8]>".to_string() ,
            FieldType::Bool => "bool".to_string(),
            FieldType::Enum(ref e) => match self.find_enum(&desc.enums) {
                Some(e) => format!("{}{}", e.get_modules(desc), e.name),
                None => return Err(Error::EnumNotFound(e.to_string())),
            },
            FieldType::Message(ref msg) => match self.find_message(&desc.messages) {
                Some(m) => {
                    let lifetime = if m.has_lifetime(desc) { "<'a>" } else { "" };
                    format!("{}{}{}", m.get_modules(desc), m.name, lifetime)
                }
                None => return Err(Error::MessageNotFound(msg.to_string())),
            },
            FieldType::ReferenceType(_) => "ref".to_string()
        })
    }

    fn lazy_rust_type(&self, _desc: &FileDescriptor) -> Result<String> {
        Ok(match *self {
            FieldType::ReferenceType(_) => "ref".to_string(),
            _ => format!("&'a [u8]")
        })
    }

    fn owned_lazy_rust_type(&self, _desc: &FileDescriptor) -> Result<String> {
        Ok(match *self {
            FieldType::ReferenceType(_) => "ref".to_string(),
            _ => format!("Vec<u8>")
        })
    }


    /// Returns the relevant function to read the data, both for regular and Cow wrapped
    fn read_fn(&self, desc: &FileDescriptor) -> Result<(String, String)> {
        Ok(match *self {
            FieldType::Message(ref msg) => match self.find_message(&desc.messages) {
                Some(m) => {
                    let m = format!("r.read_message::<{}{}>(bytes)", m.get_modules(desc), m.name);
                    (m.clone(), m)
                }
                None => return Err(Error::MessageNotFound(msg.to_string())),
            },
            FieldType::String_ => {
                let m = format!("r.read_{}(bytes)", self.proto_type());
                let cow = format!("{}", m);
                (m, cow)
            },
            FieldType::Bytes => {
                let m = format!("r.read_{}(bytes)", self.proto_type());
                let cow = format!("{}.map(Cow::Borrowed)", m);
                (m, cow)
            }
            _ => {
                let m = format!("r.read_{}(bytes)", self.proto_type());
                (m.clone(), m)
            }
        })
    }

    /// Returns the relevant function to lazily read the data, both for regular and Cow wrapped
    fn lazy_read_fn(&self, desc: &FileDescriptor) -> Result<(String, String, String)> {
        Ok(match *self {
            FieldType::Message(ref msg) => match self.find_message(&desc.messages) {
                Some(m) => {
                    let ms = format!("r.lazy_read_message_slice::<{}Lazy{}>(bytes)", m.get_modules(desc), m.name);
                    let mf = format!("r.lazy_read_message::<{}Lazy{}>(bytes)", m.get_modules(desc), m.name);
                    (ms.clone(), ms, mf)
                }
                None => return Err(Error::MessageNotFound(msg.to_string())),
            },
            FieldType::String_ | FieldType::Bytes => {
                let m = format!("r.skip_{}(bytes)", self.proto_type());
                // let cow = format!("{}.map(Cow::Borrowed)", m);
                (m.clone(), m.clone(), m)
            }
            _ => {
                let m = format!("r.skip_{}(bytes)", self.proto_type());
                (m.clone(), m.clone(), m)
            }
        })
    }

    /// Returns the relevant function to read the data when deprecated, both for regular and Cow wrapped
    fn read_fn_deprecated(&self, desc: &FileDescriptor) -> Result<(String, String)> {
        Ok(match *self {
            FieldType::Message(ref msg) => match self.find_message(&desc.messages) {
                Some(m) => {
                    let m = format!("r.read_message::<{}{}>(bytes)", m.get_modules(desc), m.name);
                    (m.clone(), m)
                }
                None => return Err(Error::MessageNotFound(msg.to_string())),
            },
            FieldType::String_ => {
                let m = format!("r.read_{}(bytes)", self.proto_type());
                let cow = format!("{}", m);
                (m, cow)
            },
            FieldType::Bytes => {
                let m = format!("r.read_{}(bytes)", self.proto_type());
                let cow = format!("{}.map(Cow::Borrowed)", m);
                (m, cow)
            }
            FieldType::Enum(_) => {
                let m = format!("r.read_uint32(bytes)");
                (m.clone(), m)
            }
            _ => {
                let m = format!("r.read_{}(bytes)", self.proto_type());
                (m.clone(), m)
            }
        })
    }

    fn get_write(&self, s: &str, boxed: bool) -> String {
        match *self {
            FieldType::Enum(_) => format!("write_enum(*{} as u32)", s),

            FieldType::Int32
                | FieldType::Uint32
                | FieldType::Bool
                | FieldType::Byte
                | FieldType::Float => format!("write_{}(*{})", self.proto_type(), s),

            FieldType::String_ => format!("write_string(&**{})", s),
            FieldType::Bytes => format!("write_bytes(&**{})", s),

            FieldType::Message(_) if boxed => format!("write_message(&**{})", s),
            FieldType::Message(_) => format!("write_message({})", s),
            FieldType::ReferenceType(_) => unreachable!("Should never occur."),
        }
    }
}

#[derive(Debug, Clone)]
pub struct Field {
    pub name: String,
    pub frequency: Frequency,
    pub typ: FieldType,
    pub number: i32,
    pub default: Option<String>,
    pub boxed: bool,
    pub deprecated: bool,
    pub struct_field: bool
}

impl Field {

    fn get_derives(&self, desc: &FileDescriptor) -> BTreeSet<Derives> {
        match self.frequency {
            Frequency::Repeated => {
                let nocopy: BTreeSet<Derives> = [Derives::Hash]
                    .iter().cloned().collect();
                nocopy.intersection(&self.typ.get_derives(desc))
                    .map(|x| x.clone())
                    .collect()
            },
            _ => self.typ.get_derives(desc)
        }
    }

    /// searches if the message must be boxed
    fn is_leaf(&self, leaf_messages: &[String]) -> bool {
        match self.typ {
            FieldType::Message(ref s) => match self.frequency {
                Frequency::Repeated => true,
                _ => {
                    let typ = match s.rfind('.') {
                        Some(p) => &s[p + 1..],
                        None => &s[..],
                    };
                    leaf_messages.iter().any(|m| &*m == &typ)
                }
            },
            _ => true,
        }
    }

    fn tag(&self) -> u32 {
        self.number as u32
    }

    fn write_definition<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        write!(w, "    pub {}: ", self.name)?;
        let rust_type = self.typ.rust_type(desc)?;
        match self.frequency {
            _ if self.boxed => writeln!(w, "Option<Box<{}>>,", rust_type)?,
            Frequency::Optional if self.default.is_some() => writeln!(w, "{},", rust_type)?,
            Frequency::Optional => writeln!(w, "Option<{}>,", rust_type)?,
            Frequency::Repeated => writeln!(w, "Vec<{}>,", rust_type)?,
            Frequency::Required => writeln!(w, "{},", rust_type)?,
        }
        Ok(())
    }

    fn write_definition_lazy<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        write!(w, "    pub {}: ", self.name)?;
        let rust_type = self.typ.lazy_rust_type(desc)?;
        match self.frequency {
            Frequency::Repeated => writeln!(w, "Vec<{}>,", rust_type)?,
            Frequency::Required => writeln!(w, "{},", rust_type)?,
            Frequency::Optional => writeln!(w, "Option<{}>,", rust_type)?,
        }
        Ok(())
    }

    fn write_definition_owned_lazy<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        write!(w, "    pub {}: ", self.name)?;
        let rust_type = self.typ.owned_lazy_rust_type(desc)?;
        match self.frequency {
            Frequency::Repeated => writeln!(w, "Vec<{}>,", rust_type)?,
            Frequency::Optional => writeln!(w, "Option<{}>,", rust_type)?,
            Frequency::Required => writeln!(w, "{},", rust_type)?,
        }
        Ok(())
    }

    fn write_read_field<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        let (val, val_cow) = self.typ.read_fn(desc)?;
        let name = &self.name;
        if self.struct_field {
            write!(w, "        ")?;
            match self.frequency {
                Frequency::Required => writeln!(w, "msg.{} = {}?;", name, val_cow)?,
                Frequency::Repeated => writeln!(w, "msg.{} = r.read_packed(bytes, |r, bytes| {})?;", name, val_cow)?,
                _ => {}
            }
        } else {
            write!(w, "                Ok({}) => ", self.tag())?;
            match self.frequency {
                _ if self.boxed => writeln!(w, "msg.{} = Some(Box::new({}?)),", name, val)?,
                Frequency::Required => writeln!(w, "msg.{} = {}?,", name, val_cow)?,
                Frequency::Optional => writeln!(w, "msg.{} = Some({}?),", name, val_cow)?,
                Frequency::Repeated => writeln!(w, "msg.{} = r.read_packed(bytes, |r, bytes| {})?,", name, val_cow)?
            }
        }
        Ok(())
    }

    fn write_lazy_read_field<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        let (_, val_cow, _) = self.typ.lazy_read_fn(desc)?;
        let name = &self.name;
        if self.struct_field {
            write!(w, "        ")?;
            match self.frequency {
                Frequency::Repeated => writeln!(w, "msg.{} = r.read_packed(bytes, |r, bytes| {})?;", name, val_cow)?,
                Frequency::Required => writeln!(w, "msg.{} = {}?;", name, val_cow)?,
                _ => {}
            }
        } else {
            write!(w, "                Ok({}) => ", self.tag())?;
            match self.frequency {
                Frequency::Required => writeln!(w, "msg.{} = {}?,", name, val_cow)?,
                Frequency::Optional => writeln!(w, "msg.{} = Some({}?),", name, val_cow)?,
                Frequency::Repeated => writeln!(w, "msg.{} = r.lazy_packed(bytes, |r, bytes| {})?,", name, val_cow)?
            }
        }
        Ok(())
    }

    fn write_lazy_read_field_slice<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        let (_, val_cow, _) = self.typ.lazy_read_fn(desc)?;
        if self.struct_field {
            write!(w, "        ")?;
            match self.frequency {
                Frequency::Repeated => writeln!(w, "r.read_packed(bytes, |r, bytes| {})?;", val_cow)?,
                Frequency::Required => writeln!(w, "{}?;", val_cow)?,
                _ => {},
            }
        } else {
            write!(w, "                Ok({}) => ", self.tag())?;
            match self.frequency {
                Frequency::Repeated => writeln!(w, "{{ r.lazy_packed(bytes, |r, bytes| {})?; }},", val_cow)?,
                Frequency::Optional => writeln!(w, "{{ {}?; }},", val_cow)?,
                _ => {},
            }
        }
        Ok(())
    }


    /// We assume that all already sliced fields can be safely lazily sliced
    /// because the methods used to derive the slices ran equivalent logic
    fn write_definition_accessor_trait_lazy_primitive_field<W: Write>(&self, w: &mut W, desc: &FileDescriptor, exclude_lifetime: bool) -> Result<()> {
        let name = &self.name;
        let rust_type = self.typ.rust_type(desc)?;
        let fn_lifetime = if self.typ.has_lifetime(desc) && !exclude_lifetime { "<'a>" } else { "" };
        let self_lifetime = if self.typ.has_lifetime(desc) && !exclude_lifetime { "'a " } else { "" };
        writeln!(w, "")?;
        if self.struct_field {
            match self.frequency {
                Frequency::Repeated => writeln!(w, "    fn get_{}{}(&{}self) -> Result<Vec<{}>>;", name, fn_lifetime, self_lifetime, rust_type)? ,
                Frequency::Required => writeln!(w, "    fn get_{}{}(&{}self) -> Result<{}>;", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Optional => {}
            }
        } else {
            match self.frequency {
                Frequency::Repeated => writeln!(w, "    fn get_{}{}(&{}self) -> Result<Vec<{}>>;", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Optional => writeln!(w, "    fn get_{}{}(&{}self) -> Result<Option<{}>>;", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Required => {}
            }
        }
        Ok(())

    }

    /// Builds methods for reading individual fields on Lazy structs
    ///
    /// We do not assert that all sub fields of nested children will also be valid
    /// so field conversion to fully resolved objects must still return Result
    fn write_lazy_read_field_convert<W: Write>(&self, w: &mut W, desc: &FileDescriptor, exclude_lifetime: bool) -> Result<()> {
        let (_, val_cow) = self.typ.read_fn(desc)?;
        let name = &self.name;
        let rust_type = self.typ.rust_type(desc)?;
        let fn_lifetime = if self.typ.has_lifetime(desc) && !exclude_lifetime { "<'a>" } else { "" };
        let self_lifetime = if self.typ.has_lifetime(desc) && !exclude_lifetime { "'a " } else { "" };
        writeln!(w, "")?;
        if self.struct_field {
            match self.frequency {
                Frequency::Repeated => writeln!(w, "    fn get_{}{}(&{}self) -> Result<Vec<{}>>{{", name, fn_lifetime, self_lifetime, rust_type)? ,
                Frequency::Required => writeln!(w, "    fn get_{}{}(&{}self) -> Result<{}>{{", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Optional => {}
            }
            match self.frequency {
                Frequency::Required => {
                    writeln!(w, "        let bytes = &self.{};", name)?;
                    writeln!(w, "        let mut r = BytesReader::from_bytes(bytes);")?;
                    writeln!(w, "        return Ok({}?);", val_cow)?;
                },
                Frequency::Optional => {},
                Frequency::Repeated => {
                    writeln!(w, "        let mut result = Vec::with_capacity(self.{}.len());", name)?;
                    writeln!(w, "        for bytes in &self.{} {{", name)?;
                    writeln!(w, "            let mut r = BytesReader::from_bytes(bytes);")?;
                    writeln!(w, "            result.push({}?)", val_cow)?;
                    writeln!(w, "        }}")?;
                    writeln!(w, "        Ok(Some(result))")?;
                },
            }
        } else {
            match self.frequency {
                Frequency::Repeated => writeln!(w, "    fn get_{}{}(&{}self) -> Result<Vec<{}>>{{", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Optional => writeln!(w, "    fn get_{}{}(&{}self) -> Result<Option<{}>>{{", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Required => {}
            }
            match self.frequency {
                Frequency::Required => {},
                Frequency::Optional => {
                    writeln!(w, "        if let Some(ref bytes) = self.{} {{", name)?;
                    writeln!(w, "            let mut r = BytesReader::from_bytes(bytes);")?;
                    writeln!(w, "            return Ok(Some({}?));", val_cow)?;
                    writeln!(w, "        }}")?;
                    writeln!(w, "        Ok(None)")?;
                },
                Frequency::Repeated => {
                    writeln!(w, "        let mut result = Vec::with_capacity(self.{}.len());", name)?;
                    writeln!(w, "        for bytes in &self.{} {{", name)?;
                    writeln!(w, "            let mut r = BytesReader::from_bytes(bytes);")?;
                    writeln!(w, "            result.push({}?)", val_cow)?;
                    writeln!(w, "        }}")?;
                    writeln!(w, "        Ok(result)")?;
                }
            }
        }
        writeln!(w, "    }}")?;
        Ok(())
    }


    /// We assume that all already sliced fields can be safely lazily sliced
    /// because the methods used to derive the slices ran equivalent logic
    fn write_definition_accessor_trait_lazy_field<W: Write>(&self, w: &mut W, desc: &FileDescriptor, exclude_lifetime: bool) -> Result<()> {
        let name = &self.name;
        let rust_type = self.typ.rust_type(desc)?;
        let fn_lifetime = if self.typ.has_lifetime(desc) && !exclude_lifetime { "<'a>" } else { "" };
        let self_lifetime = if self.typ.has_lifetime(desc) && !exclude_lifetime { "'a " } else { "" };
        writeln!(w, "")?;
        if self.struct_field {
            match self.frequency {
                Frequency::Repeated => writeln!(w, "    fn get_lazy_{}{}(&{}self) -> Vec<Lazy{}>;", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Required => writeln!(w, "    fn get_lazy_{}{}(&{}self) -> Lazy{};", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Optional => {}
            }
        } else {
            match self.frequency {
                Frequency::Repeated => writeln!(w, "    fn get_lazy_{}{}(&{}self) -> Vec<Lazy{}>;", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Optional => writeln!(w, "    fn get_lazy_{}{}(&{}self) -> Option<Lazy{}>;", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Required => {}
            }
        }
        Ok(())
    }


    // TODO: from_lazy_reader_slice -> lazy_reader_get_slice
    /// We assume that all already sliced fields can be safely lazily sliced
    /// because the methods used to derive the slices ran equivalent logic
    fn write_lazy_read_field_lazy<W: Write>(&self, w: &mut W, desc: &FileDescriptor, exclude_lifetime: bool) -> Result<()> {
        let (_, _, lazy_val_full) = self.typ.lazy_read_fn(desc)?;
        let name = &self.name;
        let rust_type = self.typ.rust_type(desc)?;
        let fn_lifetime = if self.typ.has_lifetime(desc) && !exclude_lifetime { "<'a>" } else { "" };
        let self_lifetime = if self.typ.has_lifetime(desc) && !exclude_lifetime { "'a " } else { "" };
        writeln!(w, "")?;
        if self.struct_field {
            match self.frequency {
                Frequency::Repeated => writeln!(w, "    fn get_lazy_{}{}(&{}self) -> Vec<Lazy{}>{{", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Required => writeln!(w, "    fn get_lazy_{}{}(&{}self) -> Lazy{}{{", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Optional => {}
            }
            match self.frequency {
                Frequency::Required => {
                    writeln!(w, "        let bytes = &self.{};", name)?;
                    writeln!(w, "        let mut r = BytesReader::from_bytes(bytes);")?;
                    writeln!(w, "        {}.unwrap()", lazy_val_full)?;
                }
                Frequency::Repeated => {
                    writeln!(w, "        let mut result = Vec::with_capacity(self.{}.len());", name)?;
                    writeln!(w, "        for bytes in &self.{} {{", name)?;
                    writeln!(w, "            let mut r = BytesReader::from_bytes(bytes);")?;
                    writeln!(w, "            result.push({}.unwrap())", lazy_val_full)?;
                    writeln!(w, "        }}")?;
                    writeln!(w, "        result")?;
                },
                Frequency::Optional => {}
            }
        } else {
            match self.frequency {
                Frequency::Repeated => writeln!(w, "    fn get_lazy_{}{}(&{}self) -> Vec<Lazy{}>{{", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Optional => writeln!(w, "    fn get_lazy_{}{}(&{}self) -> Option<Lazy{}>{{", name, fn_lifetime, self_lifetime, rust_type)?,
                Frequency::Required => {}
            }
            match self.frequency {
                Frequency::Optional => {
                    writeln!(w, "        if let Some(ref bytes) = self.{} {{", name)?;
                    writeln!(w, "            let mut r = BytesReader::from_bytes(bytes);")?;
                    writeln!(w, "            return Some({}.unwrap());", lazy_val_full)?;
                    writeln!(w, "        }}")?;
                    writeln!(w, "        None")?;
                },
                Frequency::Repeated => {
                    writeln!(w, "        let mut result = Vec::with_capacity(self.{}.len());", name)?;
                    writeln!(w, "        for bytes in &self.{} {{", name)?;
                    writeln!(w, "            let mut r = BytesReader::from_bytes(bytes);")?;
                    writeln!(w, "            result.push({}.unwrap())", lazy_val_full)?;
                    writeln!(w, "        }}")?;
                    writeln!(w, "        result")?;
                },
                Frequency::Required => {}
            }
        }
        writeln!(w, "    }}")?;
        Ok(())
    }

    fn write_convert_field_lazy_to_owned<W: Write>(&self, w: &mut W, _desc: &FileDescriptor) -> Result<()> {
        write!(w, "            {}: ", self.name)?;
        match self.frequency {
            Frequency::Repeated => writeln!(w, "self.{}.iter().map(|d| d.to_vec()).collect(),", self.name)?,
            Frequency::Required => writeln!(w, "self.{}.to_vec(),", self.name)?,
            Frequency::Optional => writeln!(w, "self.{}.map(|d| d.to_vec()),", self.name)?,
        }
        Ok(())
    }

    fn write_convert_owned_to_resolved<W: Write>(&self, w: &mut W, _desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "            {n}: self.get_{n}().unwrap(),", n=self.name)?;
        Ok(())
    }


    fn write_deprecated_match_tag<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        let (_, val_cow) = self.typ.read_fn_deprecated(desc)?;
        write!(w, "                Ok({}) => ", self.tag())?;
        match self.frequency {
            _ if self.boxed => writeln!(w, "{{ }},")?,
            Frequency::Optional => writeln!(w, "{{ {}?; }}", val_cow)?,
            Frequency::Repeated => writeln!(w, "{{ r.read_packed(bytes, |r, bytes| {})?; }}", val_cow)?,
            Frequency::Required => {},
        }
        Ok(())
    }

    fn write_write<W: Write>(&self, w: &mut W) -> Result<()> {
        let self_write = self.typ.get_write(&format!("&self.{}", self.name), self.boxed);
        if self.struct_field {
            match self.frequency {
                Frequency::Required => {
                    writeln!(w, "        w.{}?;", self_write)?;
                }
                Frequency::Optional => {},
                Frequency::Repeated => unimplemented!("Should structs support repeated fields"),
            }
        } else {
            match self.frequency {
                Frequency::Required => {}
                Frequency::Optional => {
                    writeln!(
                        w,
                        "        if let Some(ref s) = self.{} {{ w.write_with_tag({}, |w| w.{})?; }}",
                        self.name,
                        self.tag(),
                        self.typ.get_write("s", self.boxed)
                    )?;
                },
                Frequency::Repeated => writeln!(
                    w,
                    "        w.write_packed_with_tag({}, &self.{}, |w, m| w.{})?;",
                    self.tag(),
                    self.name,
                    self.typ.get_write("m", self.boxed)
                )?,
            }
        }

        Ok(())
    }

    fn write_shallow_merge_operation<W: Write>(&self, w: &mut W) -> Result<()> {
        match self.frequency {
            Frequency::Required => writeln!(w, "        self.{n} = other.{n}.clone();", n=self.name)? ,
            Frequency::Optional => writeln!(w, "        if other.{n}.is_some() {{ self.{n} = other.{n}.clone(); }}", n=self.name)? ,
            Frequency::Repeated => writeln!(w, "        if !other.{n}.is_empty() {{ self.{n} = other.{n}.clone(); }}", n=self.name)?
        }
        Ok(())
    }

    fn write_debug_struct_field<W: Write>(&self, w: &mut W) -> Result<()> {
        match self.frequency {
            Frequency::Required => writeln!(w, "        out.field(\"{n}\", &self.{n});", n=self.name)? ,
            Frequency::Optional => writeln!(w, "        if self.{n}.is_some() {{ out.field(\"{n}\", &self.{n}); }}", n=self.name)? ,

            Frequency::Repeated => writeln!(w, "        if !self.{n}.is_empty()  {{ out.field(\"{n}\", &self.{n}); }}", n=self.name)?
        }
        Ok(())
    }




}


#[derive(Debug, Clone, Default)]
pub struct Message {
    pub name: String,
    pub fields: Vec<Field>,
    pub oneofs: Vec<usize>,
    pub reserved_nums: Option<Vec<i32>>,
    pub reserved_names: Option<Vec<String>>,
    pub imported: bool,
    pub package: String,        // package from imports + nested items
    pub messages: Vec<Message>, // nested messages
    pub enums: Vec<Enumerator>, // nested enums
    pub module: String,         // 'package' corresponding to actual generated Rust module
    pub path: PathBuf,
    pub import: PathBuf,
    pub is_struct: bool
}

impl Message {
    fn is_leaf(&self, leaf_messages: &[String]) -> bool {
        self.imported
            || self.fields
            .iter()
            .all(|f| f.is_leaf(leaf_messages) || f.deprecated)
    }

    fn get_derives(&self, desc: &FileDescriptor) -> BTreeSet<Derives> {
        if self.has_lifetime(desc) {
            BTreeSet::new()
        } else {
            let all: BTreeSet<Derives> = [Derives::Copy, Derives::Hash]
                .iter().cloned().collect();

            self.fields
                .iter()
                .fold(all, |a, b| a.intersection(&b.get_derives(desc)).map(|x| x.clone()).collect())
        }
    }

    fn has_lifetime(&self, desc: &FileDescriptor) -> bool {
        self.fields
            .iter()
            .any(|f| match f.typ {
                FieldType::Message(ref m) if &m[..] == self.name => false,
                ref t => t.has_lifetime(desc),
            })
    }

    fn get_modules(&self, desc: &FileDescriptor) -> String {
        get_modules(&self.module, self.imported, desc)
    }

    fn is_unit(&self) -> bool {
        self.fields.is_empty()
    }

    fn write<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        println!("Writing message {}{}", self.get_modules(desc), self.name);
        writeln!(w, "")?;

        self.write_definition(w, desc)?;
        writeln!(w, "")?;
        self.write_impl_message_read(w, desc)?;
        writeln!(w, "")?;
        self.write_impl_message_write(w, desc)?;
        writeln!(w, "")?;
        self.write_impl_message_general(w, desc)?;
        writeln!(w, "")?;
        self.write_impl_message_debug(w, desc)?;
        writeln!(w, "")?;
        self.write_definition_accessor_trait(w, desc)?;
        writeln!(w, "")?;
        self.write_definition_lazy(w, desc)?;
        writeln!(w, "")?;
        self.write_impl_message_lazy_read(w, desc)?;
        writeln!(w, "")?;
        self.write_impl_message_lazy_general(w, desc)?;
        writeln!(w, "")?;
        self.write_impl_message_lazy_accessors(w, desc)?;
        writeln!(w, "")?;
        self.write_definition_lazy_owned(w, desc)?;
        writeln!(w, "")?;
        self.write_impl_message_owned_lazy_general(w, desc)?;
        writeln!(w, "")?;
        self.write_impl_message_owned_lazy_accessors(w, desc)?;

        if !(self.messages.is_empty() && self.enums.is_empty()) {
            writeln!(w, "")?;
            writeln!(w, "pub mod mod_{} {{", self.name)?;
            writeln!(w, "")?;
            if self.messages.iter().any(|m| {
                m.fields
                    .iter()
                    .any(|f| f.typ.has_cow())
            }) {
                writeln!(w, "use std::borrow::Cow;")?;
            }
            if !self.messages.is_empty() {
                writeln!(w, "use super::*;")?;
            }
            for m in &self.messages {
                m.write(w, desc)?;
            }
            for e in &self.enums {
                e.write(w)?;
            }

            writeln!(w, "")?;
            writeln!(w, "}}")?;
        }

        Ok(())
    }

    fn write_definition<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        write!(w, "#[derive(Default, PartialEq, Clone");
        for derive in self.get_derives(desc) {
            write!(w, ", {}", derive.to_string());
        }
        writeln!(w, ")]");

        if self.is_unit() {
            writeln!(w, "pub struct {} {{ }}", self.name)?;
            return Ok(());
        }

        // Write resolved definition
        if self.has_lifetime(desc) {
            writeln!(w, "pub struct {}<'a> {{", self.name)?;
        } else {
            writeln!(w, "pub struct {} {{", self.name)?;
        }
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_definition(w, desc)?;
        }
        writeln!(w, "}}")?;
        writeln!(w, "")?;

        Ok(())
    }

    fn write_definition_accessor_trait<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        if self.is_unit() {
            writeln!(w, "pub trait Access{} {{ }}", self.name)?;
            return Ok(());
        }

        // Write resolved definition
        writeln!(w, "pub trait Access{} {{", self.name)?;
        self.write_definition_accessor_trait_fields(w, desc, false);
        writeln!(w, "}}")?;
        writeln!(w, "")?;

        Ok(())
    }

    fn write_definition_accessor_trait_fields<W: Write>(
        &self,
        w: &mut W,
        desc: &FileDescriptor,
        exclude_lifetime: bool
    ) -> Result<()> {
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_definition_accessor_trait_lazy_primitive_field(w, desc, exclude_lifetime)?;

            match f.typ.clone() {
                FieldType::Message(_) => {
                    f.write_definition_accessor_trait_lazy_field(w, desc, exclude_lifetime)?;
                },
                _ => {}
            }
        }
        Ok(())
    }

    fn write_definition_lazy<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "#[derive(Debug, Default, PartialEq, Clone)]")?;
        if self.is_unit() {
            writeln!(w, "pub struct Lazy{} {{ }}", self.name)?;
            return Ok(());
        }

        // Write resolved definition
        writeln!(w, "pub struct Lazy{}<'a> {{", self.name)?;
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_definition_lazy(w, desc)?;
        }
        writeln!(w, "}}")?;
        writeln!(w, "")?;

        Ok(())
    }


    fn write_definition_lazy_owned<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "#[derive(Debug, Default, PartialEq, Clone)]")?;
        if self.is_unit() {
            writeln!(w, "pub struct OwnedLazy{} {{ }}", self.name)?;
            return Ok(());
        }

        writeln!(w, "pub struct OwnedLazy{} {{", self.name)?;
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_definition_owned_lazy(w, desc)?;
        }
        writeln!(w, "}}")?;
        writeln!(w, "")?;

        Ok(())
    }


    fn write_impl_message_read<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        if self.is_unit() {
            writeln!(w, "impl<'a> MessageRead<'a> for {} {{", self.name)?;
            writeln!(
                w,
                "    fn from_reader(r: &mut BytesReader, _: &[u8]) -> Result<Self> {{"
            )?;
            writeln!(w, "        r.read_to_end();")?;
            writeln!(w, "        Ok(Self::default())")?;
            writeln!(w, "    }}")?;
            writeln!(w, "}}")?;
            return Ok(());
        }

        if self.has_lifetime(desc) {
            writeln!(w, "impl<'a> MessageRead<'a> for {}<'a> {{", self.name)?;
            writeln!(
                w,
                "    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {{"
            )?;
        } else {
            writeln!(w, "impl<'a> MessageRead<'a> for {} {{", self.name)?;
            writeln!(
                w,
                "    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {{"
            )?;
        }

        writeln!(w, "        let mut msg = Self::default();")?;
        if !self.is_struct {
            writeln!(w, "        'wh: loop {{")?;
            writeln!(w, "            let next_tag = r.next_tag(bytes);")?;
            writeln!(w, "            match next_tag {{")?;
            writeln!(w, r#"                Ok(0) => {{ break 'wh; }},"#)?;

        }
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_read_field(w, desc)?;
        }
        if !self.is_struct {
            for f in self.fields.iter().filter(|f| f.deprecated) {
                f.write_deprecated_match_tag(w, desc)?;
            }
            writeln!(w, "                Ok(t) => {{ r.read_unknown(bytes, t, \"{}\")?; }}", self.name)?;
            writeln!(w, "                Err(e) => return Err(e),")?;
            writeln!(w, "            }}")?;
            writeln!(w, "        }}")?;
        }
        writeln!(w, "        Ok(msg)")?;
        writeln!(w, "    }}")?;
        writeln!(w, "}}")?;

        Ok(())
    }

    fn write_impl_message_lazy_read_from_reader<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "    fn from_lazy_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {{")?;
        writeln!(w, "        let mut msg = Self::default();")?;
        if !self.is_struct {
            writeln!(w, "        'wh: loop {{")?;
            writeln!(w, "            let next_tag = r.next_tag(bytes);")?;
            writeln!(w, "            match next_tag {{")?;
            writeln!(w, "                Ok(0) => {{ break 'wh; }},")?;

        }
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_lazy_read_field(w, desc)?;
        }
        for f in self.fields.iter().filter(|f| f.deprecated) {
            f.write_deprecated_match_tag(w, desc)?;
        }
        if !self.is_struct {
            writeln!(w, "                Ok(t) => {{ r.read_unknown(bytes, t, \"{}\")?; }}", self.name)?;
            writeln!(w, "                Err(e) => return Err(e),")?;
            writeln!(w, "            }}")?;
            writeln!(w, "        }}")?;
        }
        writeln!(w, "        Ok(msg)")?;
        writeln!(w, "    }}")?;
        writeln!(w, "")?;
        Ok(())
    }

    fn write_impl_message_lazy_read_from_reader_to_slice<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "    fn from_lazy_reader_slice(r: &mut BytesReader, bytes: &'a [u8]) -> Result<&'a [u8]> {{")?;
        writeln!(w, "        let init_start = r.start;")?;
        if !self.is_struct {
            writeln!(w, "        'wh: loop {{")?;

            writeln!(w, r#"            let next_tag = r.next_tag(bytes);"#)?;
            writeln!(w, "            match next_tag {{")?;
            writeln!(w, r#"                Ok(0) => {{ break 'wh; }},"#)?;
        }
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_lazy_read_field_slice(w, desc)?;
        }
        for f in self.fields.iter().filter(|f| f.deprecated) {
            f.write_deprecated_match_tag(w, desc)?;
        }
        if !self.is_struct {
            writeln!(w, "                Ok(t) => {{ r.read_unknown(bytes, t, \"{}\")?; }}", self.name)?;
            writeln!(w, "                Err(e) => return Err(e),")?;
            writeln!(w, "            }}")?;
            writeln!(w, "        }}")?;
        }
        writeln!(w, "        Ok(&bytes[init_start..r.start])")?;
        writeln!(w, "    }}")?;
        writeln!(w, "}}")?;
        Ok(())
    }

    fn write_impl_message_lazy_read<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "#[allow(dead_code)]")?;
        if self.is_unit() {
            writeln!(w, "impl<'a> LazyMessageRead<'a> for Lazy{} {{", self.name)?;
            writeln!(w, "    fn from_lazy_reader(r: &mut BytesReader, _: &[u8]) -> Result<Self> {{")?;
            writeln!(w, "        r.read_to_end();")?;
            writeln!(w, "        Ok(Self::default())")?;
            writeln!(w, "    }}")?;
            writeln!(w, "}}")?;
            return Ok(());
        }

        writeln!(w, "impl<'a> LazyMessageRead<'a> for Lazy{}<'a> {{", self.name)?;
        self.write_impl_message_lazy_read_from_reader(w, desc)?;
        self.write_impl_message_lazy_read_from_reader_to_slice(w, desc)?;
        Ok(())
    }

    fn write_impl_message_write<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        if self.is_unit() {
            writeln!(w, "impl MessageWrite for {} {{ }}", self.name)?;
            return Ok(());
        }
        if self.has_lifetime(desc) {
            writeln!(w, "impl<'a> MessageWrite for {}<'a> {{", self.name)?;
        } else {
            writeln!(w, "impl MessageWrite for {} {{", self.name)?;
        }
        self.write_write_message(w)?;
        writeln!(w, "}}")?;
        Ok(())
    }

    fn write_impl_message_general<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "#[allow(dead_code)]")?;
        if self.is_unit() {
            writeln!(w, "impl {} {{ }}", self.name)?;
            return Ok(());
        }

        if self.has_lifetime(desc) {
            writeln!(w, "impl<'a> {}<'a> {{", self.name)?;
        } else {
            writeln!(w, "impl {} {{", self.name)?;
        }
        self.write_shallow_merge_operation(w, desc)?;
        writeln!(w, "}}")?;
        Ok(())
    }

    fn write_impl_message_debug<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        if self.is_unit() {
            writeln!(w, "impl fmt::Debug for {} {{ }}", self.name)?;
            return Ok(());
        }

        if self.has_lifetime(desc) {
            writeln!(w, "impl<'a> fmt::Debug for {}<'a> {{", self.name)?;
        } else {
            writeln!(w, "impl<'a> fmt::Debug for {} {{", self.name)?;
        }
        writeln!(w, "    fn fmt(&self, fmt: &mut fmt::Formatter) -> fmt::Result {{")?;
        writeln!(w, "        let mut out = fmt.debug_struct(\"{}\");", self.name)?;
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_debug_struct_field(w)?;
        }
        writeln!(w, "        out.finish()")?;
        writeln!(w, "    }}")?;
        writeln!(w, "}}")?;
        Ok(())
    }

    fn write_write_message<W: Write>(&self, w: &mut W) -> Result<()> {
        writeln!(w, "    fn write_message<W: Write>(&self, w: &mut Writer<W>) -> Result<()> {{")?;
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_write(w)?;
        }
        if !self.is_struct {
            writeln!(w, "         w.write_u8(0)?;");
        }
        writeln!(w, "        Ok(())")?;
        writeln!(w, "    }}")?;
        Ok(())
    }

    fn write_shallow_merge_operation<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        if self.has_lifetime(desc) {
            writeln!(w, "    pub fn shallow_merge(&mut self, other: &{}<'a>) {{", self.name)?;

        } else {
            writeln!(w, "    pub fn shallow_merge(&mut self, other: &{}) {{", self.name)?;
        }

        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_shallow_merge_operation(w)?;
        }
        writeln!(w, "    }}")?;
        Ok(())
    }

    fn write_impl_message_lazy_general<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "#[allow(dead_code)]")?;
        if self.is_unit() {
            writeln!(w, "impl Lazy{} {{ }}", self.name)?;
            return Ok(());
        }

        writeln!(w, "impl<'a> Lazy{}<'a> {{", self.name)?;
        self.write_convert_message_lazy_to_owned(w, desc)?;
        writeln!(w, "}}")?;
        Ok(())
    }

    fn write_impl_message_lazy_accessors<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "#[allow(dead_code)]")?;
        if self.is_unit() {
            writeln!(w, "impl Access{n} for Lazy{n} {{ }}", n=self.name)?;
            return Ok(());
        }

        writeln!(w, "impl<'b> Access{n} for Lazy{n}<'b> {{", n=self.name)?;
        self.write_convert_message_read_lazy_fields(w, desc, false)?;
        writeln!(w, "}}")?;
        Ok(())
    }


    fn write_impl_message_owned_lazy_general<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "#[allow(dead_code)]")?;
        if self.is_unit() {
            writeln!(w, "impl OwnedLazy{} {{ }}", self.name)?;
            return Ok(());
        }

        writeln!(w, "impl OwnedLazy{} {{", self.name)?;
        self.write_owned_lazy_shallow_merge_operation(w, desc)?;
        writeln!(w, "")?;
        self.write_convert_owned_to_resolved(w, desc)?;
        writeln!(w, "}}")?;
        Ok(())
    }

    fn write_impl_message_owned_lazy_accessors<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "#[allow(dead_code)]")?;
        if self.is_unit() {
            writeln!(w, "impl Access{n} for OwnedLazy{n} {{ }}", n=self.name)?;
            return Ok(());
        }

        writeln!(w, "impl<'b> Access{n} for OwnedLazy{n} {{", n=self.name)?;
        self.write_convert_message_read_lazy_fields(w, desc, false)?;
        writeln!(w, "}}")?;
        Ok(())
    }

    fn write_convert_message_read_lazy_fields<W: Write>(
        &self,
        w: &mut W,
        desc: &FileDescriptor,
        exclude_lifetime: bool
    ) -> Result<()> {
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_lazy_read_field_convert(w, desc, exclude_lifetime)?;
            match f.typ.clone() {
                FieldType::Message(_) => {
                    f.write_lazy_read_field_lazy(w, desc, exclude_lifetime)?;
                },
                _ => {}
            }
        }
        Ok(())
    }

    fn write_convert_message_lazy_to_owned<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        writeln!(w, "    pub fn to_owned_struct(&self) -> OwnedLazy{} {{", self.name)?;
        writeln!(w, "        OwnedLazy{} {{", self.name)?;
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_convert_field_lazy_to_owned(w, desc)?;
        }
        writeln!(w, "        }}")?;
        writeln!(w, "    }}")?;
        Ok(())
    }

    fn write_convert_owned_to_resolved<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        if self.has_lifetime(desc) {
            writeln!(w, "    pub fn to_resolved<'a>(&'a self) -> {}<'a> {{", self.name)?;
        } else {
            writeln!(w, "    pub fn to_resolved(&self) -> {} {{", self.name)?;
        }
        writeln!(w, "        {} {{", self.name)?;
        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_convert_owned_to_resolved(w, desc)?;
        }
        writeln!(w, "        }}")?;
        writeln!(w, "    }}")?;
        Ok(())
    }

    fn write_owned_lazy_shallow_merge_operation<W: Write>(&self, w: &mut W, desc: &FileDescriptor) -> Result<()> {
        if self.has_lifetime(desc) {
            writeln!(w, "    pub fn shallow_merge(&mut self, other: &OwnedLazy{}) {{", self.name)?;

        } else {
            writeln!(w, "    pub fn shallow_merge(&mut self, other: &OwnedLazy{}) {{", self.name)?;
        }

        for f in self.fields.iter().filter(|f| !f.deprecated) {
            f.write_shallow_merge_operation(w)?;
        }
        writeln!(w, "    }}")?;
        Ok(())
    }

    fn sanity_checks(&self) -> Result<()> {
        // checks for reserved fields
        for f in self.fields
            .iter()
        {
            if (self.reserved_names
                .as_ref()
                .map_or(false, |names| names.contains(&f.name)))
                || (self.reserved_nums
                    .as_ref()
                    .map_or(false, |nums| nums.contains(&f.number)))
            {
                return Err(Error::InvalidMessage(format!(
                    "Error in message {}\n\
                     Field {:?} conflict with reserved fields",
                    self.name, f
                )));
            }
        }
        Ok(())
    }

    /// Searches for a matching message in all message
    ///
    /// If none is found, then it is an enum
    fn set_reference_types(&mut self, desc: &FileDescriptor) {
        for f in self.fields.iter_mut() {
            if let FieldType::ReferenceType(m) = f.typ.clone() {
                if f.typ.find_message(&desc.messages).is_some() {
                    f.typ = FieldType::Message(m);
                    continue;
                }
                if f.typ.find_enum(&desc.enums).is_some() {
                    f.typ = FieldType::Enum(m);
                    continue;
                }
                panic!("Missing type {:?}", f.typ);
            }
        }
    }

    fn sanitize_names(&mut self) {
        sanitize_keyword(&mut self.name);
        sanitize_keyword(&mut self.package);
        for f in self.fields.iter_mut() {
            sanitize_keyword(&mut f.name);
        }
        for m in &mut self.messages {
            m.sanitize_names();
        }
        for e in &mut self.enums {
            e.sanitize_names();
        }
    }
}


#[derive(Debug, Clone)]
pub struct Enumerator {
    pub name: String,
    pub fields: Vec<(String, i32)>,
    pub imported: bool,
    pub package: String,
    pub module: String,
    pub path: PathBuf,
    pub import: PathBuf,
}

impl Enumerator {
    fn sanitize_names(&mut self) {
        sanitize_keyword(&mut self.name);
        sanitize_keyword(&mut self.package);
        for f in self.fields.iter_mut() {
            sanitize_keyword(&mut f.0);
        }
    }

    fn get_modules(&self, desc: &FileDescriptor) -> String {
        get_modules(&self.module, self.imported, desc)
    }

    fn write<W: Write>(&self, w: &mut W) -> Result<()> {
        println!("Writing enum {}", self.name);
        writeln!(w, "")?;
        self.write_definition(w)?;
        writeln!(w, "")?;
        if self.fields.is_empty() {
            Ok(())
        } else {
            self.write_impl_default(w)?;
            writeln!(w, "")?;
            self.write_from_u32(w)?;
            writeln!(w, "")?;
            self.write_from_str(w)
        }
    }

    fn write_definition<W: Write>(&self, w: &mut W) -> Result<()> {
        writeln!(w, "#[derive(Hash, PartialOrd, Ord, Eq, Debug, PartialEq, Clone, Copy)]")?;

        writeln!(w, "pub enum {} {{", self.name)?;
        for &(ref f, ref number) in &self.fields {
            writeln!(w, "    {} = {},", f, number)?;
        }
        writeln!(w, "}}")?;
        Ok(())
    }

    fn write_impl_default<W: Write>(&self, w: &mut W) -> Result<()> {
        writeln!(w, "impl Default for {} {{", self.name)?;
        writeln!(w, "    fn default() -> Self {{")?;
        writeln!(w, "        {}::{}", self.name, self.fields[0].0)?;
        writeln!(w, "    }}")?;
        writeln!(w, "}}")?;
        Ok(())
    }

    fn write_from_u32<W: Write>(&self, w: &mut W) -> Result<()> {
        writeln!(w, "impl From<u32> for {} {{", self.name)?;
        writeln!(w, "    fn from(i: u32) -> Self {{")?;
        writeln!(w, "        match i {{")?;
        for &(ref f, ref number) in &self.fields {
            writeln!(w, "            {} => {}::{},", number, self.name, f)?;
        }
        writeln!(w, "            _ => Self::default(),")?;
        writeln!(w, "        }}")?;
        writeln!(w, "    }}")?;
        writeln!(w, "}}")?;
        Ok(())
    }

    fn write_from_str<W: Write>(&self, w: &mut W) -> Result<()> {
        writeln!(w, "impl<'a> From<&'a str> for {} {{", self.name)?;
        writeln!(w, "    fn from(s: &'a str) -> Self {{")?;
        writeln!(w, "        match s {{")?;
        for &(ref f, _) in &self.fields {
            writeln!(w, "            {:?} => {}::{},", f, self.name, f)?;
        }
        writeln!(w, "            _ => Self::default(),")?;
        writeln!(w, "        }}")?;
        writeln!(w, "    }}")?;
        writeln!(w, "}}")?;
        Ok(())
    }
}

pub struct Config {
    pub in_file: PathBuf,
    pub out_file: PathBuf,
    pub single_module: bool,
    pub import_search_path: Vec<PathBuf>,
    pub no_output: bool,
}

#[derive(Debug, Default, Clone)]
pub struct FileDescriptor {
    pub import_paths: Vec<PathBuf>,
    pub package: String,
    pub messages: Vec<Message>,
    pub enums: Vec<Enumerator>,
    pub module: String,
}

impl FileDescriptor {
    pub fn write_proto(config: &Config) -> Result<()> {
        let mut desc = FileDescriptor::read_proto(
            &config.in_file,
            &config.import_search_path)?;

        if desc.messages.is_empty() && desc.enums.is_empty() {
            // There could had been unsupported structures, so bail early
            return Err(Error::EmptyRead);
        }

        desc.set_reference_types();

        let mut leaf_messages = Vec::new();
        break_cycles(&mut desc.messages, &mut leaf_messages);

        desc.sanity_checks()?;
        desc.sanitize_names();

        if config.single_module {
            desc.package = "".to_string();
        }

        let (prefix, file_package) = split_package(&desc.package);

        let mut file_stem = if file_package.is_empty() {
            get_file_stem(&config.out_file)?
        } else {
            file_package.to_string()
        };

        if !file_package.is_empty() {
            sanitize_keyword(&mut file_stem);
        }
        let mut out_file = config.out_file.with_file_name(format!("{}.rs", file_stem.to_string().to_lowercase()));

        if !prefix.is_empty() {
            use std::fs::create_dir_all;
            // e.g. package is a.b; we need to create directory 'a' and insert it into the path
            let file = PathBuf::from(out_file.file_name().unwrap());
            out_file.pop();
            for p in prefix.split('.') {
                out_file.push(p);
            }
            if !out_file.exists() {
                create_dir_all(&out_file)?;
                update_mod_file(&out_file)?;
            }
            out_file.push(file);
        }
        if config.no_output {
            let imported = |b| if b { " imported" } else { "" };
            println!("source will be written to {}\n", out_file.display());
            for m in &desc.messages {
                println!(
                    "message {} module {}{}",
                    m.name,
                    m.module,
                    imported(m.imported)
                );
            }
            for e in &desc.enums {
                println!(
                    "enum {} module {}{}",
                    e.name,
                    e.module,
                    imported(e.imported)
                );
            }
            return Ok(());
        }

        let name = config.in_file.file_name().and_then(|e| e.to_str()).unwrap();
        let mut w = BufWriter::new(File::create(&out_file)?);
        desc.write(&mut w, name)?;
        update_mod_file(&out_file)
    }

    /// Opens a proto file, reads it and returns raw parsed data
    pub fn read_proto(
        in_file: &Path,
        import_search_path: &[PathBuf]
    ) -> Result<FileDescriptor> {
        let mut buf = Vec::new();
        {
            let f = File::open(in_file)?;
            let mut reader = BufReader::new(f);
            reader.read_to_end(&mut buf)?;
        }
        let mut desc = file_descriptor(&buf).to_result().map_err(Error::Nom)?;
        for mut m in &mut desc.messages {
            if m.path.as_os_str().is_empty() {
                m.path = in_file.clone().to_path_buf();
                if &import_search_path.len() > &0 {
                    if let Ok(p) = m.path.clone().strip_prefix(&import_search_path[0]) {
                        m.import = p.to_path_buf();
                    }
                }
            }
        }
        // proto files with no packages are given an implicit module,
        // since every generated Rust source file represents a module
        desc.module = if desc.package.is_empty() {
            get_file_stem(in_file)?.clone().to_string().to_lowercase()
        } else {
            desc.package.clone().to_string().to_lowercase()
        };

        Ok(desc)
    }

    fn sanity_checks(&self) -> Result<()> {
        for m in &self.messages {
            m.sanity_checks()?;
        }
        Ok(())
    }

    fn sanitize_names(&mut self) {
        for m in &mut self.messages {
            m.sanitize_names();
        }
        for e in &mut self.enums {
            e.sanitize_names();
        }
    }

    fn set_reference_types(&mut self) {
        let copy = self.clone();

        for m in &mut self.messages {
            m.set_reference_types(&copy);
        }

    }

    fn write<W: Write>(&self, w: &mut W, filename: &str) -> Result<()> {
        println!(
            "Found {} messages, and {} enums",
            self.messages.len(),
            self.enums.len(),
        );
        self.write_headers(w, filename)?;
        self.write_package_start(w)?;
        self.write_uses(w)?;
        self.write_imports(w)?;
        self.write_enums(w)?;
        self.write_messages(w)?;
        self.write_package_end(w)?;
        println!("Done processing {}", filename);
        Ok(())
    }

    fn write_headers<W: Write>(&self, w: &mut W, filename: &str) -> Result<()> {
        writeln!(
            w,
            "//! Automatically generated rust module for '{}' file",
            filename
        )?;
        writeln!(w, "")?;
        writeln!(w, "#![allow(non_snake_case)]")?;
        writeln!(w, "#![allow(non_upper_case_globals)]")?;
        writeln!(w, "#![allow(non_camel_case_types)]")?;
        writeln!(w, "#![allow(unused_imports)]")?;
        writeln!(w, "#![allow(unknown_lints)]")?;
        writeln!(w, "#![allow(clippy)]")?;
        writeln!(w, "#![allow(dead_code)]")?;

        writeln!(w, "#![cfg_attr(rustfmt, rustfmt_skip)]")?;
        writeln!(w, "")?;
        Ok(())
    }

    fn write_package_start<W: Write>(&self, w: &mut W) -> Result<()> {
        writeln!(w, "")?;
        Ok(())
    }

    fn write_uses<W: Write>(&self, w: &mut W) -> Result<()> {
        if self.messages.iter().all(|m| m.is_unit()) {
            writeln!(
                w,
                "use quick_kiwi::{{BytesReader, Result, LazyMessageRead, MessageRead, MessageWrite}};"
            )?;
            return Ok(());
        }
        writeln!(w, "use std::fmt;")?;
        writeln!(w, "use std::io::Write;")?;
        if self.messages.iter().any(|m| {
            m.fields
                .iter()
                .any(|f| f.typ.has_cow())
        }) {
            writeln!(w, "use std::borrow::Cow;")?;
        }
        writeln!(
            w,
            "use quick_kiwi::{{LazyMessageRead, MessageRead, MessageWrite, BytesReader, Writer, Result}};"
        )?;
        Ok(())
    }

    fn write_imports<W: Write>(&self, w: &mut W) -> Result<()> {
        // even if we don't have an explicit package, there is an implicit Rust module
        // This `use` allows us to refer to the package root.
        // NOTE! I'm suppressing not-needed 'use super::*' errors currently!
        let mut depth = self.package.split('.').count();
        if depth == 0 {
            depth = 1;
        }
        write!(w, "use ")?;
        for _ in 0..depth {
            write!(w, "super::")?;
        }
        writeln!(w, "*;")?;
        Ok(())
    }

    fn write_package_end<W: Write>(&self, w: &mut W) -> Result<()> {
        writeln!(w, "")?;
        Ok(())
    }

    fn write_enums<W: Write>(&self, w: &mut W) -> Result<()> {
        for m in self.enums.iter().filter(|e| !e.imported) {
            println!("Writing enum {}", m.name);
            writeln!(w, "")?;
            m.write_definition(w)?;
            writeln!(w, "")?;
            m.write_impl_default(w)?;
            writeln!(w, "")?;
            m.write_from_u32(w)?;
            writeln!(w, "")?;
            m.write_from_str(w)?;
        }
        Ok(())
    }

    fn write_messages<W: Write>(&self, w: &mut W) -> Result<()> {
        for m in self.messages.iter().filter(|m| !m.imported) {
            println!("Writing message {}", m.name);
            m.write(w, &self)?;
        }
        Ok(())
    }
}


/// Breaks cycles by adding boxes when necessary
///
/// Cycles means one Message calls itself at some point
fn break_cycles(messages: &mut [Message], leaf_messages: &mut Vec<String>) {
    for m in messages.iter_mut() {
        break_cycles(&mut m.messages, leaf_messages);
    }

    let message_names = messages
        .iter()
        .map(|m| m.name.to_string())
        .collect::<Vec<_>>();

    let mut undef_messages = (0..messages.len()).collect::<Vec<_>>();
    while !undef_messages.is_empty() {
        let len = undef_messages.len();
        let mut new_undefs = Vec::new();
        for i in undef_messages {
            if messages[i].is_leaf(&leaf_messages) {
                leaf_messages.push(message_names[i].clone())
            } else {
                new_undefs.push(i);
            }
        }
        undef_messages = new_undefs;
        if len == undef_messages.len() {
            // try boxing messages, one by one ...
            let k = undef_messages.pop().unwrap();
            {
                let mut m = messages[k].clone();
                for f in m.fields.iter_mut() {
                    if !f.is_leaf(&leaf_messages) {
                        f.frequency = Frequency::Optional;
                        f.boxed = true;
                    }
                }
                messages[k] = m;
            }
        }
    }
}

/// "" is ("",""), "a" is ("","a"), "a.b" is ("a"."b"), and so forth.
fn split_package(package: &str) -> (&str, &str) {
    if package.is_empty() {
        ("", "")
    } else {
        if let Some(i) = package.rfind('.') {
            (&package[0..i], &package[i + 1..])
        } else {
            ("", package)
        }
    }
}

const MAGIC_HEADER: &'static str = "//! Automatically generated mod.rs";

/// Given a file path, create or update the mod.rs file within its folder
fn update_mod_file(path: &Path) -> Result<()> {
    let mut file = path.to_path_buf();
    use std::fs::OpenOptions;
    use std::io::prelude::*;

    let name = file.file_stem().unwrap().to_string_lossy().to_string();
    file.pop();
    file.push("mod.rs");
    let matches = "pub mod ";
    let mut present = false;
    let mut exists = false;
    if let Ok(f) = File::open(&file) {
        exists = true;
        let mut first = true;
        for line in BufReader::new(f).lines() {
            let line = line?;
            if first {
                if line.find(MAGIC_HEADER).is_none() {
                    // it is NOT one of our generated mod.rs files, so don't modify it!
                    present = true;
                    break;
                }
                first = false;
            }
            if let Some(i) = line.find(matches) {
                let rest = &line[i + matches.len()..line.len() - 1];
                if rest == name {
                    // we already have a reference to this module...
                    present = true;
                    break;
                }
            }
        }
    }
    if !present {
        let mut f = if exists {
            OpenOptions::new().append(true).open(&file)?
        } else {
            let mut f = File::create(&file)?;
            write!(f, "{}\n", MAGIC_HEADER)?;
            f
        };
        write!(f, "pub mod {};\n", name)?;
    }
    Ok(())
}

/// get the proper sanitized file stem from an input file path
fn get_file_stem(path: &Path) -> Result<String> {
    let mut file_stem = path.file_stem()
        .and_then(|f| f.to_str())
        .map(|s| s.to_string())
        .ok_or_else(|| Error::from(Error::OutputFile(format!("{}", path.display()))))?;

    file_stem = file_stem.replace(|c: char| !c.is_alphanumeric(), "_");
    // will now be properly alphanumeric, but may be a keyword!
    sanitize_keyword(&mut file_stem);
    Ok(file_stem)
}
