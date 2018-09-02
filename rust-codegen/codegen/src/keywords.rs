static RUST_KEYWORDS: [&'static str; 76] = [
    "abstract",
    "alignof",
    "as",
    "become",
    "bool",
    "box",
    "Box",
    "break",
    "BytesReader",
    "const",
    "continue",
    "crate",
    "Cow",
    "Default",
    "do",
    "else",
    "enum",
    "Err",
    "extern",
    "f32",
    "f64",
    "false",
    "final",
    "fn",
    "for",
    "HashMap",
    "i32",
    "i64",
    "if",
    "impl",
    "in",
    "let",
    "loop",
    "match",
    "mod",
    "move",
    "mut",
    "None",
    "MessageWrite",
    "offsetof",
    "Ok",
    "Option",
    "override",
    "priv",
    "proc",
    "pub",
    "pure",
    "ref",
    "Result",
    "return",
    "self",
    "Self",
    "sizeof",
    "Some",
    "static",
    "str",
    "String",
    "struct",
    "super",
    "trait",
    "true",
    "type",
    "typeof",
    "u8",
    "u32",
    "u64",
    "unsafe",
    "unsized",
    "use",
    "Vec",
    "virtual",
    "where",
    "while",
    "Write",
    "Writer",
    "yield",
];

/// Ckeck is the identifier is a rust keyword and appends a '_kw' if that's the case
pub fn sanitize_keyword(ident: &mut String) {
    if !ident.contains('.') && RUST_KEYWORDS.contains(&&**ident) {
        ident.push_str("_kw");
    } else {
        *ident = ident
            .split('.')
            .map(|s| {
                if RUST_KEYWORDS.contains(&s) {
                    format!("{}_kw", s)
                } else {
                    s.to_string()
                }
            })
            .collect::<Vec<_>>()
            .join(".");
    }
}
