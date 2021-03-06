// ---------------------------------------------------------------------------
// USB EPROM/Flash Programmer
//
// Copyright (2022) Robson Martins
//
// This work is licensed under a Creative Commons Attribution-NonCommercial-
// ShareAlike 4.0 International License.
// ---------------------------------------------------------------------------
/**
 * @ingroup Firmware
 * @file modules/opcodes.cpp
 * @brief Implementation of the Opcodes for communication.
 * 
 * @author Robson Martins (https://www.robsonmartins.com)
 */
// ---------------------------------------------------------------------------

#include <cmath>
#include "modules/opcodes.hpp"

// ---------------------------------------------------------------------------

TCmdOpCode& TCmdOpCode::operator=(const TCmdOpCode& src) {
  this->code   =   src.code;
  this->descr  =  src.descr;
  this->params = src.params;
  this->result = src.result;
  return *this;
}

bool operator==(const TCmdOpCode& a, const TCmdOpCode& b) {
  return (a.code   ==   b.code &&
          a.descr  ==  b.descr &&
          a.params == b.params &&
          a.result == b.result);
}

// ---------------------------------------------------------------------------

bool OpCode::isOk(const void *buf, size_t size) {
  if (!buf || !size) { return false; }
  const uint8_t *pbuf = static_cast<const uint8_t*>(buf);
  return (pbuf[0] == kCmdResponseOk);
}

TCmdOpCode OpCode::getOpCode(const void *buf, size_t size) {
  if (!buf || !size) { return kCmdOpCodes.at(kCmdNop); }
  const uint8_t *pbuf = static_cast<const uint8_t*>(buf);
  return getOpCode(pbuf[0]);
}

TCmdOpCode OpCode::getOpCode(uint8_t code) {
  auto it = kCmdOpCodes.find(static_cast<kCmdOpCodeEnum>(code));
  if (it == kCmdOpCodes.end()) { return kCmdOpCodes.at(kCmdNop); }
  return it->second;
}

float OpCode::getValueAsFloat(const void *buf, size_t size) {
  if (!buf || size < 3) { return 0.0f; }
  const uint8_t *pbuf = static_cast<const uint8_t*>(buf);
  return static_cast<float>(pbuf[1]) + static_cast<float>(pbuf[2]) * 0.01f;
}

uint8_t OpCode::getValueAsByte(const void *buf, size_t size) {
  if (!buf || size < 2) { return 0x00; }
  const uint8_t *pbuf = static_cast<const uint8_t*>(buf);
  return pbuf[1];
}

bool OpCode::getValueAsBool(const void *buf, size_t size) {
  return (getValueAsByte(buf, size) != kCmdParamOff);
}

bool OpCode::setValue(void *buf, size_t size, float value) {
  if (!buf || size < 3) { return false; }
  uint8_t *pbuf = static_cast<uint8_t*>(buf);
  double i, f = std::modf(value, &i);
  pbuf[1] = i;
  pbuf[2] = f * 100;
  return true;
}

bool OpCode::setValue(void *buf, size_t size, int value) {
  if (!buf || size < 2) { return false; }
  uint8_t *pbuf = static_cast<uint8_t*>(buf);
  pbuf[1] = value & 0xFF;
  return true;
}

bool OpCode::setValue(void *buf, size_t size, bool value) {
  return setValue(buf, size,
      static_cast<uint8_t>(value ? kCmdParamOn : kCmdParamOff));
}
