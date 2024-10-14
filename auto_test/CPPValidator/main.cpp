#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <regex>

using namespace std;
const string EVENT = "event";
const string CONDITION = "condition";
const string ACTION = "action";

int main() {


    std::map<string, set<string>> decisionTable =
    {
        {"stop", {"stop", "reset_values"}},
        {"start", {"start"}},
        {"exit", {"exit"}}
        // Add more rules here
    };


    ofstream errorsLog("../../errors_log.txt");

    string event;

    set<std::string> expectedActions = {{"", ""}};

    std::ifstream inputFile("../../logger.txt");

    std::string titleLine;
    std::getline(inputFile, titleLine);
    std::stringstream ss_title(titleLine);

    cout << "Validate " << titleLine <<  endl;
    errorsLog << "Validate " << titleLine <<  endl;

    std::string line;
    while (std::getline(inputFile, line))
    {
        std::stringstream ss(line);

        std::regex e("\\s+"); // regex for whitespace
        std::sregex_token_iterator it(line.begin(), line.end(), e, -1);

        if(*it == EVENT)
        {
            if(!expectedActions.empty() && *expectedActions.begin() != "")
            {
              cout << "event ->" << event << " missing actions" <<  endl;
              errorsLog << "event ->" << event << " missing actions" <<  endl;
            }

            expectedActions.clear();


            advance(it, 2); // skip "->"

            event = *it;
            auto eventIter = decisionTable.find(*it);

            if(eventIter != decisionTable.end())
            {
                expectedActions = eventIter->second;
            }
        }

        if(*it == ACTION)
        {
            advance(it, 2); // skip "->"

            auto actionIter = expectedActions.find(*it);

            if(actionIter != expectedActions.end())
            {
               //
                cout << "event ->" << event << " ; action -> " << *actionIter <<" valid " <<  endl;

               expectedActions.erase(actionIter);
            }
            else
            {
                cout << "event ->" << event << " ; action -> " << *it <<" not  valid " <<  endl;
                errorsLog << "event ->" << event << " ; action -> " << *it <<" not  valid " <<  endl;
            }
        }
    }

    cout << "End " << titleLine <<  endl;
    errorsLog << "End " << titleLine <<  endl;
    return 0;
}
