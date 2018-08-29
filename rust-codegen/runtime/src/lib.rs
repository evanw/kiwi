//! A library to read binary kiwi files
//!
//! This reader is developed similarly to a pull reader

#![deny(missing_docs)]
#![allow(dead_code)]

extern crate byteorder;
extern crate failure;

#[macro_use]
extern crate failure_derive;

pub mod errors;
pub mod message;
pub mod reader;
pub mod writer;
pub mod schema;

pub use errors::{Result, Error};
pub use message::{LazyMessageRead, MessageRead, MessageWrite};
pub use reader::{BytesReader, Reader};
pub use writer::Writer;
