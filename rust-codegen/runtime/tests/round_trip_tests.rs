#[macro_use] extern crate proptest;
extern crate quick_kiwi;

use quick_kiwi::{ Writer, BytesReader};
use quick_kiwi::errors::Result;
use proptest::prelude::*;
use proptest::test_runner::Config;

fn write_once<F>(cb: F) -> Vec<u8>
where
    F: Fn(&mut Writer<&mut Vec<u8>>) -> Result<()>
{
    let mut buffer = Vec::new();
    {
        let mut writer = Writer::new(&mut buffer);
        cb(&mut writer);
    }
    buffer
}

proptest! {
    #![proptest_config(Config::with_source_file("tests/round_trip_tests.rs"))]

    #[test]
    fn handle_u8(x in any::<u8>()) {
        let w = write_once(|bb| bb.write_u8(x));
        let r = BytesReader::from_bytes(&w).read_u8(&w).unwrap();
        prop_assert_eq!(x, r);
    }

    #[test]
    fn handle_tag(x in any::<u32>()) {
        let w = write_once(|bb| bb.write_tag(x));
        let r = BytesReader::from_bytes(&w).read_uint32(&w).unwrap();
        prop_assert_eq!(x, r);
    }

    #[test]
    fn handle_int32(x in any::<i32>()) {
        let w = write_once(|bb| bb.write_int32(x));
        let r = BytesReader::from_bytes(&w).read_int32(&w).unwrap();
        prop_assert_eq!(x, r);
    }

    #[test]
    fn handle_uint32(x in any::<u32>()) {
        let w = write_once(|bb| bb.write_uint32(x));
        let r = BytesReader::from_bytes(&w).read_uint32(&w).unwrap();
        prop_assert_eq!(x, r);
    }

    #[test]
    fn handle_float(x in any::<f32>()) {
        let w = write_once(|bb| bb.write_float(x));
        let r = BytesReader::from_bytes(&w).read_float(&w).unwrap();
        prop_assert_eq!(x, r);
    }

    #[test]
    fn handle_bool(x in any::<bool>()) {
        let w = write_once(|bb| bb.write_bool(x));
        let r = BytesReader::from_bytes(&w).read_bool(&w).unwrap();
        prop_assert_eq!(x, r);
    }

    #[test]
    fn handle_enum(x in any::<u32>()) {
        let w = write_once(|bb| bb.write_uint32(x));
        let r = BytesReader::from_bytes(&w).read_uint32(&w).unwrap();
        prop_assert_eq!(x, r);
    }

    #[test]
    fn handle_bytes(x in prop::collection::vec(any::<u8>(), 0..20)) {
        let w = write_once(|bb| bb.write_bytes(&x));
        let r = BytesReader::from_bytes(&w).read_bytes(&w).unwrap();
        prop_assert_eq!(x, r);
    }

    #[test]
    fn handle_string(x in "\\PC*") {
        let w = write_once(|bb| bb.write_string(&x));
        let r = BytesReader::from_bytes(&w).read_string(&w).unwrap();
        prop_assert_eq!(x, r);
    }
}
