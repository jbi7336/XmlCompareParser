#include <iostream>
#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

void initXmlRet(XMLDocument*, string in);
void LoadXml(XMLDocument& xmlEng, XMLDocument& xmlKor, string in);
void compareAndInsert(XMLElement* rootKor, XMLElement* rootEng, XMLDocument& xmlRet);
void InsertChild(XMLElement* eleKor, XMLElement* node, XMLDocument& xmlRet, XMLElement*& nodeChild, int kor);
void fillBlank(string& temp);

int o = 0, t = 0, th = 0;

#define DEBUG 0

int main() {
	XMLDocument xmlEng, xmlKor, xmlRet;
	string inName;

	cin >> inName;

	LoadXml(xmlEng, xmlKor, inName);
	initXmlRet(&xmlRet, inName);

	XMLElement* rootEng = xmlEng.RootElement();
	XMLElement* rootKor = xmlKor.RootElement();

	compareAndInsert(rootKor, rootEng, xmlRet);

	if (DEBUG) {
		xmlRet.SaveFile("./out/Test.img.xml");
	} else {
		string result = "./out/TransEngToKor_" + inName + ".img.xml";
		xmlRet.SaveFile(result.data());
	}

	if (DEBUG) cout << o << " " << t << " " << th << endl;
	return 0;
}

void sortingXML() {
	XMLDocument xmlTemp;
}

void compareAndInsert(XMLElement* rootKor, XMLElement* rootEng, XMLDocument& xmlRet) {
	XMLElement* node;
	XMLElement* nodeChild;

	XMLElement* eleKor = rootKor->FirstChildElement("imgdir");
	XMLElement* eleEng = rootEng->FirstChildElement("imgdir");

	// Imgdir layer cycle
	while (eleEng != NULL && eleKor != NULL) {
		int valueEng, valueKor, compareCase;

		valueEng = stoi(eleEng->FirstAttribute()->Value());
		valueKor = stoi(eleKor->FirstAttribute()->Value());

		cout << valueEng << endl;

		compareCase = valueEng - valueKor;
		if (compareCase == 0) { // Same Word
			xmlRet.LastChildElement()->InsertEndChild(xmlRet.NewElement("imgdir"));
			node = xmlRet.LastChildElement()->LastChildElement();
			node->SetAttribute("name", eleKor->FindAttribute("name")->Value());

			InsertChild(eleKor, node, xmlRet, nodeChild, 0);

			eleEng = eleEng->NextSiblingElement();
			eleKor = eleKor->NextSiblingElement();
			DEBUG ? o = o + 1 : o = o;
		} else if (compareCase < 0) { // ENG small than KOR
			xmlRet.LastChildElement()->InsertEndChild(xmlRet.NewElement("imgdir"));
			node = xmlRet.LastChildElement()->LastChildElement();
			node->SetAttribute("name", eleEng->FindAttribute("name")->Value());

			InsertChild(eleEng, node, xmlRet, nodeChild, 0);

			eleEng = eleEng->NextSiblingElement();
			DEBUG ? t = t + 1 : t = t;
		} else { // ENG bigger than KOR

			eleKor = eleKor->NextSiblingElement();
			DEBUG ? th = th + 1 : th = th;
		}
	}
}

void InsertChild(XMLElement* eleKor, XMLElement* node, XMLDocument& xmlRet, XMLElement*& nodeChild, int kor) {
    string temp;
	int flag;

	for (XMLElement* eleChild = eleKor->FirstChildElement(); eleChild != NULL; eleChild = eleChild->NextSiblingElement()) {
		node->InsertEndChild(xmlRet.NewElement("string"));
		nodeChild = node->LastChildElement();

		temp = eleChild->FirstAttribute()->Value();
		if (&temp == NULL) {
			nodeChild->SetAttribute("name", "");
		} else {
			nodeChild->SetAttribute("name", temp.data());
		}

		temp = eleChild->FirstAttribute()->Next()->Value();

		if (&temp == NULL) {
			nodeChild->SetAttribute("name", "");
		} else {
			nodeChild->SetAttribute("name", temp.data());
		}
	}
}

void fillBlank(string& temp) {
	while (temp.length() < 100) {
		temp.push_back(' ');
	}
}

void LoadXml(XMLDocument& xmlEng, XMLDocument& xmlKor, string in) {
	string eng, kor;

	eng = "./data/" + in + "/eng/" + in + ".img.xml";
	kor = "./data/" + in + "/kor/" + in + ".img.xml";

	if (XML_SUCCESS == xmlEng.LoadFile(eng.data())) {
		cout << "Success" << endl;
	} else {
		cout << "Fail to load" << endl;
	}

	if (XML_SUCCESS == xmlKor.LoadFile(kor.data())) {
		cout << "Success" << endl;
	} else {
		cout << "Fail to load" << endl;
	}
}

void initXmlRet(XMLDocument* xmlRet, string in) {
	// Skill_eng.img
	string temp = in + ".img";
	xmlRet->InsertFirstChild(xmlRet->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\""));
	xmlRet->InsertEndChild(xmlRet->NewElement("imgdir"));
	xmlRet->FirstChildElement()->SetAttribute("name", temp.data());
}