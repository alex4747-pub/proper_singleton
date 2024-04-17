// Copyright (C) 2020  Aleksey Romanov (aleksey at voltanet dot io)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <chipset_driver.h>
#include <InitChain.h>
#include <iostream>
#include <map>
#include <string>

// This is a concrete driver for chipset-a
// Note: there is no need to have these definitions
// publically visible, it is OK to define all of it
// in .cc file.
//
namespace chipset_a {
class ChipsetADriver : public example::ChipsetDriver {
 public:
  bool SendPacket(uint8_t const* data, size_t data_len) override;

 private:
  ChipsetADriver();
  virtual ~ChipsetADriver();
  static bool IsChipsetPresent();

  static bool InitFunc();
  static simple::InitChain::Link init_worker_;
};


simple::InitChain::Link ChipsetADriver::init_worker_(100, InitFunc);

bool ChipsetADriver::SendPacket(uint8_t const*, size_t) {
  // It should not happen - in our example we simulate presense of chipset-b
  //
  abort();

  std::cout << "ChipsetADrvier::SendPacket called\n";
  return true;
}

ChipsetADriver::ChipsetADriver() {
  // It should not happen - in our example we simulate presense of chipset-b
  //
  abort();
}

ChipsetADriver::~ChipsetADriver() {
  // Should never be called
  abort();
}

bool ChipsetADriver::IsChipsetPresent() {
  // We always simulate presence of chipset-b
  return false;
}

bool ChipsetADriver::InitFunc() {
  if (InstantiationDone()) {
    std::cout << "ChipsetADrvier::InitFunc: already instantiated\n";
    return true;
  }
  if (!IsChipsetPresent()) {
    std::cout << "ChipsetADrvier::InitFunc: chipset not-present\n";
    return true;
  }

  std::cout << "ChipsetADrvier::IntiFunc: instantiation\n";
  new ChipsetADriver;
  return true;
}
}  // namespace chipset_a
