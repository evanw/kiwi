#!/usr/bin/env node

var kiwi = require('./kiwi');
var fs = require('fs');

var usage = [
  '',
  'Usage: kiwic [OPTIONS]',
  '',
  'Options:',
  '',
  '  --help           Print this message.',
  '  --schema [PATH]  The schema file to use.',
  '  --js [PATH]      Generate JavaScript code.',
  '',
].join('\n');

var main = exports.main = function(args) {
  var flags = {
    '--schema': null,
    '--js': null,
  };

  // Parse flags
  for (var i = 0; i < args.length; i++) {
    var arg = args[i];

    if (arg === '-h' || arg === '--help' || arg[0] !== '-') {
      console.log(usage);
      return 1;
    }

    else if (arg in flags) {
      if (i + 1 === args.length) {
        throw new Error('Missing value for ' + JSON.stringify(arg) + ' (use "--help" for usage)');
      }
      flags[arg] = args[++i];
    }

    else {
      throw new Error('Unknown flag ' + JSON.stringify(arg) + ' (use "--help" for usage)');
    }
  }

  // Must have a schema
  if (flags['--schema'] === null) {
    console.log(usage);
    return 1;
  }

  // Try loading the schema
  var content = fs.readFileSync(flags['--schema'], 'utf8');

  // Try parsing the schema, pretty-print errors on failure
  try {
    var schema = kiwi.compileSchema(content);
  } catch (e) {
    if (e && e.message && 'line' in e || 'column' in e) {
      e.message =
        flags['--schema'] + ':' + e.line + ':' + e.column + ': error: ' + e.message + '\n' +
        content.split('\n')[e.line - 1] + '\n' + new Array(e.column).join(' ') + '^';
    }
    throw e;
  }

  // Generate JavaScript code
  if (flags['--js'] !== null) {
    var name = schema.package;
    var js = [];

    if (name !== null) {
      js.push('var ' + name + ' = exports || ' + name + ' || {}, exports;');
    } else {
      js.push('var exports = exports || {};');
      name = 'exports';
    }

    js.push(name + '.ByteBuffer = ' + name + '.ByteBuffer || require("kiwi-schema").ByteBuffer;');

    for (var key in schema) {
      var value = schema[key];

      if (kiwi.reservedNames.indexOf(key) !== -1) {
        continue;
      }

      else if (value instanceof Function) {
        var match = /function[^(]*\(([\w\s,]+).*\)\s*\{([^]*\})/.exec(value + '');
        js.push(name + '[' + JSON.stringify(key) + '] = function(' + match[1].trim() + ') {' + match[2].replace(/\n/g, '\n  ').replace(/\n  \}$/, '\n}') + ';');
      }

      else {
        js.push(name + '[' + JSON.stringify(key) + '] = ' + JSON.stringify(value, null, 2) + ';');
      }
    }

    js.push('');
    fs.writeFileSync(flags['--js'], js.join('\n'));
  }

  return 0;
};

if (require.main === module) {
  try {
    process.exit(main(process.argv.slice(2)));
  } catch (e) {
    process.stderr.write((e && e.message || e) + '\n');
    process.exit(1);
  }
}
