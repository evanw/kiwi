# quick-kiwi

This is a _very_ heavily modified version of https://github.com/tafia/quick-protobuf
converted to handling the kiwi file format. All code for features unsupported by kiwi
has been removed.

Changes from quick-proto:
* Drops support for sizeof 
* Drops support for nested message definitions
* Drops support for imports
* Drops support for ref types
* Drops support for one of types
* Adds support for kiwi packed struct types
* Adds support for a lazily parsed byte slice only representation
* Adds support for skip_reading methods, skipping whatever operations that otherwise might deserialize instead of just slicing bytes
* Adds support for individually de-serializing individual fields from the lazy representation
* Adds support for conversion from the byte slice representation to an owned Vec representation
* Adds code from existing rust parser for Schema parsing and validation
* Modifies readers + writers to match kiwi format

Contrast to `kiwi/rust`
* Generates type safe interfaces for accessing fields without unwrapping a large enum + a struct representation
* Lazy representation is many times faster to deserialize in most cases
* Aggressively inlines deserialization methods + initializes collections with capacities
