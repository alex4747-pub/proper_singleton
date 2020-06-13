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
#include <cstdlib>

namespace example {
ChipsetDriver* ChipsetDriver::self_;

ChipsetDriver& ChipsetDriver::GetInstance() noexcept {
  if (self_ == nullptr) {
    // We have a catastrophic bug
    abort();
  }

  return *self_;
}

ChipsetDriver::ChipsetDriver() {
  if (self_ != nullptr) {
    // We have a catastrophic bug
    abort();
  }

  self_ = this;
}

bool ChipsetDriver::InstantiationDone() { return self_ != nullptr; }

// In most real life scenarios singletons never deleted,
// so if a singleton supports 'reset' operation it is
// reponsibility of concrete singleton to delete self
// and call 'clear'
//
// It is worth noting that in unit test environments
// it is acceptable to simply leak singletons in order
// to avoid cleaning up half-backed structures after
// failed test cases
//
void ChipsetDriver::Clear() {
  // Just clear the pointer it is responsibility
  // of the caller to do cleanup.
  //
  self_ = nullptr;
}

}  // namespace example
