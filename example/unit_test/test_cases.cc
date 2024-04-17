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

#include <gtest/gtest.h>
#include <mock_chipset_driver.h>
#include <network_driver.h>


class NetworkDriverTest : public ::testing::Test {
 protected:

  class TestRunner : public simple::InitChain::Runner {
   public:
    bool Run() noexcept { return DoRun(); }
    bool Reset() noexcept { return DoReset(); }
    bool Release() noexcept { return DoRelease(); }
  };

  void SetUp() override {
    // Run init chain
    bool res = runner_.Run();
    EXPECT_TRUE(res);

    // Demonstrate that mock chipset driveer
    // is instantiated
    //
    auto mcd = dynamic_cast<MockChipsetDriver*>(
        &example::ChipsetDriver::GetInstance());
    EXPECT_TRUE(mcd);

    // Demonstrate that it was create from
    // because it has default outcome value
    EXPECT_TRUE(mcd->GetOutcome());
    
    nd_ = new example::NetworkDriver;
  }

  void TearDown() override {
    delete nd_;
    nd_ = 0;

    bool res = runner_.Reset();
    EXPECT_TRUE(res);
  }

  TestRunner runner_;
  example::NetworkDriver* nd_;
};

TEST_F(NetworkDriverTest, SendPackerPos) {
  // Positive test of send packet
  auto mcd =
      dynamic_cast<MockChipsetDriver*>(&example::ChipsetDriver::GetInstance());
  EXPECT_TRUE(mcd);

  EXPECT_TRUE(mcd->GetOutcome());

  bool res = nd_->Transmit(reinterpret_cast<uint8_t const*>("test-data"), 9);
  EXPECT_TRUE(res);

  // Set non-default outcome value to demonstrate
  // destruction and recreation of mock chipset
  // driver instance
  //
  mcd->SetOutcome(false);
}

TEST_F(NetworkDriverTest, SendPackerNeg) {
  // Negative test of send packet

  auto mcd =
      dynamic_cast<MockChipsetDriver*>(&example::ChipsetDriver::GetInstance());
  EXPECT_TRUE(mcd);

  EXPECT_TRUE(mcd->GetOutcome());

  mcd->SetOutcome(false);

  bool res = nd_->Transmit(reinterpret_cast<uint8_t const*>("test-data"), 9);
  EXPECT_FALSE(res);
}
