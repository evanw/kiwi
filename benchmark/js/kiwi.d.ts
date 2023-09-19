export type Type =
  "FLAT" |
  "ROUND" |
  "POINTED";

export interface Color {
  red: number;
  green: number;
  blue: number;
  alpha: number;
}

export interface Example {
  clientID?: number;
  type?: Type;
  colors?: Color[];
}

export interface Schema {
  Type: any;
  encodeColor(message: Color): Uint8Array;
  decodeColor(buffer: Uint8Array): Color;
  encodeExample(message: Example): Uint8Array;
  decodeExample(buffer: Uint8Array): Example;
}
