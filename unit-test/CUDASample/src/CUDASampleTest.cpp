// Copyright 2015 Jason Juang

#include <vector>

#include "CUDASampleTest.h"
#include "Hvr/CUDASample/CUDASample.cuh"

CUDASampleTest::CUDASampleTest()
{
}

CUDASampleTest::~CUDASampleTest()
{
}

void CUDASampleTest::SetUp()
{
}

void CUDASampleTest::TearDown()
{
}

TEST_F(CUDASampleTest, AddOneToVector)
{
  std::vector<int> v;
  std::vector<int> v_cpu;

  for (int i = 0; i < 4096; i++)
  {
    v.push_back(i);
    v_cpu.push_back(i + 1);
  }

  hvr::CUDASample cudasample;

  cudasample.AddOneToVector(v);

  EXPECT_EQ(v, v_cpu);
}
