//! A module to manage kiwi deserialization
//!
//! There are actually two main *readers*
//! - a `BytesReader` which parses data from a `&[u8]`
//! - a `Reader` which is a wrapper on `BytesReader` which has its own buffer. It provides
//! convenient functions to the user suche as `from_file`
//!
//! It is advised, for convenience to directly work with a `Reader`.

use std::io::{self, Read};
use std::path::Path;
use std::fs::File;
use std::f32;

use errors::{Error, Result};
use message::{LazyMessageRead, MessageRead };

const WIRE_TYPE_VARINT: u8 = 0;
const WIRE_TYPE_FIXED64: u8 = 1;
const WIRE_TYPE_LENGTH_DELIMITED: u8 = 2;
const WIRE_TYPE_START_GROUP: u8 = 3;
const WIRE_TYPE_END_GROUP: u8 = 4;
const WIRE_TYPE_FIXED32: u8 = 5;


/// A struct to read protocol binary files
///
/// # Examples
///
/// ```rust
/// # mod foo_bar {
/// #     use quick_kiwi::{MessageRead, BytesReader, Result};
/// #     pub struct Foo {}
/// #     pub struct Bar {}
/// #     pub struct FooBar { pub foos: Vec<Foo>, pub bars: Vec<Bar>, }
/// #     impl<'a> MessageRead<'a> for FooBar {
/// #         fn from_reader(_: &mut BytesReader, _: &[u8]) -> Result<Self> {
/// #              Ok(FooBar { foos: vec![], bars: vec![] })
/// #         }
/// #     }
/// # }
///
/// // FooBar is a message generated from a proto file
/// // in particular it contains a `from_reader` function
/// use foo_bar::FooBar;
/// use quick_kiwi::{MessageRead, BytesReader};
///
/// fn main() {
///     // bytes is a buffer on the data we want to deserialize
///     // typically bytes is read from a `Read`:
///     // r.read_to_end(&mut bytes).expect("cannot read bytes");
///     let mut bytes: Vec<u8>;
///     # bytes = vec![];
///
///     // we can build a bytes reader directly out of the bytes
///     let mut reader = BytesReader::from_bytes(&bytes);
///
///     // now using the generated module decoding is as easy as:
///     let foobar = FooBar::from_reader(&mut reader, &bytes).expect("Cannot read FooBar");
///
///     // if instead the buffer contains a length delimited stream of message we could use:
///     // while !r.is_eof() {
///     //     let foobar: FooBar = r.read_message(&bytes).expect(...);
///     //     ...
///     // }
///     println!("Found {} foos and {} bars", foobar.foos.len(), foobar.bars.len());
/// }
/// ```
#[derive(Debug, Clone, PartialEq)]
pub struct BytesReader {
    /// current index into byte slice
    pub start: usize,
    /// end of the byte slice
    pub end: usize,
}

// TODO: Caching lookup on BytesReader
// TODO: Only on messages, structs, and vec

impl BytesReader {
    /// Creates a new reader from chunks of data
    pub fn from_bytes(bytes: &[u8]) -> BytesReader {
        BytesReader {
            start: 0,
            end: bytes.len(),
        }
    }

    /// Reads next tag, `None` if all bytes have been read
    #[inline(always)]
    pub fn next_tag(&mut self, bytes: &[u8]) -> Result<u32> {
        self.read_uint32(bytes)
    }

    /// Reads u8 byte
    #[inline(always)]
    fn read_u8(&mut self, bytes: &[u8]) -> Result<u8> {
        let b = bytes.get(self.start).ok_or_else::<Error, _>(|| {
            Error::Io(io::Error::new(
                io::ErrorKind::UnexpectedEof,
                "Cannot read next bytes",
            ))
        })?;
        self.start += 1;
        Ok(*b)
    }

    /// Skips u8 and returns the byte slice it occupies
    #[inline(always)]
    fn skip_u8<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]> {
        let bytes = bytes;
        self.start += 1;
        Ok(&bytes[self.start-1..self.start])
    }

    /// Reads int32 (varint)
    #[inline]
    pub fn read_int32(&mut self, bytes: &[u8]) -> Result<i32> {
        let value = self.read_uint32(bytes)?;
        Ok((if (value & 1) != 0 { !(value >> 1) } else { value >> 1 }) as i32)
    }

    /// Skips int32 and returns the byte slice it occupies
    #[inline(always)]
    pub fn skip_int32<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]> {
        self.skip_uint32(bytes)
    }

    /// Reads uint32 (varint)
    #[inline]
    pub fn read_uint32(&mut self, bytes: &[u8]) -> Result<u32> {
        let mut shift: u8 = 0;
        let mut result: u32 = 0;

        loop {
            let byte = self.read_u8(bytes)?;
            result |= ((byte & 127) as u32) << shift;
            shift += 7;

            if (byte & 128) == 0 || shift >= 35 {
                break;
            }
        }

        Ok(result)
    }

    /// Skips uint32 and returns the byte slice it occupies
    #[inline(always)]
    pub fn skip_uint32<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]> {
        let bytes = bytes;
        let init_start = self.start;
        let mut shift: u8 = 0;
        // let mut result: u32 = 0;
        loop {
            let byte = self.read_u8(bytes).unwrap();
            // result |= ((byte & 127) as u32) << shift;
            shift += 7;

            if (byte & 128) == 0 || shift >= 35 {
                break;
            }
        }
        Ok(&bytes[init_start..self.start])
    }

    /// Reads float (little endian f32)
    #[inline]
    pub fn read_float(&mut self, bytes: &[u8]) -> Result<f32> {
        // self.read_fixed(bytes, 4, LE::read_f32)
        let first = self.read_u8(bytes)?;

        // KIWI: Optimization: use a single byte to store zero
        if first == 0 {
            Ok(0.0)
        } else if self.start + 3 > self.end {
            Err(Error::Message("read float failure".to_string()))
        }

        // Endian-independent 32-bit read
        else {
            let mut bits: u32 =
                first as u32 |
            ((bytes[self.start] as u32) << 8) |
            ((bytes[self.start + 1] as u32) << 16) |
            ((bytes[self.start + 2] as u32) << 24);
            self.start += 3;

            // Move the exponent back into place
            bits = (bits << 23) | (bits >> 9);

            Ok(f32::from_bits(bits))
        }
    }

    /// Skips float and returns the byte slice it occupies
    #[inline]
    pub fn skip_float<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]> {
        let init_start = self.start;
        let first = self.read_u8(bytes).unwrap();

        // KIWI: Optimization: use a single byte to store zero
        if first == 0 {
            Ok(&bytes[init_start..self.start])
        } else if self.start + 3 > self.end {
            Err(Error::Message("read float failure".to_string()))
        }

        // Endian-independent 32-bit read
        else {
            self.start += 3;
            Ok(&bytes[init_start..self.start])
        }
    }

    /// Reads bool (varint, check if == 0)
    #[inline]
    pub fn read_bool(&mut self, bytes: &[u8]) -> Result<bool> {
        match self.read_u8(bytes) {
            Ok(0) => Ok(false),
            Ok(1) => Ok(true),
            _ => Err(Error::Message("Bool failure".to_string())),
        }
    }

    /// Skips bool and returns the byte slice it occupies
    #[inline]
    pub fn skip_bool<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]> {
        self.skip_u8(bytes)
    }

    /// Reads enum, encoded as u32
    #[inline]
    pub fn read_enum<E: From<u32>>(&mut self, bytes: &[u8]) -> Result<E> {
        self.read_uint32(bytes).map(|e| e.into())
    }

    /// Skips enum and returns the byte slice it occupies
    #[inline]
    pub fn skip_enum<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]> {
        self.skip_uint32(bytes)
    }

    /// Read len byte range
    #[inline(always)]
    fn read_len<'a, M, F>(&mut self, bytes: &'a [u8], mut read: F) -> Result<M>
    where
        F: FnMut(&mut BytesReader, &'a [u8]) -> Result<M>,
    {
        let len = self.read_uint32(bytes)? as usize;
        let cur_end = self.end;
        self.end = self.start + len;
        let v = read(self, bytes)?;
        self.start = self.end;
        self.end = cur_end;
        Ok(v)
    }

    /// Skips len byte range and returns the byte slice it occupies
    #[inline(always)]
    fn skip_len<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]> {
        let len = self.read_uint32(bytes).unwrap() as usize;
        let init_start = self.start;
        self.start = self.start + len;
        Ok(&bytes[init_start..self.start])
    }

    /// Reads bytes (Vec<u8>)
    #[inline]
    pub fn read_bytes<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]> {
        let len = self.read_uint32(bytes).unwrap() as usize;
        let init_start = self.start;
        self.start = self.start + len;
        Ok(&bytes[init_start..self.start])
    }

    /// Skips bytes and returns the byte slice it occupies
    #[inline]
    pub fn skip_bytes<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]> {
        self.skip_len(bytes)
    }

    /// Reads string (String)
    #[inline]
    pub fn read_string<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a str> {
        let start = self.start;
        while self.start < self.end {
            if bytes[self.start] == 0 {
                self.start += 1;
                return ::std::str::from_utf8(&bytes[start..self.start - 1]).map_err(|e| e.into())
            }

            self.start += 1;
        }
        Err(Error::Message("String parse failure".to_string()))
    }

    /// Skips a string and returns the byte slice it occupies
    #[inline]
    pub fn skip_string<'a>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]> {
        let init_start = self.start;
        while self.start < self.end {
            if bytes[self.start] == 0 {
                self.start += 1;
                return Ok(&bytes[init_start..self.start - 1]);
            }

            self.start += 1;
        }
        Err(Error::Message("String skip failure".to_string()))
    }

    /// Reads packed repeated field (Vec<M>)
    ///
    /// Note: packed field are stored as a variable length chunk of data, while regular repeated
    /// fields behaves like an iterator, yielding their tag everytime
    #[inline]
    pub fn read_packed<'a, M, F>(&mut self, bytes: &'a [u8], mut read: F) -> Result<Vec<M>>
    where
        F: FnMut(&mut BytesReader, &'a [u8]) -> Result<M>,
    {
        let len = self.read_uint32(bytes)? as usize;
        let mut v = Vec::with_capacity(len);
        if len != 0 {
            for _ in 0..len {
                v.push(read(self, bytes)?);
            }
        }
        Ok(v)
    }

    /// Skips through the packed list, turning the entire sublist
    /// into lazy structs
    #[inline]
    pub fn lazy_packed<'a, M, F>(&mut self, bytes: &'a [u8], mut lazy_read: F) -> Result<Vec<M>>
    where
        F: FnMut(&mut BytesReader, &'a [u8]) -> Result<M>,
    {
        let len = self.read_uint32(bytes)? as usize;
        let mut v = Vec::with_capacity(len);
        if len != 0 {
            for _ in 0..len {
                v.push(lazy_read(self, bytes)?);
            }
        }
        Ok(v)
    }

    /// Skips through the packed list, turning the entire sublist
    /// into lazy structs
    #[inline]
    pub fn lazy_packed_to_packed<'a, M, F>(&mut self, bytes: &'a [u8], mut lazy_read: F) -> Result<Vec<M>>
    where
        F: FnMut(&mut BytesReader, &'a [u8]) -> Result<M>,
    {
        let len = self.read_uint32(bytes)? as usize;
        let mut v = Vec::with_capacity(len);
        if len != 0 {
            for _ in 0..len {
                v.push(lazy_read(self, bytes)?);
            }
        }
        Ok(v)
    }


    /// Reads packed repeated field (Vec<M>)
    ///
    /// Note: packed field are stored as a variable length chunk of data, while regular repeated
    /// fields behaves like an iterator, yielding their tag everytime
    #[inline]
    pub fn read_packed_print<'a, M, F>(&mut self, bytes: &'a [u8], mut read: F) -> Result<Vec<M>>
        where
        F: FnMut(&mut BytesReader, &'a [u8]) -> Result<M>,
    {
        let len = self.read_uint32(bytes)? as usize;
        println!("count: {}", len);
        let mut v = Vec::with_capacity(len);
        if len != 0 {
            for c in 0..len {
                println!("item {} of {}", c, len);
                v.push(read(self, bytes)?);
            }
        }
        Ok(v)
    }

    /// Reads a nested message
    #[inline]
    pub fn read_message<'a, M>(&mut self, bytes: &'a [u8]) -> Result<M>
    where
        M: MessageRead<'a>,
    {
        M::from_reader(self, bytes)
    }

    /// Lazy reads a nested message
    #[inline]
    pub fn lazy_read_message<'a, M>(&mut self, bytes: &'a [u8]) -> Result<M>
    where
        M: LazyMessageRead<'a>,
    {
        M::from_lazy_reader(self, bytes)
    }

    /// Lazy reads a nested message as a slice instead of Lazy struct
    #[inline]
    pub fn lazy_read_message_slice<'a, M>(&mut self, bytes: &'a [u8]) -> Result<&'a [u8]>
    where
        M: LazyMessageRead<'a>,
    {
        M::from_lazy_reader_slice(self, bytes)
    }

    /// Reads unknown data, based on its tag value (which itself gives us the wire_type value)
    #[inline]
    pub fn read_unknown(&mut self, _bytes: &[u8], tag_value: u32, name: &str) -> Result<()> {
        panic!("read_unknown tag: {:?} name: {} location: {} end: {}",
               tag_value, name, self.start, self.end);
    }

    /// Gets the remaining length of bytes not read yet
    #[inline(always)]
    pub fn len(&self) -> usize {
        self.end - self.start
    }

    /// Checks if `self.len == 0`
    #[inline(always)]
    pub fn is_eof(&self) -> bool {
        self.start == self.end
    }

    /// Advance inner cursor to the end
    pub fn read_to_end(&mut self) {
        self.start = self.end;
    }
}

/// A struct to read kiwi data
///
/// Contrary to `BytesReader`, this struct will own a buffer
///
/// # Examples
///
/// ```rust,should_panic
/// # mod foo_bar {
/// #     use quick_kiwi::{MessageRead, BytesReader, Result};
/// #     pub struct Foo {}
/// #     pub struct Bar {}
/// #     pub struct FooBar { pub foos: Vec<Foo>, pub bars: Vec<Bar>, }
/// #     impl<'a> MessageRead<'a> for FooBar {
/// #         fn from_reader(_: &mut BytesReader, _: &[u8]) -> Result<Self> {
/// #              Ok(FooBar { foos: vec![], bars: vec![] })
/// #         }
/// #     }
/// # }
///
/// // FooBar is a message generated from a proto file
/// // In particular it implements the `MessageRead` trait, containing a `from_reader` function.
/// use foo_bar::FooBar;
/// use quick_kiwi::Reader;
///
/// fn main() {
///     // create a reader, which will parse the kiwi binary file and pop events
///     // this reader will read the entire file into an internal buffer
///     let mut reader = Reader::from_file("/path/to/binary/kiwi.bin")
///         .expect("Cannot read input file");
///
///     // Use the generated module fns with the reader to convert your data into rust structs.
///     //
///     // Depending on your input file, the message can or not be prefixed with the encoded length
///     // for instance, a *stream* which contains several messages generally split them using this
///     // technique (see https://developers.google.com/protocol-buffers/docs/techniques#streaming)
///     //
///     // To read a message without a length prefix you can directly call `FooBar::from_reader`:
///     // let foobar = reader.read(FooBar::from_reader).expect("Cannot read FooBar message");
///     //
///     // Else to read a length then a message, you can use:
///     let foobar: FooBar = reader.read(|r, b| r.read_message(b))
///         .expect("Cannot read FooBar message");
///     // Reader::read_message uses `FooBar::from_reader` internally through the `MessageRead`
///     // trait.
///
///     println!("Found {} foos and {} bars!", foobar.foos.len(), foobar.bars.len());
/// }
/// ```
pub struct Reader {
    buffer: Vec<u8>,
    inner: BytesReader,
}

impl Reader {
    /// Creates a new `Reader`
    pub fn from_reader<R: Read>(mut r: R, capacity: usize) -> Result<Reader> {
        let mut buf = Vec::with_capacity(capacity);
        unsafe {
            buf.set_len(capacity);
        }
        buf.shrink_to_fit();
        r.read_exact(&mut buf)?;
        Ok(Reader::from_bytes(buf))
    }

    /// Creates a new `Reader` out of a file path
    pub fn from_file<P: AsRef<Path>>(src: P) -> Result<Reader> {
        let len = src.as_ref().metadata().unwrap().len() as usize;
        let f = File::open(src)?;
        Reader::from_reader(f, len)
    }

    /// Creates a new reader consuming the bytes
    pub fn from_bytes(bytes: Vec<u8>) -> Reader {
        let reader = BytesReader {
            start: 0,
            end: bytes.len(),
        };
        Reader {
            buffer: bytes,
            inner: reader,
        }
    }

    /// Run a `BytesReader` dependent function
    #[inline]
    pub fn read<'a, M, F>(&'a mut self, mut read: F) -> Result<M>
    where
        F: FnMut(&mut BytesReader, &'a [u8]) -> Result<M>,
    {
        read(&mut self.inner, &self.buffer)
    }

    /// Gets the inner `BytesReader`
    pub fn inner(&mut self) -> &mut BytesReader {
        &mut self.inner
    }

    /// Gets the buffer used internally
    pub fn buffer(&self) -> &[u8] {
        &self.buffer
    }
}

#[test]
fn test_varint() {
    let data = [0x96, 0x01];
    let mut r = BytesReader::from_bytes(&data[..]);
    assert_eq!(150, r.read_uint32(&data[..]).unwrap());
    assert!(r.is_eof());
}
