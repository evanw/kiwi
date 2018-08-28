use std::str;
use std::path::{Path, PathBuf};

use types::{Enumerator, Field, FieldType, FileDescriptor, Frequency, Message, snake_case};
use nom::{digit, hex_digit, multispace};

fn is_word(b: u8) -> bool {
    match b {
        b'a'...b'z' | b'A'...b'Z' | b'0'...b'9' | b'_' | b'.' => true,
        _ => false,
    }
}

named!(
    word<String>,
    map_res!(take_while!(is_word), |b: &[u8]| String::from_utf8(
        b.to_vec()
    ))
);
named!(
    word_ref<&str>,
    map_res!(take_while!(is_word), str::from_utf8)
);

named!(
    hex_integer<i32>,
    do_parse!(
        tag!("0x")
            >> num:
        map_res!(
            map_res!(hex_digit, str::from_utf8),
            |s| i32::from_str_radix(s, 16)
        ) >> (num)
    )
);

named!(
    integer<i32>,
    map_res!(map_res!(digit, str::from_utf8), str::FromStr::from_str)
);

named!(
    comment<()>,
    do_parse!(tag!("//") >> take_until_and_consume!("\n") >> ())
);
named!(
    block_comment<()>,
    do_parse!(tag!("/*") >> take_until_and_consume!("*/") >> ())
);

/// word break: multispace or comment
named!(
    br<()>,
    alt!(map!(multispace, |_| ()) | comment | block_comment)
);

named!(
    package<String>,
    do_parse!(
        tag!("package") >> many1!(br) >> package: word >> many0!(br) >> tag!(";") >> (package)
    )
);

named!(
    num_range<Vec<i32>>,
    do_parse!(
        from_: integer >> many1!(br) >> tag!("to") >> many1!(br) >> to_: integer
            >> ((from_..(to_ + 1)).collect())
    )
);

named!(
    frequency<Frequency>,
    alt!(tag!("optional") => { |_| Frequency::Optional } |
         tag!("repeated") => { |_| Frequency::Repeated } |
         tag!("required") => { |_| Frequency::Required } )
);


named!(
    frequency_array_format<Option<&[u8]>>,
    opt!(complete!(tag!("[]")))
);

named!(
    field_type<FieldType>,
    alt!(tag!("int") => { |_| FieldType::Int32 } |
         tag!("uint") => { |_| FieldType::Uint32 } |
         tag!("bool") => { |_| FieldType::Bool } |
         tag!("string") => { |_| FieldType::String_ } |
         tag!("byte[]") => { |_| FieldType::Bytes } |
         tag!("float") => { |_| FieldType::Float } |
         word => { |w| FieldType::ReferenceType(w) })
);

named!(
    check_deprecated<bool>,
    alt!(tag!("[deprecated]") => { |_| true } |
         many0!(br) => { |_| false })
);

enum MessageEvent {
    Field(Field),
    Ignore,
}

// Struct fields are always required + cannot be deprecated
named!(
    struct_field<Field>,
    do_parse!(
        typ: field_type >> freq: frequency_array_format >>
            many1!(br) >> name: word >> many0!(br) >> tag!(";") >> (
                Field {
                    name: snake_case(&name),
                    frequency: match freq {
                        Some(_) => Frequency::Repeated,
                        None => Frequency::Required
                    },
                    typ: typ,
                    number: 0,
                    default: None,
                    boxed: false,
                    deprecated: false,
                    struct_field: true
                })
    )
);


named!(
    message_struct_event<MessageEvent>,
    alt!(struct_field => { |f| MessageEvent::Field(f) } |
         br => { |_| MessageEvent::Ignore })
);

named!(
    message_struct_events<(String, Vec<MessageEvent>)>,
    do_parse!(
        tag!("struct") >> many1!(br) >> name: word >> many0!(br) >> tag!("{") >> many0!(br)
            >> events: many0!(message_struct_event) >> many0!(br) >> tag!("}") >> many0!(br)
            >> many0!(tag!(";")) >> ((name, events))
    )
);

named!(
    message_struct<Message>,
    map!(
        message_struct_events,
        |(name, events): (String, Vec<MessageEvent>)| {
            let mut msg = Message {
                name: name.clone(),
                is_struct: true,
                ..Message::default()
            };
            for e in events {
                match e {
                    MessageEvent::Field(f) => msg.fields.push(f),
                    MessageEvent::Ignore => (),
                }
            }
            msg
        }
    )
);


// Kiwi makes everything optional by default
named!(
    message_field<Field>,
    do_parse!(
        typ: field_type >> freq: frequency_array_format >> many1!(br) >> name: word
            >> many0!(br) >> tag!("=") >> many0!(br) >> number: integer >> many0!(br)
            >> is_deprecated: check_deprecated >> many0!(br) >> tag!(";") >> (
                Field {
                    name: snake_case(&name),
                    frequency: match freq {
                        Some(_) => Frequency::Repeated,
                        None => Frequency::Optional
                    },
                    typ: typ,
                    number: number,
                    default: None,
                    boxed: false,
                    deprecated: is_deprecated,
                    struct_field: false
                })
    )
);


named!(
    message_event<MessageEvent>,
    alt!(message_field => { |f| MessageEvent::Field(f) } |
         br => { |_| MessageEvent::Ignore })
);

named!(
    message_events<(String, Vec<MessageEvent>)>,
    do_parse!(
        tag!("message") >> many1!(br) >> name: word >> many0!(br) >> tag!("{") >> many0!(br)
            >> events: many0!(message_event) >> many0!(br) >> tag!("}") >> many0!(br)
            >> many0!(tag!(";")) >> ((name, events))
    )
);


named!(
    message<Message>,
    map!(
        message_events,
        |(name, events): (String, Vec<MessageEvent>)| {
            let mut msg = Message {
                name: name.clone(),
                is_struct: false,
                ..Message::default()
            };
            for e in events {
                match e {
                    MessageEvent::Field(f) => msg.fields.push(f),
                    MessageEvent::Ignore => (),
                }
            }
            msg
        }
    )
);

named!(
    enum_field<(String, i32)>,
    do_parse!(
        name: word >> many0!(br) >> tag!("=") >> many0!(br) >> number: alt!(hex_integer | integer)
            >> many0!(br) >> tag!(";") >> many0!(br) >> ((name, number))
    )
);

named!(
    enumerator<Enumerator>,
    do_parse!(
        tag!("enum") >> many1!(br) >> name: word >> many0!(br) >> tag!("{") >> many0!(br)
            >> fields: many0!(enum_field) >> many0!(br) >> tag!("}") >> many0!(br)
            >> many0!(tag!(";")) >> (Enumerator {
                package: "".to_string(),
                name: name,
                fields: fields,
                imported: false,
                module: "".to_string(),
                import: PathBuf::new(),
                path: PathBuf::new(),
            })
    )
);

named!(
    option_ignore<()>,
    do_parse!(tag!("option") >> many1!(br) >> take_until_and_consume!(";") >> ())
);

enum Event {
    Package(String),
    Message(Message),
    Enum(Enumerator),
    Ignore,
}

named!(
    event<Event>,
    alt!(
        package => { |p| Event::Package(p) } |
        message => { |m| Event::Message(m) } |
        message_struct => { |m| Event::Message(m) } |
        enumerator => { |e| Event::Enum(e) } |
        option_ignore => { |_| Event::Ignore } |
        br => { |_| Event::Ignore })
);

named!(pub file_descriptor<FileDescriptor>,
       map!(many0!(event), |events: Vec<Event>| {
           let mut desc = FileDescriptor::default();
           for event in events {
               match event {
                   Event::Package(p) => desc.package = p,
                   Event::Message(m) => desc.messages.push(m),
                   Event::Enum(e) => desc.enums.push(e),
                   Event::Ignore => (),
               }
           }
           desc
       }));

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_message() {
        let msg = r#"message ReferenceData
    {
        ScenarioInfo  scenarioSet = 1;
        CalculatedObjectInfo calculatedObjectSet = 2;
        RiskFactorList riskFactorListSet = 3;
        RiskMaturityInfo riskMaturitySet = 4;
        IndicatorInfo indicatorSet = 5;
        RiskStrikeInfo riskStrikeSet = 6;
        FreeProjectionList freeProjectionListSet = 7;
        ValidationProperty ValidationSet = 8;
        CalcProperties calcPropertiesSet = 9;
        MaturityInfo maturitySet = 10;
    }"#;

        let mess = message(msg.as_bytes());
        if let ::nom::IResult::Done(_, mess) = mess {
            assert_eq!(10, mess.fields.len());
        } else {
            panic!("Failed to parse message");
        }
    }



    #[test]
    fn test_message_2() {
        let msg = r#"message MessagePass
    {
        MessageType type = 1;
        uint sessionID = 2;
        uint ackID = 3;
        string signalName = 7;
        Access access = 8;
        string styleSetName = 9;
        StyleSetType styleSetType = 10;
        StyleSetContentType styleSetContentType = 11;
        int pasteID = 12;
        Vector pasteOffset = 13;
        string pasteFileKey = 14;
        string signalPayload = 15;
      }"#;

        let mess = message(msg.as_bytes());
        if let ::nom::IResult::Done(_, mess) = mess {
            assert_eq!(12, mess.fields.len());
        } else {
            panic!("Failed to parse message");
        }
    }


    #[test]
    fn test_message_lists() {
        let msg = r#"message MessagePass
    {
        MessageType type = 1;
        uint[] sessionID = 2;
        uint[] ackID = 3;
        string signalName = 7;
        Access access = 8;
        string styleSetName = 9;
        StyleSetType styleSetType = 10;
        StyleSetContentType styleSetContentType = 11;
        int pasteID = 12;
        Vector pasteOffset = 13;
        string[] pasteFileKey = 14;
        string[] signalPayload = 15;
      }"#;

        let mess = message(msg.as_bytes());
        if let ::nom::IResult::Done(_, mess) = mess {
            assert_eq!(12, mess.fields.len());
        } else {
            panic!("Failed to parse message");
        }
    }


    #[test]
    fn test_mstruct() {
        let msg = r#"struct Matrix {
            float m00;
            float m01;
            float m02;
            float m10;
            float m11;
            float m12;
        }"#;

        let mess = message_struct(msg.as_bytes());
        if let ::nom::IResult::Done(_, mess) = mess {
            assert_eq!(6, mess.fields.len());
        } else {
            panic!("Failed to parse struct");
        }
    }


    #[test]
    fn test_mstruct2() {
        let msg = r#"struct ExportConstraint {
        ExportConstraintType type;
        float value;
      }"#;

        let mess = message_struct(msg.as_bytes());
        if let ::nom::IResult::Done(_, mess) = mess {
            assert_eq!(2, mess.fields.len());
        } else {
            panic!("Failed to parse struct");
        }
    }

    #[test]
    fn test_enum() {
        let msg = r#"enum PairingStatus {
                DEALPAIRED        = 0;
                INVENTORYORPHAN   = 1;
                CALCULATEDORPHAN  = 2;
                CANCELED          = 3;
    }"#;

        let mess = enumerator(msg.as_bytes());
        if let ::nom::IResult::Done(_, mess) = mess {
            assert_eq!(4, mess.fields.len());
        }
    }

    #[test]
    fn test_ignore() {
        let msg = r#"option optimize_for = SPEED;"#;

        match option_ignore(msg.as_bytes()) {
            ::nom::IResult::Done(_, _) => (),
            e => panic!("Expecting done {:?}", e),
        }
    }

    #[test]
    fn test_nested_message() {
        let msg = r#"message A
    {
        message B {
            repeated int32 a = 1;
            optional string b = 2;
        }
        optional b = 1;
    }"#;

        let mess = message(msg.as_bytes());
        if let ::nom::IResult::Done(_, mess) = mess {
            assert!(mess.messages.len() == 1);
        }
    }
}
