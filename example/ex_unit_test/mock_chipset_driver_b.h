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

// NOLINT(build/header_guard)
//
#ifndef INCLUDE_MOCK_CHIPSET_DRIVER_B_H_
#define INCLUDE_MOCK_CHIPSET_DRIVER_B_H_

#include <chipset_driver.h>
#include <simple_init_chain.h>

class MockChipsetDriverB : public example::ChipsetDriver {
 public:
  bool SendPacket(uint8_t const* data, size_t data_len) override;

  // Testing support manipulate test outcome
  void SetOutcome(bool val) { outcome_ = val; }

 private:
  MockChipsetDriverB();
  virtual ~MockChipsetDriverB();
  static bool DoInit(int level, simple::InitChain::ConfigMap const& configMap);
  static void DoReset(int level, simple::InitChain::ConfigMap const& configMap);

  bool outcome_;

  // In oder to do proper cleanup we have to know that we had installed
  // the concrete daemon
  //
  static bool init_done_;

  // Element of init chain
  //
  static simple::InitChain::El init_el_;
};

#endif  //  INCLUDE_MOCK_CHIPSET_DRIVER_B_H_
