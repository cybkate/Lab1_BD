#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

using namespace std;

vector<vector<int>> MID;

string find_n(string line, int n) {
    int pos = 0;
    string s;
    s = line;
    for (int i = 1; i < n; i++) {
        pos = s.find(";");
        s.erase(0, pos + 1);
    }
    pos = s.find(";");
    return s.substr(0, pos);
}

fstream& GotoLine(fstream& file, unsigned int num) {
    file.seekg(ios::beg);
    for (int i = 0; i < num - 1; ++i) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return file;
}

void get_ind() {
    fstream file("R.ind");
    string line;
    while (!file.eof()) {
        file >> line;
        if (line != "") {
            vector<int> newRow = { stoi(find_n(line,1)), stoi(find_n(line,2)) };
            MID.push_back(newRow);
        }
    }
}

void set_ind() {
    ofstream file("R.ind");
    int i = 0;
    for (const auto& row : MID) {
        if (i == 0) {
            file << row[0] << ";" << row[1];
        }
        else {
            file << "\n" << row[0] << ";" << row[1];
        }
        i++;
    }
}

void editLine(const string& fileName, int lineNum, const string& newLine) {
    fstream file(fileName, ios::in | ios::out);
    string line;

    GotoLine(file, lineNum);
    getline(file, line);
    file.close();
    file.open(fileName);
    GotoLine(file, lineNum);

    streampos pos = file.tellg();

    file.seekp(pos);
    if (line.length() == newLine.length()) {
        file << newLine;
    }
    else if (line.length() > newLine.length()) {
        file << newLine;
        for (int i = 0; i < line.length() - newLine.length(); i++) {
            file << " ";
        }
    }
    else {
        file << newLine << "\n";
    }
    file.close();
}

void get_m(int ind) {
    fstream file("R.ind");
    file.seekg(ios::beg);
    string line;
    int pos;
    pos = -1;
    while (!file.eof()) {
        file >> line;
        if (line.length() > 0) {
            pos = line.find(";");
            if (stoi(find_n(line, 1)) == ind) {
                pos = stoi(find_n(line, 2));
                break;
            }
        }
    }
    file.close();
    if (pos != -1) {
        fstream file("R.fl");
        GotoLine(file, pos);
        getline(file, line);
        if (line.length() > 0) {
            cout << line << endl;
        }
    }
    else {
        cout << "Not found..." << endl;
    }
}

void get_s(int crdNum, int bid) {
    fstream file("L.fl");
    file.seekg(ios::beg);
    string line;
    int pos;
    pos = -1;
    while (!file.eof()) {
        file >> line;
        if (line.length() > 0) {
            if (stoi(find_n(line, 1)) == 1) {
                if (stoi(find_n(line, 1)) == crdNum && stoi(find_n(line, 2)) == bid) {
                    file.close();
                    cout << line << endl;
                }
            }
        }
    }
    cout << "Not found..." << endl;
}

void del_s(int crdNum, int bid) {
    fstream file("L.fl");
    file.seekg(ios::beg);
    string line;
    int pos;
    pos = 1;
    while (!file.eof()) {
        file >> line;
        if (line.length() > 0) {
            if (stoi(find_n(line, 1)) == 1) {
                if (stoi(find_n(line, 2)) == crdNum && stoi(find_n(line, 3)) == bid) {
                    file.close();
                    file.open("L.fl");
                    GotoLine(file, pos);
                    file.seekp(file.tellg() + static_cast<streamoff>(1));
                    file.seekp(-1, ios::cur);
                    file.put('0');
                    file.close();
                    return;
                }
            }
        }
        pos++;
    }
    cout << "Not found..." << endl;
}

void del_m(int crdNum) {
    for (int i = 0; i < MID.size(); i++) {
        if (MID[i][0] == crdNum) {
            fstream file("R.fl");
            GotoLine(file, MID[i][1]);
            file.seekp(file.tellg() + static_cast<streamoff>(1));
            file.seekp(-1, ios::cur);
            file.put('0');
            GotoLine(file, MID[i][1]);
            string line;
            int pos;
            file >> line;
            pos = stoi(line.substr(line.length() - 1, line.length()));
            MID.erase(MID.begin() + i);
            while (line[line.length() - 1] != '0') {
                file.open("L.fl");
                pos = stoi(line.substr(line.length() - 1, line.length()));
                GotoLine(file, pos);
                file >> line;
                GotoLine(file, pos);
                file.seekp(file.tellg() + static_cast<streamoff>(1));
                file.seekp(-1, ios::cur);
                file.put('0');
                file.close();
            }
            break;
        }
    }
    set_ind();
}

void update_m(int crdNum, int field, string info) {
    fstream file("R.ind");
    file.seekg(ios::beg);
    string line;
    int pos;
    pos = -1;
    while (!file.eof()) {
        file >> line;
        pos = line.find(";");

        if (stoi(find_n(line, 1)) == crdNum) {
            pos = stoi(find_n(line, 2));
            break;
        }
    }
    file.close();
    file.open("R.fl");
    GotoLine(file, pos);
    getline(file, line);
    int p = 0;
    string token;
    int i = 1;
    while ((p = line.find(";")) != string::npos) {
        if (i == field) {
            token += info;
        }
        else {
            token += line.substr(0, p);
        }
        token += ";";
        line.erase(0, p + 1);
        i++;
    }
    token += line;
    file.close();
    editLine("R.fl", pos, token);
}

void update_s(int crdNum, int bid, int field, string info) {
    fstream file("L.fl");
    string line;
    int j = 1;
    while (!file.eof()) {
        file >> line;
        string c = line;
        c.erase(0, 2);
        int a = stoi(c.substr(0, c.find(";")));
        c.erase(0, c.find(";") + 1);
        int b = stoi(c.substr(0, c.find(";")));
        if (a == crdNum && b == bid) {
            break;
        }
        j++;
    }
    int p = 0;
    string token;
    int i = 1;
    while ((p = line.find(";")) != string::npos) {
        if (i == field) {
            token += info;
        }
        else {
            token += line.substr(0, p);
        }
        token += ";";
        line.erase(0, p + 1);
        i++;
    }

    token += line;
    file.close();
    editLine("L.fl", j, token);
}

void insert_m(int id, string name, string num) {
    fstream file("R.fl");
    int i = 0;
    string line;
    while (!file.eof()) {
        getline(file, line);
        i++;
    }
    if (line == "") i = 0;
    line = "1;" + name + ";" + num + ";0";
    file.close();
    ofstream file2;
    file2.open("R.fl", std::ios::app);
    if (i == 0) {
        file2 << line;
    }
    else {
        file2 << "\n" << line;
    }
    file2.close();
    vector<int>newRow = { id, i + 1 };
    MID.push_back(newRow);
    set_ind();
}

void insert_s(int id, int bid, string date) {
    fstream file("L.fl");
    int i = 0;
    string line;
    while (!file.eof()) {
        getline(file, line);
        i++;
    }
    if (line == "") i = 0;
    line = "1;" + to_string(id) + ";" + to_string(bid) + ";" + date + ";0";
    file.close();
    ofstream file2;
    file2.open("L.fl", std::ios::app);
    if (i == 0) {
        file2 << line;
    }
    else {
        file2 << "\n" << line;
    }
    file2.close();

    file.open("R.fl");
    for (const auto& row : MID) {
        if (row[0] == id) {
            GotoLine(file, row[1]);
            getline(file, line);
            file.close();
            if (line[line.length() - 1] == '0') {
                file.open("R.fl");
                GotoLine(file, row[1]);
                file.seekp(file.tellg() + static_cast<streamoff>(line.length()));
                file.seekp(-1, ios::cur);
                file.put(to_string(i + 1)[0]);
                file.close();
            }
            else {
                int pos = -1;
                file.close();

                while (line[line.length() - 1] != '0') {
                    file.open("L.fl");
                    pos = stoi(line.substr(line.length() - 1, line.length()));
                    GotoLine(file, pos);
                    file >> line;
                    file.close();
                }
                if (pos != -1) {
                    file.open("L.fl");
                    GotoLine(file, pos);
                    file.seekp(file.tellg() + static_cast<streamoff>(line.length()));
                    file.seekp(-1, ios::cur);
                    file.put(to_string(i + 1)[0]);
                    file.close();
                }
            }
        }
    }
}

void ut_m() {
    fstream file("R.fl");
    string line;
    cout << endl << "   ID     Name     Email" << endl;
    for (const auto& row : MID) {
        GotoLine(file, row[1]);
        getline(file, line);
        cout << "   " << row[0];
        for (int i = 1; i <= 4; i++) {
            int pos = line.find(";");
            if (i == 2 || i == 3) {
                cout << "   " << line.substr(0, pos);
            }
            line.erase(0, pos + 1);
        }
        cout << endl;
    }
    cout << endl;
}

void ut_s() {
    fstream file("L.fl");
    string line;
    cout << endl << "   R_ID   B_ID  DrawingStart_Date" << endl;
    while (!file.eof()) {
        file >> line;
        if (line[0] == '1') {
            for (int i = 1; i <= 5; i++) {
                int pos = line.find(";");
                if (i == 2 || i == 3 || i == 4) {
                    cout << "   " << line.substr(0, pos);
                }
                line.erase(0, pos + 1);
            }
            cout << endl;
        }
    }
    cout << endl;
}

void calc_m() {
    int i = 0;
    for (const auto& row : MID) {
        i++;
    }
    cout << " " << i << endl;
}

void calc_s() {
    fstream file("L.fl");
    string line;
    int i = 0;
    while (!file.eof()) {
        file >> line;
        if (line[0] == '1') {
            i++;
        }
    }
    cout << " " << i << endl;
}

int calc_id() {
    int i = 0;
    for (const auto& row : MID) {

        if (row[0] >= i) {
            i = row[0] + 1;
        }
    }
    if (i == 0) {
        i = 1001;
    }
    return i;
}

int main()
{
    get_ind();
    int ID_count = calc_id();
    string command;
    while (true) {
        cout << "Input command> ";
        cin >> command;

        if (command == "ut_m") {
            ut_m();
        }

        else if (command == "ut_s") {
            ut_s();
        }

        else if (command == "del_m") {
            int id;
            cout << "Input id> ";
            try {
                cin >> id;
            }
            catch (...) {
                cout << "Id must be int number" << endl;
            }
            del_m(id);
        }

        else if (command == "del_s") {
            int id1, id2;
            string s;
            try {
                cout << "Input mangaka id> ";
                cin >> id1;
                cout << "Input manga id> ";
                cin >> id2;
            }
            catch (...) {
                cout << "Id must be int number" << endl;
            }
            del_s(id1, id2);
        }

        else if (command == "insert_m") {
            string name, num;
            cout << "Input mangaka name> ";
            cin >> name;
            cout << "Input mangaka email address> ";
            cin >> num;
            insert_m(ID_count, name, num);
            ID_count++;
        }
        else if (command == "insert_s") {
            int id, bid;
            string date;
            cout << "Input mangaka id> ";
            cin >> id;
            cout << "Input manga id> ";
            cin >> bid;
            cout << "Input drawing start date> ";
            cin >> date;
            insert_s(id, bid, date);
        }
        else if (command == "get_m") {
            int id;
            cout << "Input mangaka id> ";
            cin >> id;
            get_m(id);
        }
        else if (command == "get_s") {
            int id1, id2;
            cout << "Input mangaka id> ";
            cin >> id1;
            cout << "Input manga id> ";
            cin >> id2;
            get_s(id1, id2);
        }
        else if (command == "update_m") {
            int id, field;
            string info;
            cout << "Input mangaka id> ";
            cin >> id;
            cout << "Input property num> ";
            cin >> field;
            cout << "Input new info> ";
            cin >> info;
            update_m(id, field, info);
        }
        else if (command == "update_s") {
            int id1, id2;
            string info;
            cout << "Input mangaka id> ";
            cin >> id1;
            cout << "Input manga id> ";
            cin >> id2;
            cout << "Input new drawing start date> ";
            cin >> info;
            update_s(id1, id2, 4, info);
        }
        else if (command == "calc_m") {
            calc_m();
        }
        else if (command == "calc_s") {
            calc_s();
        }
    }

    return 0;
}