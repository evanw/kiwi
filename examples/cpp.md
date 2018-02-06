# Kiwi demo: C++

The schema in this example is a file called `testv1.kiwi` that looks like this:

```proto
package testv1;

message Test {
  int x = 1;
}
```

Install the `kiwic` command using `npm install -g kiwi-schema`, then generate the header file from the schema using `kiwic --schema testv1.kiwi --cpp testv1.h`.
Download the file [kiwi.h](https://github.com/evanw/kiwi/blob/master/kiwi.h), then compile the file below using something like `c++ main.cpp -std=c++11`.

```cpp
#define IMPLEMENT_KIWI_H
#define IMPLEMENT_SCHEMA_H

#include <stdio.h>
#include "testv1.h"

int main() {
  testv1::Test message;
  message.set_x(123);

  kiwi::ByteBuffer buffer;
  bool encode_success = message.encode(buffer);

  testv1::Test message2;
  kiwi::MemoryPool pool;
  bool decode_success = message2.decode(buffer, pool);

  // Fields in a Kiwi "message" are optional, and will be nullptr if omitted
  if (message2.x() != nullptr) {
    printf("x is %d\n", *message2.x());
  }

  return 0;
}
```

## Forwards compatibility

Forwards compatibility means reading newer data from an older program.
It can be achieved by bundling the schema along with the data instead of baking the schema into the program.
Forwards compatibility should always be possible as long as the newer schema is backwards compatible with the older schema.

The binary schema format is useful for this purpose.
To convert the textual schema format to the binary schema format, use the command `kiwic --schema testv1.kiwi --binary testv1.bkiwi`.

For example, given the following version 2 of the schema in file `testv2.kiwi`:

```proto
package testv2;

message Test {
  int x = 1;
  int y = 2;
}
```

Convert that to the binary format using `kiwic --schema testv2.kiwi --binary testv2.bkiwi`, then generate the header file using `kiwic --schema testv2.kiwi --cpp testv2.h`.
Compile the following code to demonstrate forward compatibility:

```cpp
#define IMPLEMENT_KIWI_H
#define IMPLEMENT_SCHEMA_H

#include <stdio.h>
#include "testv1.h"
#include "testv2.h"

static void readFile(const char *name, kiwi::ByteBuffer &bb) {
  auto f = fopen(name, "rb");
  assert(f);
  int c;
  while ((c = fgetc(f)) != -1) bb.writeByte(c);
  fclose(f);
}

int main() {
  testv2::Test messageV2;
  messageV2.set_x(123);
  messageV2.set_y(234);
  kiwi::ByteBuffer bufferV2;
  bool encode_success = messageV2.encode(bufferV2);
  assert(encode_success);

  // // Decoding new data using the old version of the schema will fail
  // testv1::Test messageV1;
  // kiwi::MemoryPool pool;
  // bool decode_success = messageV1.decode(bufferV2, pool);
  // assert(decode_success);

  // Decoding old data from new data is possible with the new schema
  kiwi::ByteBuffer data;
  readFile("testv2.bkiwi", data);
  testv1::BinarySchema schema;
  bool parse_success = schema.parse(data);
  assert(parse_success);
  testv1::Test messageV1;
  kiwi::MemoryPool pool;
  bool decode_success = messageV1.decode(bufferV2, pool, &schema);
  assert(decode_success);

  printf("x is %d\n", *messageV1.x());
  return 0;
}
```

Notice how the `decode()` function takes an optional third argument: the schema that was provided along with the data.
Even though we're decoding using the older decoder, the older decoder can use the newer schema to skip over newer fields that it doesn't know about.
