//! A module to manage kiwi serialization

use std;
use std::io::Write;

use errors::Result;
use message::{ MessageWrite};

use byteorder::WriteBytesExt;
use byteorder::LittleEndian as LE;

/// Helper method for writing uint32 to raw bytes as in our format
pub fn raw_write_uint32(mut v: u32) -> Vec<u8> {
    let mut out: Vec<u8> = vec![];
    loop {
        let byte = v as u8 & 127;
        v >>= 7;

        if v == 0 {
            out.push(byte);
            return out;
        }

        out.push(byte | 128);
    }
}

/// A struct to write kiwi messages
///
/// # Examples
///
/// ```rust
/// // an automatically generated module which is in a separate file in general
/// mod foo_bar {
///     # use quick_kiwi::{MessageWrite, Writer, Result};
///     # use std::borrow::Cow;
///     # use std::io::Write;
///     pub struct Foo<'a> { pub name: Option<Cow<'a, str>>, }
///     pub struct Bar { pub id: Option<u32> }
///     pub struct FooBar<'a> { pub foos: Vec<Foo<'a>>, pub bars: Vec<Bar>, }
///     impl<'a> MessageWrite for FooBar<'a> {
///         // implements
///         // fn get_size(&self) -> usize { ... }
///         // fn write_message<W: Write>(&self, r: &mut Writer<W>) -> Result<()> { ... }
///         # fn get_size(&self) -> usize { 0 }
///         # fn write_message<W: Write>(&self, _: &mut Writer<W>) -> Result<()> { Ok(()) }
///     }
/// }
///
/// // FooBar is a message generated from a proto file
/// // in parcicular it contains a `write_message` function
/// use foo_bar::{FooBar, Foo, Bar};
/// use std::borrow::Cow;
/// use quick_kiwi::Writer;
///
/// fn main() {
///     // let mut r = File::create("...").expect("Cannot create file");
///     // for the sake of example, we'll use a simpler struct which impl `Write`
///     let mut r = Vec::new();
///     let mut writer = Writer::new(&mut r);
///
///     // manually generates a FooBar for the example
///     let foobar = FooBar {
///         foos: vec![Foo { name: Some(Cow::Borrowed("test!")) }, Foo { name: None }],
///         bars: vec![Bar { id: Some(43) }, Bar { id: None }],
///     };
///
///     // now using the generated module
///     writer.write_message(&foobar).expect("Cannot write FooBar");
/// }
/// ```
pub struct Writer<W> {
    inner: W,
}

impl<W: Write> Writer<W> {
    /// Creates a new `KiwiWriter`
    pub fn new(w: W) -> Writer<W> {
        Writer { inner: w }
    }

    /// Writes a tag, which represents both the field number and the wire type
    #[inline(always)]
    pub fn write_tag(&mut self, tag: u32) -> Result<()> {
        self.write_uint32(tag as u32)
    }

    /// Writes a `int32` which is internally coded as a `varint`
    #[inline(always)]
    pub fn write_int32(&mut self, v: i32) -> Result<()> {
        self.write_uint32(((v << 1) ^ (v >> 31)) as u32)
    }

    /// Writes a `uint32` which is internally coded as a `varint`
    #[inline(always)]
    pub fn write_uint32(&mut self, mut v: u32) -> Result<()> {
        loop {
            let byte = v as u8 & 127;
            v >>= 7;

            if v == 0 {
                return self.inner.write_u8(byte)
                    .map_err(|e| e.into())
            }

            self.inner.write_u8(byte | 128)?;
        }

    }

    /// Writes a `float`
    #[inline(always)]
    pub fn write_float(&mut self, v: f32) -> Result<()> {
        // Reinterpret as an integer
        let mut bits = v.to_bits();

        // Move the exponent to the first 8 bits
        bits = (bits >> 23) | (bits << 9);

        // Optimization: use a single byte to store zero and denormals (try for an exponent of 0)
        if (bits & 255) == 0 {
            return self.inner.write_u8(0)
                .map_err(|e| e.into());
        }

        // Endian-independent 32-bit write
        self.inner.write_all(&[
            bits as u8,
            (bits >> 8) as u8,
            (bits >> 16) as u8,
            (bits >> 24) as u8,
        ]).map_err(|e| e.into())

    }

    /// Writes a `bool` 1 = true, 0 = false
    #[inline(always)]
    pub fn write_bool(&mut self, v: bool) -> Result<()> {
        self.inner
            .write_u8(if v { 1 } else { 0 })
            .map_err(|e| e.into())
    }

    /// Writes an `enum` converting it to a `u32` first
    #[inline(always)]
    pub fn write_enum(&mut self, v: u32) -> Result<()> {
        self.write_uint32(v)
    }

    /// Writes `bytes`: length first then the chunk of data
    #[inline(always)]
    pub fn write_bytes(&mut self, bytes: &[u8]) -> Result<()> {
        self.write_uint32(bytes.len() as u32)?;
        self.inner.write_all(bytes).map_err(|e| e.into())
    }

    /// Writes `bytes`: length first then the chunk of data
    #[inline(always)]
    pub fn write_raw_bytes(&mut self, bytes: &[u8]) -> Result<()> {
        self.inner.write_all(bytes).map_err(|e| e.into())
    }

    /// Writes `string`: length first then the chunk of data
    #[inline(always)]
    pub fn write_string(&mut self, s: &str) -> Result<()> {
        if !s.is_empty() {
            self.write_raw_bytes(s.as_bytes())?;
        }
        self.inner.write_u8(0).map_err(|e| e.into())
    }

    /// Writes a message which implements `MessageWrite`
    #[inline]
    pub fn write_message<M: MessageWrite>(&mut self, m: &M) -> Result<()> {
        m.write_message(self)?;
        self.inner.write_u8(0).map_err(|e| e.into())
    }

    /// Writes another item prefixed with tag
    #[inline]
    pub fn write_with_tag<F>(&mut self, tag: u32, mut write: F) -> Result<()>
    where
        F: FnMut(&mut Self) -> Result<()>,
    {
        self.write_tag(tag)?;
        write(self)
    }

    /// Writes tag then repeated field
    ///
    /// If array is empty, then do nothing (do not even write the tag)
    pub fn write_packed_with_tag<M, F>(
        &mut self,
        tag: u32,
        v: &[M],
        mut write: F
    ) -> Result<()>
    where
        F: FnMut(&mut Self, &M) -> Result<()>
    {
        if !v.is_empty() {
            self.write_tag(tag)?;
            self.write_uint32(v.len() as u32)?;
            for m in v {
                write(self, m)?;
            }
        }
        Ok(())
    }

}


#[cfg(test)]
mod tests {
    use super::*;
    use std::f32;

    fn write_once(cb: fn(&mut Writer<&mut Vec<u8>>) -> Result<()>) -> Vec<u8> {
        let mut buffer = Vec::new();
        let mut writer = Writer::new(&mut buffer);

        // let mut bb = ByteBufferMut::new();
        cb(&mut writer);
        writer.inner.to_vec()
    }

    #[test]
    fn write_bool() {
        assert_eq!(write_once(|bb| bb.write_bool(false)), [0]);
        assert_eq!(write_once(|bb| bb.write_bool(true)), [1]);
    }

    #[test]
    fn write_bytes() {
        assert_eq!(write_once(|bb| bb.write_bytes(&[1, 2, 3])), [3, 1,2,3]);
        assert_eq!(write_once(|bb| bb.write_bytes(&[])), [0]);
        assert_eq!(write_once(|bb| bb.write_bytes(&[4,5])), [2, 4,5]);
    }

    #[test]
    fn write_int32() {
        assert_eq!(write_once(|bb| bb.write_int32(0)), [0]);
        assert_eq!(write_once(|bb| bb.write_int32(-1)), [1]);
        assert_eq!(write_once(|bb| bb.write_int32(1)), [2]);
        assert_eq!(write_once(|bb| bb.write_int32(-2)), [3]);
        assert_eq!(write_once(|bb| bb.write_int32(2)), [4]);
        assert_eq!(write_once(|bb| bb.write_int32(-64)), [127]);
        assert_eq!(write_once(|bb| bb.write_int32(64)), [128, 1]);
        assert_eq!(write_once(|bb| bb.write_int32(128)), [128, 2]);
        assert_eq!(write_once(|bb| bb.write_int32(-129)), [129, 2]);
        assert_eq!(write_once(|bb| bb.write_int32(-65535)), [253, 255, 7]);
        assert_eq!(write_once(|bb| bb.write_int32(65535)), [254, 255, 7]);
        assert_eq!(write_once(|bb| bb.write_int32(-2147483647)), [253, 255, 255, 255, 15]);
        assert_eq!(write_once(|bb| bb.write_int32(2147483647)), [254, 255, 255, 255, 15]);
        assert_eq!(write_once(|bb| bb.write_int32(-2147483648)), [255, 255, 255, 255, 15]);
    }

    #[test]
    fn write_uint32() {
        assert_eq!(write_once(|bb| bb.write_uint32(0)), [0]);
        assert_eq!(write_once(|bb| bb.write_uint32(1)), [1]);
        assert_eq!(write_once(|bb| bb.write_uint32(2)), [2]);
        assert_eq!(write_once(|bb| bb.write_uint32(3)), [3]);
        assert_eq!(write_once(|bb| bb.write_uint32(4)), [4]);
        assert_eq!(write_once(|bb| bb.write_uint32(127)), [127]);
        assert_eq!(write_once(|bb| bb.write_uint32(128)), [128, 1]);
        assert_eq!(write_once(|bb| bb.write_uint32(256)), [128, 2]);
        assert_eq!(write_once(|bb| bb.write_uint32(129)), [129, 1]);
        assert_eq!(write_once(|bb| bb.write_uint32(257)), [129, 2]);
        assert_eq!(write_once(|bb| bb.write_uint32(131069)), [253, 255, 7]);
        assert_eq!(write_once(|bb| bb.write_uint32(131070)), [254, 255, 7]);
        assert_eq!(write_once(|bb| bb.write_uint32(4294967293)), [253, 255, 255, 255, 15]);
        assert_eq!(write_once(|bb| bb.write_uint32(4294967294)), [254, 255, 255, 255, 15]);
        assert_eq!(write_once(|bb| bb.write_uint32(4294967295)), [255, 255, 255, 255, 15]);
        assert_eq!(write_once(|bb| bb.write_uint32(std::u32::MAX)), [255, 255, 255, 255, 15]);
    }

    #[test]
    fn write_float() {
        assert_eq!(write_once(|bb| bb.write_float(0.0)), [0]);
        assert_eq!(write_once(|bb| bb.write_float(-0.0)), [0]);
        assert_eq!(write_once(|bb| bb.write_float(123.456)), [133, 242, 210, 237]);
        assert_eq!(write_once(|bb| bb.write_float(-123.456)), [133, 243, 210, 237]);
        assert_eq!(write_once(|bb| bb.write_float(f32::MIN)), [254, 255, 255, 255]);
        assert_eq!(write_once(|bb| bb.write_float(f32::MAX)), [254, 254, 255, 255]);
        assert_eq!(write_once(|bb| bb.write_float(-f32::MIN_POSITIVE)), [1, 1, 0, 0]);
        assert_eq!(write_once(|bb| bb.write_float(f32::MIN_POSITIVE)), [1, 0, 0, 0]);
        assert_eq!(write_once(|bb| bb.write_float(f32::NEG_INFINITY)), [255, 1, 0, 0]);
        assert_eq!(write_once(|bb| bb.write_float(f32::INFINITY)), [255, 0, 0, 0]);
        assert_eq!(write_once(|bb| bb.write_float(f32::NAN)), [255, 0, 0, 128]);
        assert_eq!(write_once(|bb| bb.write_float(1.0e-40)), [0]);
    }

    #[test]
    fn write_string() {
        assert_eq!(write_once(|bb| bb.write_string("")), [0]);
        assert_eq!(write_once(|bb| bb.write_string("a")), [97, 0]);
        assert_eq!(write_once(|bb| bb.write_string("abc")), [97, 98, 99, 0]);
        assert_eq!(write_once(|bb| bb.write_string("🍕")), [240, 159, 141, 149, 0]);
    }
}
