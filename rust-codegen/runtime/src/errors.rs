//! A module to handle all errors via error-chain crate

use std::io;
use failure::Fail;

/// An error enum which derives `Fail`
#[derive(Debug, Fail)]
pub enum Error {
    /// Io error
    #[fail(display = "{}", _0)]
    Io(#[cause] io::Error),

    /// Utf8 Error
    #[fail(display = "{}", _0)]
    Utf8(#[cause] ::std::str::Utf8Error),

    /// Deprecated feature (in kiwi specification)
    #[fail(display = "Feature '{}' has been deprecated", _0)]
    Deprecated(&'static str),

    /// Unknown wire type
    #[fail(display = "Unknown wire type '{}', must be less than 6", _0)]
    UnknownWireType(u8, String),

    /// Varint decoding error
    #[fail(display = "Cannot decode varint")]
    Varint,

    /// Error while parsing kiwi message
    #[fail(display = "Error while parsing message: {}", _0)]
    Message(String)
}

/// A wrapper for `Result<T, Error>`
pub type Result<T> = ::std::result::Result<T, Error>;

impl Into<io::Error> for Error {
    fn into(self) -> ::std::io::Error {
        match self {
            Error::Io(x) => x,
            Error::Utf8(x) => io::Error::new(io::ErrorKind::InvalidData, x),
            x => io::Error::new(io::ErrorKind::Other, x.compat()),
        }
    }
}

impl From<io::Error> for Error {
    fn from(e: io::Error) -> Error {
        Error::Io(e)
    }
}

impl From<::std::str::Utf8Error> for Error {
    fn from(e: ::std::str::Utf8Error) -> Error {
        Error::Utf8(e)
    }
}
