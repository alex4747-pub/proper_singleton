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
#include <network_driver.h>
#include <simple_init_chain.h>
#include <cassert>
#include <iostream>

int main(int, char**) {
  bool res = simple::InitChain::Run();
  assert(res);

  auto nd = new example::NetworkDriver;

  res = nd->Transmit(reinterpret_cast<uint8_t const*>("test-data"), 9);

  std::cout << "Transmit result: " << (res ? "success" : "failure") << "\n";

  std::cout << "========= Performing reset and another run ==========\n";

  simple::InitChain::Reset();

  res = simple::InitChain::Run();
  assert(res);

  res = nd->Transmit(reinterpret_cast<uint8_t const*>("test-data"), 9);

  std::cout << "Transmit result: " << (res ? "success" : "failure") << "\n";

  return 0;
}
