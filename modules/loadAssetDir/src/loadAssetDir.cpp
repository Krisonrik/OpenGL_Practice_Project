#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>

#include "tinyxml2.h"

#include "hvr/loadAssetDir/loadAssetDir.hpp"

loadAssetDir::loadAssetDir(){};
loadAssetDir::~loadAssetDir(){};
void loadAssetDir::createXML()
{
  tinyxml2::XMLDocument xmlDoc;
  tinyxml2::XMLNode* pRoot = xmlDoc.NewElement("Root");

  xmlDoc.InsertFirstChild(pRoot);
  xmlDoc.InsertFirstChild(pRoot);
  tinyxml2::XMLElement* pElement = xmlDoc.NewElement("IntValue");

  pElement->SetText(10);
  pRoot->InsertEndChild(pElement);
  pElement = xmlDoc.NewElement("FloatValue");
  pElement->SetText(0.5f);

  pRoot->InsertEndChild(pElement);

  pElement = xmlDoc.NewElement("Date");
  pElement->SetAttribute("day", 26);
  pElement->SetAttribute("month", "April");
  pElement->SetAttribute("year", 2014);
  pElement->SetAttribute("dateFormat", "26/04/2014");

  pRoot->InsertEndChild(pElement);

  pElement = xmlDoc.NewElement("List");

  for (const auto& item : vecList)
  {
    tinyxml2::XMLElement* pListElement = xmlDoc.NewElement("Item");
    pListElement->SetText(item);

    pElement->InsertEndChild(pListElement);
  }

  pElement->SetAttribute("itemCount", vecGroup.size());

  pRoot->InsertEndChild(pElement);

  tinyxml2::XMLError eResult = xmlDoc.SaveFile("SavedData.xml");
  tinyxml2::XMLCheckResult(eResult);

  return;
}
