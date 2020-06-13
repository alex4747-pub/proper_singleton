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

#include <mock_chipset_driver_b.h>
#include <mock_chipset_driver_cfg.h>
#include <cassert>
#include <iostream>

bool MockChipsetDriverB::init_done_;

// We give it negative initialization level to make sure
// that it is initialized before the actual drivers
simple::InitChain::El MockChipsetDriverB::init_el_(-1, DoInit, DoReset);

bool MockChipsetDriverB::SendPacket(uint8_t const*, size_t) {
  std::cout << "MockChipsetDrvierB::SendPacket called:"
            << "  result: " << (outcome_ ? "success" : "failure") << "\n";
  return outcome_;
}

MockChipsetDriverB::MockChipsetDriverB() : outcome_(true) {}

MockChipsetDriverB::~MockChipsetDriverB() {}

bool MockChipsetDriverB::DoInit(int, simple::InitChain::ConfigMap const& map) {
  std::cout << "MockChipsetDrvierB::DoInit called\n";

  if (InstantiationDone()) {
    std::cout << "MockChipsetDrvierB::DoInit already instantiated\n";
    return true;
  }

  auto cit = map.find(MOCK_CHIPSET_DRIVER_CFG_KEY);

  if (cit == map.end() || (*cit).second != "b") {
    std::cout << "MockChipsetDrvierB::DoInit not configured\n";
    return true;
  }

  std::cout << "MockChipsetDrvierB::DoInit instantiation\n";

  assert(!init_done_);

  new MockChipsetDriverB;
  init_done_ = true;

  return true;
}

void MockChipsetDriverB::DoReset(int, simple::InitChain::ConfigMap const&) {
  std::cout << "MockChipsetDrvierB::DoReset called\n";

  if (!InstantiationDone()) {
    std::cout << "MockChipsetDrvierB::DoReset nothing to do\n";
    return;
  }

  if (!init_done_) {
    std::cout << "MockChipsetDrvierB::DoReset not instantiated by us\n";
    return;
  }

  std::cout << "MockChipsetDrvierB::DoReset de-instantiation\n";

  // Note: we need a cast here because the base class does not have
  // public destructor, we use dynamic cast to check against
  // screwups
  MockChipsetDriverB* mcdb = dynamic_cast<MockChipsetDriverB*>(&GetInstance());
  assert(mcdb != nullptr);

  Clear();
  init_done_ = false;

  delete mcdb;

  return;
}
