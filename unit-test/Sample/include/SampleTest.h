// Copyright 2015 Jason Juang

#ifndef UNIT_TEST_SAMPLE_INCLUDE_SAMPLETEST_H_
#define UNIT_TEST_SAMPLE_INCLUDE_SAMPLETEST_H_

HVR_WINDOWS_DISABLE_ALL_WARNING
#include "gtest/gtest.h"
HVR_WINDOWS_ENABLE_ALL_WARNING

class SampleTest : public ::testing::Test
{
 protected:
  SampleTest();
  ~SampleTest() override;

  void SetUp() override;
  void TearDown() override;
};

#endif  // UNIT_TEST_SAMPLE_INCLUDE_SAMPLETEST_H_
