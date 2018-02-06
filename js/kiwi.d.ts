export class ByteBuffer {
  constructor(data?: Uint8Array);
  toUint8Array(): Uint8Array;

  readByte(): number;
  readVarFloat(): number;
  readVarUint(): number;
  readVarInt(): number;
  readString(): string;

  writeByte(value: number): void;
  writeVarFloat(value: number): void;
  writeVarUint(value: number): void;
  writeVarInt(value: number): void;
  writeString(value: string): void;
}

export const nativeTypes: string[];
export const reservedNames: string[];

export interface Schema {
  package: string | null,
  definitions: Definition[];
}

export interface Definition {
  name: string,
  line: number,
  column: number,
  kind: 'ENUM' | 'STRUCT' | 'MESSAGE',
  fields: Field[],
}

export interface Field {
  name: string,
  line: number,
  column: number,
  type: string | null,
  isArray: boolean,
  isDeprecated: boolean,
  value: number,
}

export function parseSchema(text: string): Schema;
export function decodeBinarySchema(bb: Uint8Array | ByteBuffer): Schema;
export function encodeBinarySchema(schema: string | Uint8Array | ByteBuffer): Uint8Array;
export function compileSchema(schema: string | Uint8Array | ByteBuffer): any;
export function compileSchemaJS(schema: string | Uint8Array | ByteBuffer): string;
export function compileSchemaCPP(schema: string | Uint8Array | ByteBuffer): string;
export function compileSchemaCallbackCPP(schema: string | Uint8Array | ByteBuffer): string;
export function compileSchemaSkew(schema: string | Uint8Array | ByteBuffer): string;
export function compileSchemaTypeScript(schema: string | Uint8Array | ByteBuffer): string;
