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
#include <mock_chipset_driver_a.h>
#include <mock_chipset_driver_b.h>
#include <mock_chipset_driver_cfg.h>
#include <network_driver.h>

class NetworkDriverTestA : public ::testing::Test {
 protected:
  void SetUp() override {
    simple::InitChain::ConfigMap map;
    map.emplace(MOCK_CHIPSET_DRIVER_CFG_KEY, "a");

    bool res = simple::InitChain::Run(map);
    EXPECT_TRUE(res);

    // Make sure expected mock is instantiated
    auto mcda = dynamic_cast<MockChipsetDriverA*>(
        &example::ChipsetDriver::GetInstance());
    EXPECT_TRUE(mcda);

    nd_ = new example::NetworkDriver;
  }

  void TearDown() override {
    delete nd_;
    nd_ = 0;

    simple::InitChain::Reset();
  }

  example::NetworkDriver* nd_;
};

TEST_F(NetworkDriverTestA, SendPackerPos) {
  // Positive test of send packet
  bool res = nd_->Transmit(reinterpret_cast<uint8_t const*>("test-data"), 9);
  EXPECT_TRUE(res);
}

TEST_F(NetworkDriverTestA, SendPackerNeg) {
  // Negative test of send packet

  // Prepare mock chipset driver
  auto mcda =
      dynamic_cast<MockChipsetDriverA*>(&example::ChipsetDriver::GetInstance());
  EXPECT_TRUE(mcda);

  mcda->SetOutcome(false);

  bool res = nd_->Transmit(reinterpret_cast<uint8_t const*>("test-data"), 9);
  EXPECT_FALSE(res);
}

class NetworkDriverTestB : public ::testing::Test {
 protected:
  void SetUp() override {
    simple::InitChain::ConfigMap map;
    map.emplace(MOCK_CHIPSET_DRIVER_CFG_KEY, "b");

    bool res = simple::InitChain::Run(map);
    EXPECT_TRUE(res);

    // Make sure expected mock is instantiated
    auto mcdb = dynamic_cast<MockChipsetDriverB*>(
        &example::ChipsetDriver::GetInstance());
    EXPECT_TRUE(mcdb);

    nd_ = new example::NetworkDriver;
  }

  void TearDown() override {
    delete nd_;
    nd_ = 0;

    simple::InitChain::Reset();
  }

  example::NetworkDriver* nd_;
};

TEST_F(NetworkDriverTestB, SendPackerPos) {
  // Positive test of send packet
  bool res = nd_->Transmit(reinterpret_cast<uint8_t const*>("test-data"), 9);
  EXPECT_TRUE(res);
}

TEST_F(NetworkDriverTestB, SendPackerNeg) {
  // Negative test of send packet

  // Prepare mock chipset driver
  auto mcdb =
      dynamic_cast<MockChipsetDriverB*>(&example::ChipsetDriver::GetInstance());
  EXPECT_TRUE(mcdb);

  mcdb->SetOutcome(false);

  bool res = nd_->Transmit(reinterpret_cast<uint8_t const*>("test-data"), 9);
  EXPECT_FALSE(res);
}
