#include "MsgParser.h"

#include "../tiny/tinyxml2/tinyxml2.h"
#include <vector>

using namespace tinyxml2;

std::string MsgParser::StateChange(const std::string& msg)
{
    tinyxml2::XMLDocument doc;

    // Create the root element "Baccarat"
    tinyxml2::XMLElement* root = doc.NewElement("Baccarat");
    doc.InsertFirstChild(root);

    // Create the first child element "changestate" with an attribute "name"
    tinyxml2::XMLElement* changestate = doc.NewElement("change_state");
    changestate->SetAttribute("name", msg.c_str());
    root->InsertFirstChild(changestate);



    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);

    return std::string(printer.CStr());
}

std::string MsgParser::DealtCards(const std::string& role, Hand& hand)
{
    tinyxml2::XMLDocument doc;
    // Create the root element "Baccarats"

    // Create the root element
    tinyxml2::XMLElement* root = doc.NewElement(role.c_str());
    doc.InsertFirstChild(root);

    // Iterate through the deck and create XML elements for each card
    for (auto& card : hand.cards)
    {
        tinyxml2::XMLElement* cardElem = doc.NewElement("Card");
        tinyxml2::XMLElement* faceElem = doc.NewElement("Face");
        faceElem->SetText(card.face);
        cardElem->InsertEndChild(faceElem);

        tinyxml2::XMLElement* suitElem = doc.NewElement("Suit");
        suitElem->SetText(card.suit);
        cardElem->InsertEndChild(suitElem);

        root->InsertEndChild(cardElem);
    }

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);

    return std::string(printer.CStr());
}

std::string MsgParser::RoundOutcome(Outcome outcome)
{
    tinyxml2::XMLDocument doc;

    // Create the root element
    tinyxml2::XMLElement* root = doc.NewElement("outcome");
    doc.InsertFirstChild(root);
    tinyxml2::XMLElement* elem = doc.NewElement("outcome");
    elem->SetText(static_cast<int>(outcome));
    // elem->InsertEndChild(suitElem);

    root->InsertEndChild(elem);

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);

    return std::string(printer.CStr());
}




