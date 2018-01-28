#!/bin/sh

set -e

node ../js/node_modules/mocha/bin/mocha ./test.js

node ../js/cli.js --schema ./test-schema.kiwi --js ./test-schema.js

node ../js/cli.js --schema ./test-schema.kiwi --ts ./test-schema.ts

node ../js/cli.js --schema ./test-schema.kiwi --cpp ./test-schema.h
node ../js/cli.js --schema ./test1-schema.kiwi --cpp ./test1-schema.h --binary ./test1-schema.bkiwi
node ../js/cli.js --schema ./test2-schema.kiwi --cpp ./test2-schema.h --binary ./test2-schema.bkiwi
node ../js/cli.js --schema ./test-schema-large.kiwi --cpp ./test-schema-large.h
c++ ./test.cpp -std=c++11 -I..
./a.out
rm ./a.out

node ../js/cli.js --schema ./test-schema.kiwi --callback-cpp ./test-schema-callback.h
c++ ./test-callback.cpp -std=c++11 -I..
./a.out
rm ./a.out

node ../js/cli.js --schema ./test-schema.kiwi --skew ./test-schema.sk
node ../js/cli.js --schema ./test1-schema.kiwi --skew ./test1-schema.sk
node ../js/cli.js --schema ./test2-schema.kiwi --skew ./test2-schema.sk
node ../js/cli.js --schema ./test-schema-large.kiwi --skew ./test-schema-large.sk
../js/node_modules/.bin/skewc --output-file=temp.js ../kiwi.sk ../typedarray.sk ./test-schema.sk ./test1-schema.sk ./test2-schema.sk ./test-schema-large.sk ./test.sk
node ../js/node_modules/mocha/bin/mocha temp.js
rm temp.js
