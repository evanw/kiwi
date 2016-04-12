#ifndef KIWI_H
#define KIWI_H

#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <string>

namespace kiwi {
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
    bool readByte(uint8_t &result);
    bool readFloat(float &result);
    bool readVarUint(uint32_t &result);
    bool readVarInt(int32_t &result);
    bool readString(std::string &result);

    void writeByte(uint8_t value);
    void writeFloat(float value);
    void writeVarUint(uint32_t value);
    void writeVarInt(int32_t value);
    void writeString(const std::string &value);

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

  class Codec {
  public:
    Codec();
    Codec(ByteBuffer &bb);
    Codec(Codec &&encoder);
    Codec(const Codec &) = delete;
    Codec &operator = (const Codec &) = delete;

  protected:
    void _structReadField(uint32_t field);
    void _structReadFieldNested(uint32_t field, Codec &codec);
    bool _structReadByte(uint32_t field, uint8_t &value);
    bool _structReadVarInt(uint32_t field, int32_t &value);
    bool _structReadVarUint(uint32_t field, uint32_t &value);
    bool _structReadFloat(uint32_t field, float &value);
    bool _structReadString(uint32_t field, std::string &value);

    bool _messageReadField();
    bool _messageReadByte(uint32_t field, uint8_t &value);
    bool _messageReadVarInt(uint32_t field, int32_t &value);
    bool _messageReadVarUint(uint32_t field, uint32_t &value);
    bool _messageReadFloat(uint32_t field, float &value);
    bool _messageReadString(uint32_t field, std::string &value);

    void _structWriteField(uint32_t field);
    void _structWriteByte(uint32_t field, uint8_t value);
    void _structWriteVarInt(uint32_t field, int32_t value);
    void _structWriteVarUint(uint32_t field, uint32_t value);
    void _structWriteFloat(uint32_t field, float value);
    void _structWriteString(uint32_t field, const std::string &value);

    void _structWriteArrayField(uint32_t field);
    void _structWriteArrayBegin(uint32_t field, uint32_t count);
    void _structWriteArrayByte(uint32_t field, uint8_t value);
    void _structWriteArrayVarInt(uint32_t field, int32_t value);
    void _structWriteArrayVarUint(uint32_t field, uint32_t value);
    void _structWriteArrayFloat(uint32_t field, float value);
    void _structWriteArrayString(uint32_t field, const std::string &value);
    void _structWriteArrayEnd(uint32_t field);

    void _messageWriteField(uint32_t field);
    void _messageWriteByte(uint32_t field, uint8_t value);
    void _messageWriteVarInt(uint32_t field, int32_t value);
    void _messageWriteVarUint(uint32_t field, uint32_t value);
    void _messageWriteFloat(uint32_t field, float value);
    void _messageWriteString(uint32_t field, const std::string &value);

    void _messageWriteArrayField();
    void _messageWriteArrayBegin(uint32_t field, uint32_t count);
    void _messageWriteArrayByte(uint8_t value);
    void _messageWriteArrayVarInt(int32_t value);
    void _messageWriteArrayVarUint(uint32_t value);
    void _messageWriteArrayFloat(float value);
    void _messageWriteArrayString(const std::string &value);
    void _messageWriteArrayEnd();

    void _messageFinish();

    ByteBuffer *_bb = nullptr;
    uint32_t _nextField = 0;
    uint32_t _countRemaining = 0;
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

  bool ByteBuffer::readString(std::string &result) {
    uint32_t size;
    if (!readVarUint(size) || _index + size < _index || _index + size > _size) {
      result.clear();
      return false;
    }

    result.resize(size);
    memcpy(&result[0], _data + _index, size);
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

  void ByteBuffer::writeString(const std::string &value) {
    writeVarUint(value.size());
    size_t index = _size;
    _growBy(value.size());
    memcpy(_data + index, value.data(), value.size());
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

  Codec::Codec() {
  }

  Codec::Codec(ByteBuffer &bb) : _bb(&bb) {
  }

  Codec::Codec(Codec &&encoder) : _bb(encoder._bb), _nextField(encoder._nextField), _countRemaining(encoder._countRemaining) {
    encoder._bb = nullptr;
  }

  ////////////////////////////////////////////////////////////////////////////////

  void Codec::_structReadField(uint32_t field) {
    assert(_bb && _nextField++ == field && !_countRemaining); // Must set each field once in order
  }

  void Codec::_structReadFieldNested(uint32_t field, Codec &codec) {
    _structReadField(field);
    assert(_bb && !codec._bb);
    codec._bb = _bb;
  }

  bool Codec::_structReadByte(uint32_t field, uint8_t &value) {
    if (!_bb) return false;
    _structReadField(field);
    if (_bb->readByte(value)) return true;
    _bb = nullptr;
    return false;
  }

  bool Codec::_structReadVarInt(uint32_t field, int32_t &value) {
    if (!_bb) return false;
    _structReadField(field);
    if (_bb->readVarInt(value)) return true;
    _bb = nullptr;
    return false;
  }

  bool Codec::_structReadVarUint(uint32_t field, uint32_t &value) {
    if (!_bb) return false;
    _structReadField(field);
    if (_bb->readVarUint(value)) return true;
    _bb = nullptr;
    return false;
  }

  bool Codec::_structReadFloat(uint32_t field, float &value) {
    if (!_bb) return false;
    _structReadField(field);
    if (_bb->readFloat(value)) return true;
    _bb = nullptr;
    return false;
  }

  bool Codec::_structReadString(uint32_t field, std::string &value) {
    if (!_bb) return false;
    _structReadField(field);
    if (_bb->readString(value)) return true;
    _bb = nullptr;
    return false;
  }

  ////////////////////////////////////////////////////////////////////////////////

  bool Codec::_messageReadField() {
    return _bb && _bb->readVarUint(_nextField);
  }

  bool Codec::_messageReadByte(uint32_t field, uint8_t &value) {
    if (!_bb) return false;
    assert(_nextField == field); // This must be checked by the caller first
    if (_bb->readByte(value)) return true;
    _bb = nullptr;
    return false;
  }

  bool Codec::_messageReadVarInt(uint32_t field, int32_t &value) {
    if (!_bb) return false;
    assert(_nextField == field); // This must be checked by the caller first
    if (_bb->readVarInt(value)) return true;
    _bb = nullptr;
    return false;
  }

  bool Codec::_messageReadVarUint(uint32_t field, uint32_t &value) {
    if (!_bb) return false;
    assert(_nextField == field); // This must be checked by the caller first
    if (_bb->readVarUint(value)) return true;
    _bb = nullptr;
    return false;
  }

  bool Codec::_messageReadFloat(uint32_t field, float &value) {
    if (!_bb) return false;
    assert(_nextField == field); // This must be checked by the caller first
    if (_bb->readFloat(value)) return true;
    _bb = nullptr;
    return false;
  }

  bool Codec::_messageReadString(uint32_t field, std::string &value) {
    if (!_bb) return false;
    assert(_nextField == field); // This must be checked by the caller first
    if (_bb->readString(value)) return true;
    _bb = nullptr;
    return false;
  }

  ////////////////////////////////////////////////////////////////////////////////

  void Codec::_structWriteField(uint32_t field) {
    assert(_bb && _nextField++ == field && !_countRemaining); // Must set each field once in order
  }

  void Codec::_structWriteByte(uint32_t field, uint8_t value) {
    _structWriteField(field);
    _bb->writeByte(value);
  }

  void Codec::_structWriteVarInt(uint32_t field, int32_t value) {
    _structWriteField(field);
    _bb->writeVarInt(value);
  }

  void Codec::_structWriteVarUint(uint32_t field, uint32_t value) {
    _structWriteField(field);
    _bb->writeVarUint(value);
  }

  void Codec::_structWriteFloat(uint32_t field, float value) {
    _structWriteField(field);
    _bb->writeFloat(value);
  }

  void Codec::_structWriteString(uint32_t field, const std::string &value) {
    _structWriteField(field);
    _bb->writeString(value);
  }

  ////////////////////////////////////////////////////////////////////////////////

  void Codec::_structWriteArrayField(uint32_t field) {
    assert(_bb && _nextField == field);
  }

  void Codec::_structWriteArrayBegin(uint32_t field, uint32_t count) {
    assert(!_countRemaining); // Must not be inside an array
    _structWriteArrayField(field);
    _bb->writeVarUint(count);
    _countRemaining = count;
  }

  void Codec::_structWriteArrayByte(uint32_t field, uint8_t value) {
    assert(_countRemaining-- > 0);
    _structWriteArrayField(field);
    _bb->writeByte(value);
  }

  void Codec::_structWriteArrayVarInt(uint32_t field, int32_t value) {
    assert(_countRemaining-- > 0);
    _structWriteArrayField(field);
    _bb->writeVarInt(value);
  }

  void Codec::_structWriteArrayVarUint(uint32_t field, uint32_t value) {
    assert(_countRemaining-- > 0);
    _structWriteArrayField(field);
    _bb->writeVarUint(value);
  }

  void Codec::_structWriteArrayFloat(uint32_t field, float value) {
    assert(_countRemaining-- > 0);
    _structWriteArrayField(field);
    _bb->writeFloat(value);
  }

  void Codec::_structWriteArrayString(uint32_t field, const std::string &value) {
    assert(_countRemaining-- > 0);
    _structWriteArrayField(field);
    _bb->writeString(value);
  }

  void Codec::_structWriteArrayEnd(uint32_t field) {
    assert(!_countRemaining); // Must write all elements
    _structWriteArrayField(field);
    _nextField++;
  }

  ////////////////////////////////////////////////////////////////////////////////

  void Codec::_messageWriteField(uint32_t field) {
    assert(_bb && !_countRemaining); // Must not be inside an array
    _bb->writeVarUint(field);
  }

  void Codec::_messageWriteByte(uint32_t field, uint8_t value) {
    _messageWriteField(field);
    _bb->writeByte(value);
  }

  void Codec::_messageWriteVarInt(uint32_t field, int32_t value) {
    _messageWriteField(field);
    _bb->writeVarInt(value);
  }

  void Codec::_messageWriteVarUint(uint32_t field, uint32_t value) {
    _messageWriteField(field);
    _bb->writeVarUint(value);
  }

  void Codec::_messageWriteFloat(uint32_t field, float value) {
    _messageWriteField(field);
    _bb->writeFloat(value);
  }

  void Codec::_messageWriteString(uint32_t field, const std::string &value) {
    _messageWriteField(field);
    _bb->writeString(value);
  }

  ////////////////////////////////////////////////////////////////////////////////

  void Codec::_messageWriteArrayField() {
    assert(_bb && _countRemaining-- > 0);
  }

  void Codec::_messageWriteArrayBegin(uint32_t field, uint32_t count) {
    _messageWriteField(field);
    _bb->writeVarUint(count);
    _countRemaining = count;
  }

  void Codec::_messageWriteArrayByte(uint8_t value) {
    _messageWriteArrayField();
    _bb->writeByte(value);
  }

  void Codec::_messageWriteArrayVarInt(int32_t value) {
    _messageWriteArrayField();
    _bb->writeVarInt(value);
  }

  void Codec::_messageWriteArrayVarUint(uint32_t value) {
    _messageWriteArrayField();
    _bb->writeVarUint(value);
  }

  void Codec::_messageWriteArrayFloat(float value) {
    _messageWriteArrayField();
    _bb->writeFloat(value);
  }

  void Codec::_messageWriteArrayString(const std::string &value) {
    _messageWriteArrayField();
    _bb->writeString(value);
  }

  void Codec::_messageWriteArrayEnd() {
    assert(_bb && _countRemaining == 0); // Must write all elements
  }

  ////////////////////////////////////////////////////////////////////////////////

  void Codec::_messageFinish() {
    if (_bb) {
      _bb->writeVarUint(0);
      _bb = nullptr;
    }
  }
}

#endif
