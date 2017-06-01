// Copyright 2015 Jason Juang

#include "SampleTest.h"

#include <string>

#include "Hvr/Sample/Sample.h"

HVR_WINDOWS_DISABLE_ALL_WARNING
#include "opencv2/opencv.hpp"
HVR_WINDOWS_ENABLE_ALL_WARNING

SampleTest::SampleTest()
{
}

SampleTest::~SampleTest()
{
}

void SampleTest::SetUp()
{
}

void SampleTest::TearDown()
{
}

TEST_F(SampleTest, AddTwoIntegerTestGood)
{
  hvr::Sample sample;

  EXPECT_EQ(sample.AddTwoInteger(2, 3), 5);
}

TEST_F(SampleTest, TurnImageToWhiteTestGood)
{
  hvr::Sample sample;

  bool correct = true;

  cv::Mat img = cv::Mat(100, 100, CV_8UC3, cv::Scalar(0));

  sample.TurnImageToWhite(img);

  const int h = img.rows;
  const int w = img.cols;

  for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++)
      for (int c = 0; c < 3; c++)
      {
        if (img.at<cv::Vec3b>(i, j).val[c] != 255) correct = false;
      }

  EXPECT_EQ(correct, true);
}
