# Kiwi demo: Skew

This demo is for the [Skew programming language](http://skew-lang.org/).
The schema in this example is a file called `testv1.kiwi` that looks like this:

```
package testv1;

message Test {
  int x = 1;
}
```

Install the `kiwic` command using `npm install -g kiwi-schema`, then generate the Skew source code from the schema using `kiwic --schema testv1.kiwi --skew testv1.sk`.
Download the files [kiwi.sk](https://github.com/evanw/kiwi/blob/master/kiwi.sk) and [typedarray.sk](https://github.com/evanw/kiwi/blob/master/typedarray.sk), then compile the file below using something like `skewc *.sk --output-file=out.js`.

```coffee
@import
var console dynamic

@entry
def main int {
  var message = testv1.Test.new
  message.x = 123

  var buffer = message.encode
  var message2 = testv1.Test.decode(buffer)

  # Fields in a Kiwi "message" are optional, and can be tested for presence like this
  if message2.has_x {
    console.log("x is \(message2.x)")
  }

  return 0
}
```

## Forwards compatibility

Forwards compatibility means reading newer data from an older program.
It can be achieved by bundling the schema along with the data instead of baking the schema into the program.
Forwards compatibility should always be possible as long as the newer schema is backwards compatible with the older schema.

The binary schema format is useful for this purpose.
To convert the textual schema format to the binary schema format, use the command `kiwic --schema testv1.kiwi --binary testv1.bkiwi`.

For example, given the following version 2 of the schema in file `testv2.kiwi`:

```
package testv2;

message Test {
  int x = 1;
  int y = 2;
}
```

Convert that to the binary format using `kiwic --schema testv2.kiwi --binary testv2.bkiwi`, then generate the Skew source code from the schema using `kiwic --schema testv2.kiwi --skew testv2.sk`.
Compile the following code to demonstrate forward compatibility:

```coffee
@import {
  var console dynamic
  var require dynamic
}

@entry
def main int {
  var messageV2 = testv2.Test.new
  messageV2.x = 123
  messageV2.y = 234
  var bufferV2 = messageV2.encode

  # # Decoding new data using the old version of the schema will fail
  # var messageV1 = testv1.Test.decode(bufferV2)

  # Decoding old data from new data is possible with the new schema
  var fs = require("fs")
  var schema = testv1.BinarySchema.new
  schema.parse(Uint8Array.new(fs.readFileSync("testv2.bkiwi")))
  var messageV1 = testv1.Test.decode(bufferV2, schema)

  console.log("x is \(messageV1.x)")
  return 0
}
```

Notice how the `decode()` function takes an optional second argument: the schema that was provided along with the data.
Even though we're decoding using the older decoder, the older decoder can use the newer schema to skip over newer fields that it doesn't know about.
