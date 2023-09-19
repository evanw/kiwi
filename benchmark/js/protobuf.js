const protobuf = require("protobufjs");
const fs = require("fs");
const assert = require("assert");
const { Bench } = require("tinybench");

const bench = new Bench({
  time: process.env.BENCH_TIME || 1000,
});

const schemaString = fs.readFileSync('test.proto', 'utf8');

const root = new protobuf.Root();
const parseSchema = protobuf.parse(schemaString, root);

const TestExample = root.lookupType("test.Example");
const enumType = root.lookupEnum("test.Type");
const testObject = {
  clientID: 100,
  type: enumType.values.POINTED,
  colors: [
    {
      rgba: Buffer.from([255, 127, 0, 255])
    },
  ],
};
const message = TestExample.create(testObject);
const encoded = TestExample.encode(message).finish();
const decoded = TestExample.decode(encoded);
const object = TestExample.toObject(decoded);
assert.deepStrictEqual(object, testObject);

bench.add("parse + compile", () => {
  const root = new protobuf.Root();
  protobuf.parse(schemaString, root);
  root.lookupType("test.Example");
});

bench.add("to binary", () => {
  const message = TestExample.create(testObject);
  TestExample.encode(message).finish();
});

bench.add("from binary", () => {
  const message = TestExample.decode(encoded);
  TestExample.toObject(message);
});

console.log('protobuf size:', Buffer.byteLength(encoded));

bench.run().then(() => {
  console.table(bench.table());
});
