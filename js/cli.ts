#!/usr/bin/env node

import * as fs from 'fs';
import { compileSchema, compileSchemaJS } from './js';
import { compileSchemaTypeScript } from './ts';
import { Schema } from './schema';
import { compileSchemaCPP } from './cpp';
import { compileSchemaCallbackCPP } from './cpp-callback';
import { compileSchemaSkew } from './skew';
import { encodeBinarySchema, decodeBinarySchema } from './binary';
import { prettyPrintSchema } from './printer';
import { parseSchema } from './parser';
import { ByteBuffer } from './bb';
import { compileSchemaSkewTypes } from './skew-types';

let usage = [
  '',
  'Usage: kiwic [OPTIONS]',
  '',
  'Options:',
  '',
  '  --help                Print this message.',
  '  --schema [PATH]       The schema file to use.',
  '  --js [PATH]           Generate JavaScript code.',
  '  --ts [PATH]           Generate TypeScript type definitions.',
  '  --cpp [PATH]          Generate C++ code (tree style).',
  '  --callback-cpp [PATH] Generate C++ code (callback style).',
  '  --skew [PATH]         Generate Skew code.',
  '  --skew-types [PATH]   Generate Skew type definitions.',
  '  --text [PATH]         Encode the schema as text.',
  '  --binary [PATH]       Encode the schema as a binary blob.',
  '  --root-type [NAME]    Set the root type for JSON.',
  '  --to-json [PATH]      Convert a binary file to JSON.',
  '  --from-json [PATH]    Convert a JSON file to binary.',
  '',
  'Examples:',
  '',
  '  kiwic --schema test.kiwi --js test.js',
  '  kiwic --schema test.kiwi --cpp test.h',
  '  kiwic --schema test.kiwi --skew test.sk',
  '  kiwic --schema test.kiwi --binary test.bkiwi',
  '  kiwic --schema test.bkiwi --text test.kiwi',
  '  kiwic --schema test.kiwi --root-type Test --from-json buffer.json',
  '  kiwic --schema test.kiwi --root-type Test --to-json buffer.bin',
  '',
].join('\n');

function writeFileString(path: string, text: string): void {
  try {
    if (fs.readFileSync(path, 'utf8') === text) {
      return; // Avoid unnecessarily modifying files
    }
  } catch (e) {
  }
  fs.writeFileSync(path, text);
}

function writeFileBuffer(path: string, buffer: Buffer): void {
  try {
    if (fs.readFileSync(path).equals(buffer)) {
      return; // Avoid unnecessarily modifying files
    }
  } catch (e) {
  }
  fs.writeFileSync(path, buffer);
}

export function main(args: string[]): number {
  let flags: {[flag: string]: string | null} = {
    '--schema': null,
    '--js': null,
    '--ts': null,
    '--cpp': null,
    '--callback-cpp': null,
    '--skew': null,
    '--skew-types': null,
    '--binary': null,
    '--text': null,
    '--root-type': null,
    '--to-json': null,
    '--from-json': null,
  };

  // Parse flags
  for (let i = 0; i < args.length; i++) {
    let arg = args[i];

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
  let buffer = fs.readFileSync(flags['--schema']);
  let isText = Array.prototype.indexOf.call(buffer, 0) === -1; // Binary schemas will have null-terminated strings
  let content = isText ? buffer.toString() : new Uint8Array(buffer);

  // Try parsing the schema, pretty-print errors on failure
  try {
    var parsed: Schema = typeof content === 'string' ? parseSchema(content) : decodeBinarySchema(new ByteBuffer(content));
    var compiled = compileSchema(parsed);
  } catch (e) {
    if (e && e.message && 'line' in e || 'column' in e) {
      e.message = flags['--schema'] + ':' + e.line + ':' + e.column + ': error: ' + e.message;
      if (typeof content === 'string') {
        e.message += '\n' + content.split('\n')[e.line - 1] + '\n' + new Array(e.column).join(' ') + '^';
      }
    }
    throw e;
  }

  // Validate the root type
  let rootType = flags['--root-type'];
  if (rootType !== null && !(('encode' + rootType) in compiled && ('decode' + rootType) in compiled)) {
    throw new Error('Invalid root type: ' + JSON.stringify(rootType));
  }

  // Generate JavaScript code
  if (flags['--js'] !== null) {
    writeFileString(flags['--js'], compileSchemaJS(parsed));
  }

  // Generate JavaScript code
  if (flags['--ts'] !== null) {
    writeFileString(flags['--ts'], compileSchemaTypeScript(parsed));
  }

  // Generate C++ code
  if (flags['--cpp'] !== null) {
    writeFileString(flags['--cpp'], compileSchemaCPP(parsed));
  }
  if (flags['--callback-cpp'] !== null) {
    writeFileString(flags['--callback-cpp'], compileSchemaCallbackCPP(parsed));
  }

  // Generate Skew code
  if (flags['--skew'] !== null) {
    writeFileString(flags['--skew'], compileSchemaSkew(parsed));
  }
  if (flags['--skew-types'] !== null) {
    writeFileString(flags['--skew-types'], compileSchemaSkewTypes(parsed));
  }

  // Generate a binary schema file
  if (flags['--binary'] !== null) {
    writeFileBuffer(flags['--binary'], Buffer.from(encodeBinarySchema(parsed)));
  }

  // Generate a textual schema file
  if (flags['--text'] !== null) {
    writeFileBuffer(flags['--text'], Buffer.from(prettyPrintSchema(parsed)));
  }

  // Convert a binary file to JSON
  if (flags['--to-json'] !== null) {
    if (rootType === null) {
      throw new Error('Missing flag --root-type when using --to-json');
    }
    writeFileString(flags['--to-json'] + '.json', JSON.stringify((compiled as any)['decode' + rootType](
      new Uint8Array(fs.readFileSync(flags['--to-json']))), null, 2) + '\n');
  }

  // Convert a JSON file to binary
  if (flags['--from-json'] !== null) {
    if (rootType === null) {
      throw new Error('Missing flag --root-type when using --from-json');
    }
    writeFileBuffer(flags['--from-json'] + '.bin', Buffer.from((compiled as any)['encode' + rootType](
      JSON.parse(fs.readFileSync(flags['--from-json'], 'utf8')))));
  }

  return 0;
};

if (require.main === module) {
  // try {
    process.exit(main(process.argv.slice(2)));
  // } catch (e) {
  //   process.stderr.write((e && e.message || e) + '\n');
  //   process.exit(1);
  // }
}
