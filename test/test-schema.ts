export namespace test {
  export type Enum =
    "A" |
    "B";

  export interface EnumStruct {
    x: Enum;
    y: Enum[];
  }

  export interface BoolStruct {
    x: boolean;
  }

  export interface ByteStruct {
    x: number;
  }

  export interface IntStruct {
    x: number;
  }

  export interface UintStruct {
    x: number;
  }

  export interface FloatStruct {
    x: number;
  }

  export interface StringStruct {
    x: string;
  }

  export interface CompoundStruct {
    x: number;
    y: number;
  }

  export interface NestedStruct {
    a: number;
    b: CompoundStruct;
    c: number;
  }

  export interface BoolMessage {
    x?: boolean;
  }

  export interface ByteMessage {
    x?: number;
  }

  export interface IntMessage {
    x?: number;
  }

  export interface UintMessage {
    x?: number;
  }

  export interface FloatMessage {
    x?: number;
  }

  export interface StringMessage {
    x?: string;
  }

  export interface CompoundMessage {
    x?: number;
    y?: number;
  }

  export interface NestedMessage {
    a?: number;
    b?: CompoundMessage;
    c?: number;
  }

  export interface BoolArrayStruct {
    x: boolean[];
  }

  export interface ByteArrayStruct {
    x: Uint8Array;
  }

  export interface IntArrayStruct {
    x: number[];
  }

  export interface UintArrayStruct {
    x: number[];
  }

  export interface FloatArrayStruct {
    x: number[];
  }

  export interface StringArrayStruct {
    x: string[];
  }

  export interface CompoundArrayStruct {
    x: number[];
    y: number[];
  }

  export interface BoolArrayMessage {
    x?: boolean[];
  }

  export interface ByteArrayMessage {
    x?: Uint8Array;
  }

  export interface IntArrayMessage {
    x?: number[];
  }

  export interface UintArrayMessage {
    x?: number[];
  }

  export interface FloatArrayMessage {
    x?: number[];
  }

  export interface StringArrayMessage {
    x?: string[];
  }

  export interface CompoundArrayMessage {
    x?: number[];
    y?: number[];
  }

  export interface RecursiveMessage {
    x?: RecursiveMessage;
  }

  export interface NonDeprecatedMessage {
    a?: number;
    b?: number;
    c?: number[];
    d?: number[];
    e?: ByteStruct;
    f?: ByteStruct;
    g?: number;
  }

  export interface DeprecatedMessage {
    a?: number;
    c?: number[];
    e?: ByteStruct;
    g?: number;
  }

  export interface SortedStruct {
    a1: boolean;
    b1: number;
    c1: number;
    d1: number;
    e1: number;
    f1: string;
    a2: boolean;
    b2: number;
    c2: number;
    d2: number;
    e2: number;
    f2: string;
    a3: boolean[];
    b3: Uint8Array;
    c3: number[];
    d3: number[];
    e3: number[];
    f3: string[];
  }

  export interface Schema {
    Enum: any;
    encodeEnumStruct(message: EnumStruct): Uint8Array;
    decodeEnumStruct(buffer: Uint8Array): EnumStruct;
    encodeBoolStruct(message: BoolStruct): Uint8Array;
    decodeBoolStruct(buffer: Uint8Array): BoolStruct;
    encodeByteStruct(message: ByteStruct): Uint8Array;
    decodeByteStruct(buffer: Uint8Array): ByteStruct;
    encodeIntStruct(message: IntStruct): Uint8Array;
    decodeIntStruct(buffer: Uint8Array): IntStruct;
    encodeUintStruct(message: UintStruct): Uint8Array;
    decodeUintStruct(buffer: Uint8Array): UintStruct;
    encodeFloatStruct(message: FloatStruct): Uint8Array;
    decodeFloatStruct(buffer: Uint8Array): FloatStruct;
    encodeStringStruct(message: StringStruct): Uint8Array;
    decodeStringStruct(buffer: Uint8Array): StringStruct;
    encodeCompoundStruct(message: CompoundStruct): Uint8Array;
    decodeCompoundStruct(buffer: Uint8Array): CompoundStruct;
    encodeNestedStruct(message: NestedStruct): Uint8Array;
    decodeNestedStruct(buffer: Uint8Array): NestedStruct;
    encodeBoolMessage(message: BoolMessage): Uint8Array;
    decodeBoolMessage(buffer: Uint8Array): BoolMessage;
    encodeByteMessage(message: ByteMessage): Uint8Array;
    decodeByteMessage(buffer: Uint8Array): ByteMessage;
    encodeIntMessage(message: IntMessage): Uint8Array;
    decodeIntMessage(buffer: Uint8Array): IntMessage;
    encodeUintMessage(message: UintMessage): Uint8Array;
    decodeUintMessage(buffer: Uint8Array): UintMessage;
    encodeFloatMessage(message: FloatMessage): Uint8Array;
    decodeFloatMessage(buffer: Uint8Array): FloatMessage;
    encodeStringMessage(message: StringMessage): Uint8Array;
    decodeStringMessage(buffer: Uint8Array): StringMessage;
    encodeCompoundMessage(message: CompoundMessage): Uint8Array;
    decodeCompoundMessage(buffer: Uint8Array): CompoundMessage;
    encodeNestedMessage(message: NestedMessage): Uint8Array;
    decodeNestedMessage(buffer: Uint8Array): NestedMessage;
    encodeBoolArrayStruct(message: BoolArrayStruct): Uint8Array;
    decodeBoolArrayStruct(buffer: Uint8Array): BoolArrayStruct;
    encodeByteArrayStruct(message: ByteArrayStruct): Uint8Array;
    decodeByteArrayStruct(buffer: Uint8Array): ByteArrayStruct;
    encodeIntArrayStruct(message: IntArrayStruct): Uint8Array;
    decodeIntArrayStruct(buffer: Uint8Array): IntArrayStruct;
    encodeUintArrayStruct(message: UintArrayStruct): Uint8Array;
    decodeUintArrayStruct(buffer: Uint8Array): UintArrayStruct;
    encodeFloatArrayStruct(message: FloatArrayStruct): Uint8Array;
    decodeFloatArrayStruct(buffer: Uint8Array): FloatArrayStruct;
    encodeStringArrayStruct(message: StringArrayStruct): Uint8Array;
    decodeStringArrayStruct(buffer: Uint8Array): StringArrayStruct;
    encodeCompoundArrayStruct(message: CompoundArrayStruct): Uint8Array;
    decodeCompoundArrayStruct(buffer: Uint8Array): CompoundArrayStruct;
    encodeBoolArrayMessage(message: BoolArrayMessage): Uint8Array;
    decodeBoolArrayMessage(buffer: Uint8Array): BoolArrayMessage;
    encodeByteArrayMessage(message: ByteArrayMessage): Uint8Array;
    decodeByteArrayMessage(buffer: Uint8Array): ByteArrayMessage;
    encodeIntArrayMessage(message: IntArrayMessage): Uint8Array;
    decodeIntArrayMessage(buffer: Uint8Array): IntArrayMessage;
    encodeUintArrayMessage(message: UintArrayMessage): Uint8Array;
    decodeUintArrayMessage(buffer: Uint8Array): UintArrayMessage;
    encodeFloatArrayMessage(message: FloatArrayMessage): Uint8Array;
    decodeFloatArrayMessage(buffer: Uint8Array): FloatArrayMessage;
    encodeStringArrayMessage(message: StringArrayMessage): Uint8Array;
    decodeStringArrayMessage(buffer: Uint8Array): StringArrayMessage;
    encodeCompoundArrayMessage(message: CompoundArrayMessage): Uint8Array;
    decodeCompoundArrayMessage(buffer: Uint8Array): CompoundArrayMessage;
    encodeRecursiveMessage(message: RecursiveMessage): Uint8Array;
    decodeRecursiveMessage(buffer: Uint8Array): RecursiveMessage;
    encodeNonDeprecatedMessage(message: NonDeprecatedMessage): Uint8Array;
    decodeNonDeprecatedMessage(buffer: Uint8Array): NonDeprecatedMessage;
    encodeDeprecatedMessage(message: DeprecatedMessage): Uint8Array;
    decodeDeprecatedMessage(buffer: Uint8Array): DeprecatedMessage;
    encodeSortedStruct(message: SortedStruct): Uint8Array;
    decodeSortedStruct(buffer: Uint8Array): SortedStruct;
  }
}
