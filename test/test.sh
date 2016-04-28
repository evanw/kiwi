#!/bin/sh

node ../node_modules/mocha/bin/mocha ./test.js

node ../cli.js --schema ./test-schema.kiwi --js ./test-schema.js

node ../cli.js --schema ./test-schema.kiwi --cpp ./test-schema.h && \
  c++ ./test.cpp -std=c++11 -I.. && \
  ./a.out && \
  rm ./a.out

node ../cli.js --schema ./test-schema.kiwi --skew ./test-schema.sk && \
  ../node_modules/.bin/skewc --output-file=temp.js ../kiwi.sk ../typedarray.sk ./test-schema.sk ./test.sk && \
  node ../node_modules/mocha/bin/mocha temp.js && \
  rm temp.js
