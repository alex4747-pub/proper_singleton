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

#include <mock_chipset_driver_a.h>
#include <mock_chipset_driver_cfg.h>
#include <cassert>
#include <iostream>

bool MockChipsetDriverA::init_done_;

// We give it negative initialization level to make sure
// that it is initialized before the actual drivers
simple::InitChain::El MockChipsetDriverA::init_el_(-1, DoInit, DoReset);

bool MockChipsetDriverA::SendPacket(uint8_t const*, size_t) {
  std::cout << "MockChipsetDrvierA::SendPacket called:"
            << "  result: " << (outcome_ ? "success" : "failure") << "\n";
  return outcome_;
}

MockChipsetDriverA::MockChipsetDriverA() : outcome_(true) {}

MockChipsetDriverA::~MockChipsetDriverA() {}

bool MockChipsetDriverA::DoInit(int, simple::InitChain::ConfigMap const& map) {
  std::cout << "MockChipsetDrvierA::DoInit called\n";

  if (InstantiationDone()) {
    std::cout << "MockChipsetDrvierA::DoInit already instantiated\n";
    return true;
  }

  auto cit = map.find(MOCK_CHIPSET_DRIVER_CFG_KEY);

  if (cit == map.end() || (*cit).second != "a") {
    std::cout << "MockChipsetDrvierA::DoInit not configured\n";
    return true;
  }

  std::cout << "MockChipsetDrvierA::DoInit instantiation\n";

  assert(!init_done_);

  new MockChipsetDriverA;
  init_done_ = true;

  return true;
}

void MockChipsetDriverA::DoReset(int, simple::InitChain::ConfigMap const&) {
  std::cout << "MockChipsetDrvierA::DoReset called\n";

  if (!InstantiationDone()) {
    std::cout << "MockChipsetDrvierA::DoReset nothing to do\n";
    return;
  }

  if (!init_done_) {
    std::cout << "MockChipsetDrvierA::DoReset not instantiated by us\n";
    return;
  }

  std::cout << "MockChipsetDrvierA::DoReset de-instantiation\n";

  // Note: we need a cast here because the base class does not have
  // public destructor, we use dynamic cast to check against
  // screwups
  MockChipsetDriverA* mcda = dynamic_cast<MockChipsetDriverA*>(&GetInstance());
  assert(mcda != nullptr);

  Clear();
  init_done_ = false;

  delete mcda;

  return;
}
