// Copyright 2015 Jason Juang

#ifndef UNIT_TEST_CUDASAMPLE_INCLUDE_CUDASAMPLETEST_H_
#define UNIT_TEST_CUDASAMPLE_INCLUDE_CUDASAMPLETEST_H_

HVR_WINDOWS_DISABLE_ALL_WARNING
#include "gtest/gtest.h"
HVR_WINDOWS_ENABLE_ALL_WARNING

class CUDASampleTest : public ::testing::Test
{
 protected:
  CUDASampleTest();
  ~CUDASampleTest() override;

  void SetUp() override;
  void TearDown() override;
};

#endif  // UNIT_TEST_CUDASAMPLE_INCLUDE_CUDASAMPLETEST_H_
