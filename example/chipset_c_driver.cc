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
#include <cassert>
#include <iostream>

// This is a concrete driver for chipset-a
// Note: there is no need to have these definitions
// publically visible, it is OK to define all of it
// in .cc file.
//
namespace chipset_c {
class ChipsetCDriver : public example::ChipsetDriver {
 public:
  bool SendPacket(uint8_t const* data, size_t data_len) override;

 private:
  ChipsetCDriver();
  virtual ~ChipsetCDriver();
  static bool IsChipsetPresent();


  // In oder to do proper cleanup we have to know that we had installed
  // the concrete daemon
  //
  static bool init_done_;

  static bool InitFunc();
  static bool ResetFunc();
  
  static simple::InitChain::Link init_worker_;
};

bool ChipsetCDriver::init_done_;

// Force initialization of c driver after a and b drivers
// for illustration of chipset presence test usage and init_done_
// check on reset
//
simple::InitChain::Link ChipsetCDriver::init_worker_(110, InitFunc, ResetFunc);

bool ChipsetCDriver::SendPacket(uint8_t const*, size_t) {
  std::cout << "ChipsetCDrvier::SendPacket called\n";
  return true;
}

ChipsetCDriver::ChipsetCDriver() {}

ChipsetCDriver::~ChipsetCDriver() {}

bool ChipsetCDriver::IsChipsetPresent() {
  // We always simulate presence of chipset-c
  return true;
}

bool ChipsetCDriver::InitFunc() {
  if (InstantiationDone()) {
    std::cout << "ChipsetCDrvier::InitFunc already: instantiated\n";
    return true;
  }

  if (!IsChipsetPresent()) {
    std::cout << "ChipsetCDrvier::InitFunc: chipset not-present\n";
    return true;
  }

  std::cout << "ChipsetCDrvier::InitFunc: instantiation\n";

  assert(!ChipsetCDriver::init_done_);

  // Note: there is no memory leak here, becaue allocated
  // object is pointed to by self_ and will cleaned up
  // on reset if requiredo
  new ChipsetCDriver;
  ChipsetCDriver::init_done_ = true;  // NOLINT

  return true;
}

bool ChipsetCDriver::ResetFunc() {
  if (!InstantiationDone()) {
    std::cout << "ChipsetCDrvier::ResetFunc: nothing to do\n";
    return true;
  }

  if (!ChipsetCDriver::init_done_) {
    std::cout << "ChipsetCDrvier::ResetFunc: not instantiated by us\n";
    return true;
  }

  std::cout << "ChipsetCDrvier::ResetFunc: delete self\n";

  // Note: we need a cast here because the base class does not have
  // public destructor, we use dynamic cast to check against
  // screwups
  ChipsetCDriver* ccd = dynamic_cast<ChipsetCDriver*>(&GetInstance());
  assert(ccd != nullptr);

  Clear();
  ChipsetCDriver::init_done_ = false;

  delete ccd;

  return true;
}

}  // namespace chipset_c
