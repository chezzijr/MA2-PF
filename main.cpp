#include "knight2.h"
#define TC_PATH string("./tc/")

string trim(const string& s) {
    int i = 0, j = s.size() - 1;
    while (isspace(s[i])) i++;
    while (isspace(s[j])) j--;
    return s.substr(i, j - i + 1);
}

int main(int argc, char ** argv) {
    // string file_armyknights(TC_PATH), file_events(TC_PATH);
    // if (argc == 1) {
    //     file_armyknights += "tc40_armyknights"; // hard-code
    //     file_events += "tc40_events";
    // }
    // else if (argc == 3) {
    //     file_armyknights = argv[1];
    //     file_events = argv[2];
    // }
    // else {
    //     cout << "Error number of arguments" << endl;
    //     exit(1);
    // }

    int tcs[] = {1, 4, 40, 50, 54, 58, 68, 70, 73, 1003, 1004, 1011, 1018, 1028, 1040, 1058, 1059, 1079, 2049, 2059, 2060};
    for (int i = 0; i < sizeof(tcs) / sizeof(int); i++) {

        string file_armyknights(TC_PATH + "tc" + to_string(tcs[i]) + "_armyknights");
        string file_events(TC_PATH + "tc" + to_string(tcs[i]) + "_events");

        stringstream output, expected;
        auto* old = cout.rdbuf(output.rdbuf());

        // BEGIN
        KnightAdventure * knightAdventure = new KnightAdventure();
        knightAdventure->loadArmyKnights(file_armyknights);
        knightAdventure->loadEvents(file_events);
        knightAdventure->run();

        delete knightAdventure;

        cout.rdbuf(old);
        ifstream expectedFile(TC_PATH + "tc" + to_string(tcs[i]) + "_expected");
        expected << expectedFile.rdbuf();

        string o = trim(output.str());
        string e = trim(expected.str());

        if (o.compare(e) == 0) {
            clog << "[PASSED]" << " tc" << tcs[i] << endl;
        } else {
            clog << "[FAILED]" << " tc" << tcs[i] << endl;
            clog << "\tExpected result:\n";
            clog << e << '\n';
            clog << "\tOutput:\n";
            clog << o << '\n';
        }

    }
    

    return 0;
}