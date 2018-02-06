# Kiwi demo: JavaScript

The schema in this example is a file called `test.kiwi` that looks like this:

```
message Test {
  int x = 1;
}
```

Install the kiwi package using `npm install kiwi-schema`.
Then the following code should work:

```js
var kiwi = require('kiwi-schema');
var fs = require('fs');

var schema = kiwi.compileSchema(fs.readFileSync('test.kiwi', 'utf8'));
var buffer = schema.encodeTest({x: 123});
var test = schema.decodeTest(buffer);

// Fields in a Kiwi "message" are optional, and will be undefined if omitted
if (test.x !== undefined) {
  console.log('x is', test.x);
}
```

## TypeScript

It's also easy to use Kiwi with TypeScript.
Run `node node_modules/.bin/kiwic --schema test.kiwi --ts test.ts` to generate TypeScript type definitions for a schema.
That will generate a file that looks like this:

```ts
export interface Test {
  x?: number;
}

export interface Schema {
  encodeTest(message: Test): Uint8Array;
  decodeTest(buffer: Uint8Array): Test;
}
```

Which can be used like this:

```ts
import {Test, Schema} from './test';
import {compileSchema} from 'kiwi-schema';
import {readFileSync} from 'fs';

let schema: Schema = compileSchema(readFileSync('test.kiwi', 'utf8'));
let buffer: Uint8Array = schema.encodeTest({x: 123});
let test: Test = schema.decodeTest(buffer);

if (test.x !== undefined) {
  console.log('x is', test.x);
}
```

## Forwards compatibility

Forwards compatibility means reading newer data from an older program.
It can be achieved by bundling the schema along with the data instead of baking the schema into the program.
Forwards compatibility should always be possible as long as the newer schema is backwards compatible with the older schema.

The binary schema format is useful for this purpose.
To convert the textual schema format to the binary schema format, use the command `node node_modules/.bin/kiwic --schema test.kiwi --binary test.bkiwi`.
That binary buffer can then be passed to the `compileSchema()` function instead of the textual schema.
