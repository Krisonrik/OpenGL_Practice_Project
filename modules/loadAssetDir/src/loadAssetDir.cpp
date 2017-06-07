// Copyright 2015 Jason Juang

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <tinyxml2.h>

#include "glog/logging.h"
HVR_WINDOWS_ENABLE_ALL_WARNING
#include <string.h>
#include "hvr/loadAssetDir/loadAssetDir.hpp"

loadAssetDir::loadAssetDir(){};
loadAssetDir::~loadAssetDir(){};

void loadAssetDir::loadXML(char* argv, std::string& str)
{
  tinyxml2::XMLDocument doc_xml;
  // int x;
  // float y;
  const char* dir;

  if (doc_xml.LoadFile(argv))
  {
    LOG(ERROR) << "failed to load config file";
    return;
  }
  else
  {
    tinyxml2::XMLHandle xml(&doc_xml);

    // LOG(INFO) << xml.FirstChildElement("CONFIG")
    //                 .FirstChildElement("VALUE")
    //                 .FirstChildElement("dir")
    //                 .ToElement()
    //                 ->GetText();
    dir = xml.FirstChildElement("CONFIG")
              .FirstChildElement("VALUE")
              .FirstChildElement("dir")
              .ToElement()
              ->GetText();
    str.assign(dir);
    // xml.FirstChildElement("CONFIG")
    //    .FirstChildElement("VALUE")
    //    .FirstChildElement("x")
    //    .ToElement()
    //    ->QueryIntText(&x);
    // xml.FirstChildElement("CONFIG")
    //    .FirstChildElement("VALUE")
    //    .FirstChildElement("y")
    //    .ToElement()
    //    ->QueryFloatText(&y);

    // LOG(INFO) << x;
    // LOG(INFO) << y;
  }

  return;
}
