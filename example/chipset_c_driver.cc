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
#include <simple_init_chain.h>
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
  static bool DoInit(int level, simple::InitChain::ConfigMap const& configMap);
  static void DoReset(int level, simple::InitChain::ConfigMap const& configMap);

  // In oder to do proper cleanup we have to know that we had installed
  // the concrete daemon
  //
  static bool init_done_;

  // Element of init chain
  //
  static simple::InitChain::El init_el_;
};

bool ChipsetCDriver::init_done_;

// Force initialization of c driver after a and b drivers
// for illustration of chipset presence test usage and init_done_
// check on reset
//
simple::InitChain::El ChipsetCDriver::init_el_(110, DoInit, DoReset);

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

bool ChipsetCDriver::DoInit(int, simple::InitChain::ConfigMap const&) {
  std::cout << "ChipsetCDrvier::DoInit called\n";

  if (InstantiationDone()) {
    std::cout << "ChipsetCDrvier::DoInit already instantiated\n";
    return true;
  }

  if (!IsChipsetPresent()) {
    std::cout << "ChipsetCDrvier::DoInit chipset not-present\n";
    return true;
  }

  std::cout << "ChipsetCDrvier::DoInit instantiation\n";

  assert(!init_done_);

  // Note: there is no memory leak here, becaue allocated
  // object is pointed to by self_ and will cleaned up
  // on reset if requiredo
  new ChipsetCDriver;
  init_done_ = true;  // NOLINT

  return true;
}

void ChipsetCDriver::DoReset(int, simple::InitChain::ConfigMap const&) {
  std::cout << "ChipsetCDrvier::DoReset called\n";

  if (!InstantiationDone()) {
    std::cout << "ChipsetCDrvier::DoReset nothing to do\n";
    return;
  }

  if (!init_done_) {
    std::cout << "ChipsetCDrvier::DoReset not instantiated by us\n";
    return;
  }

  std::cout << "ChipsetCDrvier::DoReset de-instantiation\n";

  // Note: we need a cast here because the base class does not have
  // public destructor, we use dynamic cast to check against
  // screwups
  ChipsetCDriver* ccd = dynamic_cast<ChipsetCDriver*>(&GetInstance());
  assert(ccd != nullptr);

  Clear();
  init_done_ = false;

  delete ccd;

  return;
}

}  // namespace chipset_c
