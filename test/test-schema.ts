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
    x: number[];
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
    x?: number[];
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
}
