#!/bin/sh

node ../node_modules/mocha/bin/mocha ./test.js
node ../cli.js --schema ./test-schema.kiwi --js ./test-schema.js
node ../cli.js --schema ./test-schema.kiwi --cpp ./test-schema.h && c++ ./test.cpp -std=c++11 -I.. && ./a.out && rm ./a.out
