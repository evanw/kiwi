#!/usr/bin/env node

var kiwi = require('./kiwi');
var fs = require('fs');

var usage = [
  '',
  'Usage: kiwic [OPTIONS]',
  '',
  'Options:',
  '',
  '  --help              Print this message.',
  '  --schema [PATH]     The schema file to use.',
  '  --js [PATH]         Generate JavaScript code.',
  '  --cpp [PATH]        Generate C++ code.',
  '  --root-type [NAME]  Set the root type for JSON.',
  '  --to-json [PATH]    Convert a binary file to JSON.',
  '  --from-json [PATH]  Convert a JSON file to binary.',
  '',
].join('\n');

var main = exports.main = function(args) {
  var flags = {
    '--schema': null,
    '--js': null,
    '--cpp': null,
    '--root-type': null,
    '--to-json': null,
    '--from-json': null,
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

  // Validate the root type
  var rootType = flags['--root-type'];
  if (rootType !== null && !(('encode' + rootType) in schema && ('decode' + rootType) in schema)) {
    throw new Error('Invalid root type: ' + JSON.stringify(rootType));
  }

  // Generate JavaScript code
  if (flags['--js'] !== null) {
    fs.writeFileSync(flags['--js'], kiwi.compileSchemaJS(content));
  }

  // Generate C++ code
  if (flags['--cpp'] !== null) {
    fs.writeFileSync(flags['--cpp'], kiwi.compileSchemaCPP(content));
  }

  // Convert a binary file to JSON
  if (flags['--to-json'] !== null) {
    if (rootType === null) {
      throw new Error('Missing flag --root-type when using --to-json');
    }
    fs.writeFileSync(flags['--to-json'] + '.json', JSON.stringify(schema['decode' + rootType](new Uint8Array(fs.readFileSync(flags['--to-json']))), null, 2) + '\n');
  }

  // Convert a JSON file to binary
  if (flags['--from-json'] !== null) {
    if (rootType === null) {
      throw new Error('Missing flag --root-type when using --from-json');
    }
    fs.writeFileSync(flags['--from-json'] + '.bin', Buffer(schema['encode' + rootType](JSON.parse(fs.readFileSync(flags['--from-json'], 'utf8')))));
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
