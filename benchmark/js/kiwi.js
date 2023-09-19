const { Bench } = require('tinybench')
const kiwi = require('kiwi-schema');
const assert = require('assert');
const fs = require('fs');

const bench = new Bench({
  time: process.env.BENCH_TIME || 1000
})

const schemaString = fs.readFileSync('test.kiwi', 'utf8');

const parseSchema = kiwi.parseSchema(schemaString)
/** @type {import('./kiwi').Schema} */
const schema = kiwi.compileSchema(parseSchema);
const testObject = {
  "clientID": 100,
  "type": "POINTED",
  "colors": [
    {
      "rgba": new Uint8Array([255, 127, 0, 255])
    }
  ]
};
const encoded = schema.encodeExample(testObject);
const decoded = schema.decodeExample(encoded);
assert.deepStrictEqual(decoded, testObject);

bench.add('parse + compile', () => {
  const parseSchema = kiwi.parseSchema(schemaString);
  kiwi.compileSchema(parseSchema);
});

bench.add('to binary', () => {
  schema.encodeExample(testObject);
})

bench.add('from binary', () => {
  schema.decodeExample(encoded);
})

console.log('kiwi size:', Buffer.byteLength(encoded));

bench.run().then(() => {
  console.table(bench.table());
});
