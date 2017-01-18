#include "Utils.hpp"

string string_from_file(string fname) {
    ifstream f;
    f.open(fname);
    if (!f.good()) {
        throw (runtime_error("Nie udało się otworzyć pliku " + fname));
        exit(1);
    }
    stringstream contents;
    contents << f.rdbuf();
    return contents.str();
}

locale loc;
string tolower(string s){
    string res = s;
    for(int i = 0; i < s.size(); i++)
        res[i] = tolower(s[i], loc);
    return res;
}
string toupper(string s){
    string res = s;
    for(int i = 0; i < s.size(); i++)
        res[i] = toupper(s[i], loc);
    return res;
}

string trim(string str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool rmatch(string pattern, string str) {
    smatch m;
    regex re(pattern);
    return regex_match(str, m, re);
}

int random_int(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(min, max);
    return dis(gen);
}
