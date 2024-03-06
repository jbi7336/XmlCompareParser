#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

void initXmlRet(XMLDocument*);
void Test(XMLDocument& xmlEng, XMLDocument& xmlRet);
void LoadXml(XMLDocument& xmlEng, XMLDocument& xmlKor);
void compareAndInsert(XMLElement* rootKor, XMLElement* rootEng, XMLDocument& xmlRet);

#define DEBUG 1

int main() {
	int Flag = 1;
	XMLDocument xmlEng, xmlKor, xmlRet;
	LoadXml(xmlEng, xmlKor);
	initXmlRet(&xmlRet);

	XMLElement* rootEng = xmlEng.RootElement();
	XMLElement* rootKor = xmlKor.RootElement();
	
	compareAndInsert(rootKor, rootEng, xmlRet);

	cout << xmlRet.RootElement()->ChildElementCount() << endl;

	DEBUG ? xmlRet.SaveFile("./out/TransEngToKor_Skill.xml") : xmlRet.SaveFile("./out/Test.img.xml");

	return 0;
}

void compareAndInsert(XMLElement* rootKor, XMLElement* rootEng, XMLDocument& xmlRet)
{
	XMLElement* node;
	XMLElement* nodeChild;

	XMLElement* eleKor = rootKor->FirstChildElement("imgdir");
	XMLElement* eleEng = rootEng->FirstChildElement("imgdir");

	// Imgdir layer cycle
	while (eleEng != NULL && eleKor != NULL) {
		int valueEng, valueKor, compareCase;

		valueEng = stoi(eleEng->FirstAttribute()->Value());
		valueKor = stoi(eleKor->FirstAttribute()->Value());

		compareCase = valueEng - valueKor;
		if (compareCase == 0) { // Same Word
			xmlRet.LastChildElement()->InsertEndChild(xmlRet.NewElement("imgdir"));
			node = xmlRet.LastChildElement()->LastChildElement();
			node->SetAttribute("name", eleEng->FindAttribute("name")->Value());

			for (XMLElement* eleChild = eleKor->FirstChildElement(); eleChild != NULL; eleChild = eleChild->NextSiblingElement()) {
				node->InsertEndChild(xmlRet.NewElement("string"));
				nodeChild = node->LastChildElement();

				nodeChild->SetAttribute(
					"name",
					eleChild->FirstAttribute()->Value()
				);

				nodeChild->SetAttribute(
					"value",
					eleChild->FirstAttribute()->Next()->Value()
				);
			}

			eleEng = eleEng->NextSiblingElement();
			eleKor = eleKor->NextSiblingElement();
		} else if (compareCase < 0) { // ENG small than KOR
			xmlRet.LastChildElement()->InsertEndChild(xmlRet.NewElement("imgdir"));
			node = xmlRet.LastChildElement()->LastChildElement();
			node->SetAttribute("name", eleEng->FindAttribute("name")->Value());

			for (XMLElement* eleChild = eleEng->FirstChildElement(); eleChild != NULL; eleChild = eleChild->NextSiblingElement()) {
				node->InsertEndChild(xmlRet.NewElement("string"));
				nodeChild = node->LastChildElement();

				nodeChild->SetAttribute(
					"name",
					eleChild->FirstAttribute()->Value()
				);

				nodeChild->SetAttribute(
					"value",
					eleChild->FirstAttribute()->Next()->Value()
				);
			}

			eleEng = eleEng->NextSiblingElement();
		} else { // ENG bigger than KOR

			eleKor = eleKor->NextSiblingElement();
		}
	}
}

void LoadXml(XMLDocument& xmlEng, XMLDocument& xmlKor)
{
	if (XML_SUCCESS == xmlEng.LoadFile("./data/Skill_eng.img.xml")) {
		cout << "Success" << endl;
	} else {
		cout << "Fail to load" << endl;
	}

	if (XML_SUCCESS == xmlKor.LoadFile("./data./Skill_kor.img.xml")) {
		cout << "Success" << endl;
	} else {
		cout << "Fail to load" << endl;
	}
}

void Test(XMLDocument& xmlKor, XMLDocument& xmlRet)
{
	XMLElement* root = xmlKor.RootElement();
	XMLElement* node;
	XMLElement* nodeChild;


	if (0 == 1) {
		// imgdir layer cycle
		for (XMLElement* ele = root->FirstChildElement("imgdir"); ele != NULL; ele = ele->NextSiblingElement()) {
			xmlRet.LastChildElement()->InsertEndChild(xmlRet.NewElement("imgdir"));
			node = xmlRet.LastChildElement()->LastChildElement();

			string value = ele->FindAttribute("name")->Value();
			node->SetAttribute("name", value.data());

			// strimg layer cycle
			for (XMLElement* eleChild = ele->FirstChildElement(); eleChild != NULL; eleChild = eleChild->NextSiblingElement()) {
				node->InsertEndChild(xmlRet.NewElement("string"));
				nodeChild = node->LastChildElement();

				string childName = eleChild->FirstAttribute()->Value();
				string childValue = eleChild->FirstAttribute()->Next()->Value();
				nodeChild->SetAttribute("name", childName.data());
				nodeChild->SetAttribute("value", childValue.data());
			}
		}
	}
}

void initXmlRet(XMLDocument* xmlRet) {
	// Skill_eng.img
	xmlRet->InsertFirstChild(xmlRet->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\""));
	xmlRet->InsertEndChild(xmlRet->NewElement("imgdir"));
	xmlRet->FirstChildElement()->SetAttribute("name", "Skill.img");
}