#include "MsgParser.h"

#include "../tiny/tinyxml2/tinyxml2.h"
#include <string>
#include <iostream>

MsgParser::MsgParser() {}

void MsgParser::Parse()
{
    std::string xmlData = "<command><command_name>name</command_name></command>";

    using namespace tinyxml2;
    XMLDocument doc;

    doc.Parse(xmlData.c_str());

    XMLElement* commandElement = doc.FirstChildElement("command");
    if(commandElement)
    {
        XMLElement* commandNameElement = commandElement->FirstChildElement("command_name");
        if (commandNameElement)
        {
            std::cout << "Command Name: " << commandNameElement->GetText() << std::endl;
        }

        // XMLElement* paramsElement = commandElement->FirstChildElement("params");
        // if (paramsElement) {
        //     XMLElement* paramElement = paramsElement->FirstChildElement("param");
        //     while (paramElement) {
        //         std::cout << "Param: " << paramElement->GetText() << std::endl;
        //         paramElement = paramElement->NextSiblingElement("param");
        //     }
        // }
    }

}




