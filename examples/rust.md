# Kiwi demo: Rust

The schema in this example is a file called `test.kiwi` that looks like this:

```proto
message Test {
  int x = 1;
}
```

Install the `kiwic` command using `npm install -g kiwi-schema`, then convert the textual schema to the binary schema format using `kiwic --schema test.kiwi --binary test.bkiwi`.
Add a dependency on the [kiwi-schema](https://crates.io/crates/kiwi-schema) crate to your Cargo.toml file.
Then the following code should build and run:

```rust
extern crate kiwi_schema;
use kiwi_schema::*;
use std::collections::HashMap;

fn main() {
  let schema = Schema::decode(include_bytes!("test.bkiwi")).unwrap();
  let message = Value::Object("Test", {
    let mut map = HashMap::new();
    map.insert("x", Value::Int(123));
    map
  });
  let buffer = message.encode(&schema);
  let def = schema.def("Test").unwrap();
  let test = Value::decode(&schema, def.index, &buffer).unwrap();

  // Fields in a Kiwi "message" are optional, and will be None if omitted
  if let Some(x) = test.get("x") {
    println!("x is {}", x.as_int());
  }
}
```

See the [crate documentation](https://docs.rs/kiwi-schema/*/kiwi_schema/) for more details.

## Forwards compatibility

Forwards compatibility means reading newer data from an older program.
It can be achieved by bundling the schema along with the data instead of baking the schema into the program.
Forwards compatibility should always be possible as long as the newer schema is backwards compatible with the older schema.
