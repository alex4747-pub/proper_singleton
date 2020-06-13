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
namespace chipset_b {
class ChipsetBDriver : public example::ChipsetDriver {
 public:
  bool SendPacket(uint8_t const* data, size_t data_len) override;

 private:
  ChipsetBDriver();
  virtual ~ChipsetBDriver();
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

bool ChipsetBDriver::init_done_;

// Force initialization of b driver after a driver
// for consistency
//
simple::InitChain::El ChipsetBDriver::init_el_(105, DoInit, DoReset);

bool ChipsetBDriver::SendPacket(uint8_t const*, size_t) {
  std::cout << "ChipsetBDrvier::SendPacket called\n";
  return true;
}

ChipsetBDriver::ChipsetBDriver() {}

ChipsetBDriver::~ChipsetBDriver() {}

bool ChipsetBDriver::IsChipsetPresent() {
  // We always simulate no presence of chipset-b
  return false;
}

bool ChipsetBDriver::DoInit(int, simple::InitChain::ConfigMap const&) {
  std::cout << "ChipsetBDrvier::DoInit called\n";

  if (InstantiationDone()) {
    std::cout << "ChipsetBDrvier::DoInit already instantiated\n";
    return true;
  }

  if (!IsChipsetPresent()) {
    std::cout << "ChipsetBDrvier::DoInit chipset not-present\n";
    return true;
  }

  std::cout << "ChipsetBDrvier::DoInit instantiation\n";

  assert(!init_done_);

  new ChipsetBDriver;
  init_done_ = true;

  return true;
}

void ChipsetBDriver::DoReset(int, simple::InitChain::ConfigMap const&) {
  std::cout << "ChipsetBDrvier::DoReset called\n";

  if (!InstantiationDone()) {
    std::cout << "ChipsetBDrvier::DoReset nothing to do\n";
    return;
  }

  if (!init_done_) {
    std::cout << "ChipsetBDrvier::DoReset not instantiated by us\n";
    return;
  }

  std::cout << "ChipsetBDrvier::DoReset de-instantiation\n";

  // Note: we need a cast here because the base class does not have
  // public destructor, we use dynamic cast to check against
  // screwups
  ChipsetBDriver* cbd = dynamic_cast<ChipsetBDriver*>(&GetInstance());
  assert(cbd != nullptr);

  Clear();
  init_done_ = false;

  delete cbd;

  return;
}

}  // namespace chipset_b
