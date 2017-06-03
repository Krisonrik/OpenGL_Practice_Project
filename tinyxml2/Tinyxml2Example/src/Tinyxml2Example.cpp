// Copyright 2015 Jason Juang

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <tinyxml2.h>

#include "glog/logging.h"
HVR_WINDOWS_ENABLE_ALL_WARNING

int main(int argc, char **argv)
{
  google::InitGoogleLogging(argv[0]);
  FLAGS_log_prefix = false;

#ifndef NDEBUG
  google::SetStderrLogging(google::INFO);
  FLAGS_log_prefix = true;
#endif

  if (argc != 2)
  {
    LOG(ERROR) << "Please input something like:";
    LOG(ERROR) << "/path/to/input.xml";
    return -1;
  }

  tinyxml2::XMLDocument doc_xml;

  if (doc_xml.LoadFile(argv[1]))
  {
    LOG(ERROR) << "failed to load config file";
    return false;
  }
  else
  {
    tinyxml2::XMLHandle xml(&doc_xml);

    LOG(INFO) << xml.FirstChildElement("CONFIG")
                     .FirstChildElement("SETTING")
                     .ToElement()
                     ->GetText();

    int x;
    float y;

    xml.FirstChildElement("CONFIG")
        .FirstChildElement("VALUE")
        .FirstChildElement("x")
        .ToElement()
        ->QueryIntText(&x);
    xml.FirstChildElement("CONFIG")
        .FirstChildElement("VALUE")
        .FirstChildElement("y")
        .ToElement()
        ->QueryFloatText(&y);

    LOG(INFO) << x;
    LOG(INFO) << y;
  }

  return 0;
}
