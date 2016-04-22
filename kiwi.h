#ifndef KIWI_H
#define KIWI_H

#include <assert.h>
#include <memory.h>
#include <stdlib.h>

namespace kiwi {
  class String;
  class MemoryPool;

  class ByteBuffer {
  public:
    ByteBuffer();
    ByteBuffer(uint8_t *data, size_t size);
    ~ByteBuffer();
    ByteBuffer(const ByteBuffer &) = delete;
    ByteBuffer &operator = (const ByteBuffer &) = delete;

    uint8_t *data() const { return _data; }
    size_t size() const { return _size; }
    size_t index() const { return _index; }

    bool seekTo(size_t index);
    bool readByte(bool &result);
    bool readByte(uint8_t &result);
    bool readFloat(float &result);
    bool readVarUint(uint32_t &result);
    bool readVarInt(int32_t &result);
    bool readString(String &result, MemoryPool &pool);

    void writeByte(uint8_t value);
    void writeFloat(float value);
    void writeVarUint(uint32_t value);
    void writeVarInt(int32_t value);
    void writeString(const char *value);

  private:
    void _growBy(size_t amount);

    enum { INITIAL_CAPACITY = 256 };
    uint8_t *_data = nullptr;
    size_t _size = 0;
    size_t _capacity = 0;
    size_t _index = 0;
    bool _ownsData = false;
  };

  ////////////////////////////////////////////////////////////////////////////////

  struct String {
    String() {}
    String(const char *c_str) : _c_str(c_str) {}

    const char *c_str() const { return _c_str; }

  private:
    const char *_c_str = nullptr;
  };

  inline bool operator == (const String &a, const String &b) { return !strcmp(a.c_str(), b.c_str()); }
  inline bool operator != (const String &a, const String &b) { return !(a == b); }

  ////////////////////////////////////////////////////////////////////////////////

  template <typename T>
  struct Array {
    Array() {}
    Array(T *data, uint32_t size) : _data(data), _size(size) {}

    T *data() { return _data; }
    T *begin() { return _data; }
    T *end() { return _data + _size; }
    uint32_t size() const { return _size; }
    T &operator [] (uint32_t index) { assert(index < _size); return _data[index]; }

  private:
    T *_data = nullptr;
    uint32_t _size = 0;
  };

  ////////////////////////////////////////////////////////////////////////////////

  class MemoryPool {
  public:
    ~MemoryPool();

    template <typename T>
    T *allocate(uint32_t count);

    template <typename T>
    Array<T> *array(uint32_t size) { return new (allocate<Array<T>>(1)) Array<T>(allocate<T>(size), size); }

    String string(const char *data, uint32_t count);
    String string(const char *c_str) { return string(c_str, strlen(c_str)); }

  private:
    enum { INITIAL_CAPACITY = 1 << 12 };

    struct Chunk {
      uint8_t *data = nullptr;
      uint32_t capacity = 0;
      uint32_t used = 0;
      Chunk *next = nullptr;
    };

    Chunk *_first = nullptr;
    Chunk *_last = nullptr;
  };

  ////////////////////////////////////////////////////////////////////////////////

  ByteBuffer::ByteBuffer() : _data(new uint8_t[INITIAL_CAPACITY]), _capacity(INITIAL_CAPACITY), _ownsData(true) {
  }

  ByteBuffer::ByteBuffer(uint8_t *data, size_t size) : _data(data), _size(size), _capacity(size) {
  }

  ByteBuffer::~ByteBuffer() {
    if (_ownsData) {
      delete [] _data;
    }
  }

  bool ByteBuffer::seekTo(size_t index) {
    if (index <= _size) {
      _index = index;
      return true;
    }

    return false;
  }

  bool ByteBuffer::readByte(bool &result) {
    uint8_t value;
    if (!readByte(value)) {
      result = false;
      return false;
    }

    result = value;
    return true;
  }

  bool ByteBuffer::readByte(uint8_t &result) {
    if (_index + sizeof(uint8_t) > _size) {
      result = 0;
      return false;
    }

    result = _data[_index];
    _index += sizeof(uint8_t);
    return true;
  }

  bool ByteBuffer::readFloat(float &result) {
    if (_index + sizeof(float) > _size) {
      result = 0;
      return false;
    }

    memcpy(&result, _data + _index, sizeof(float));
    _index += sizeof(float);
    return true;
  }

  bool ByteBuffer::readVarUint(uint32_t &result) {
    uint8_t shift = 0;
    uint8_t byte;
    result = 0;

    do {
      if (!readByte(byte)) {
        return false;
      }

      result |= (byte & 127) << shift;
      shift += 7;
    } while (byte & 128 && shift < 35);

    return true;
  }

  bool ByteBuffer::readVarInt(int32_t &result) {
    uint32_t value;
    if (!readVarUint(value)) {
      result = 0;
      return false;
    }

    result = value & 1 ? ~(value >> 1) : value >> 1;
    return true;
  }

  bool ByteBuffer::readString(String &result, MemoryPool &pool) {
    uint32_t size = 0;
    result = String();

    do {
      if (_index >= _size) return false;
    } while (_data[_index + size++] != '\0');

    result = pool.string(reinterpret_cast<char *>(_data + _index), size - 1);
    _index += size;
    return true;
  }

  void ByteBuffer::writeByte(uint8_t value) {
    size_t index = _size;
    _growBy(sizeof(uint8_t));
    _data[index] = value;
  }

  void ByteBuffer::writeFloat(float value) {
    size_t index = _size;
    _growBy(sizeof(float));
    memcpy(_data + index, &value, sizeof(float));
  }

  void ByteBuffer::writeVarUint(uint32_t value) {
    do {
      uint8_t byte = value & 127;
      value >>= 7;
      writeByte(value ? byte | 128 : byte);
    } while (value);
  }

  void ByteBuffer::writeVarInt(int32_t value) {
    writeVarUint((value << 1) ^ (value >> 31));
  }

  void ByteBuffer::writeString(const char *value) {
    uint32_t count = strlen(value) + 1;
    size_t index = _size;
    _growBy(count);
    memcpy(_data + index, value, count);
  }

  void ByteBuffer::_growBy(size_t amount) {
    if (_size + amount > _capacity) {
      size_t capacity = (_size + amount) * 2;
      uint8_t *data = new uint8_t[capacity];
      memcpy(data, _data, _size);

      if (_ownsData) {
        delete [] _data;
      }

      _data = data;
      _capacity = capacity;
      _ownsData = true;
    }

    _size += amount;
  }

  ////////////////////////////////////////////////////////////////////////////////

  inline uint32_t nextMultipleOf(uint32_t value, uint32_t stride) {
    value += stride - 1;
    return value - value % stride;
  }

  MemoryPool::~MemoryPool() {
    for (Chunk *chunk = _first, *next; chunk; chunk = next) {
      next = chunk->next;
      delete [] chunk->data;
      delete chunk;
    }
  }

  template <typename T>
  T *MemoryPool::allocate(uint32_t count) {
    Chunk *chunk = _last;
    uint32_t size = count * sizeof(T);
    uint32_t index = chunk ? nextMultipleOf(chunk->used, alignof(T)) : 0;

    if (chunk && index + size >= index && index + size <= chunk->capacity) {
      chunk->used = index + size;
    }

    else {
      chunk = new Chunk;
      chunk->capacity = size > INITIAL_CAPACITY ? size : INITIAL_CAPACITY;
      chunk->data = new uint8_t[chunk->capacity](); // "()" means zero-initialized
      chunk->used = size;

      if (_last) _last->next = chunk;
      else _first = chunk;
      _last = chunk;
    }

    return reinterpret_cast<T *>(chunk->data + index);
  }

  String MemoryPool::string(const char *text, uint32_t count) {
    char *c_str = allocate<char>(count + 1);
    memcpy(c_str, text, count);
    return c_str;
  }
}

#endif
