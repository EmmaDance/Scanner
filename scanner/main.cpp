#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <regex>


using namespace std;

void initCT(map<string, int> &ct) {
    ct.insert(pair<string, int>("identifier", 0));
    ct.insert(pair<string, int>("constant", 1));
    ct.insert(pair<string, int>("int", 2));
    ct.insert(pair<string, int>("char", 3));
    ct.insert(pair<string, int>("cin", 4));
    ct.insert(pair<string, int>("cout", 5));
    ct.insert(pair<string, int>("endl", 6));
    ct.insert(pair<string, int>("while", 7));
    ct.insert(pair<string, int>("if", 8));
    ct.insert(pair<string, int>("else", 9));
    ct.insert(pair<string, int>(":", 10));
    ct.insert(pair<string, int>(";", 11));
    ct.insert(pair<string, int>(",", 12));
    ct.insert(pair<string, int>("+", 13));
    ct.insert(pair<string, int>("*", 14));
    ct.insert(pair<string, int>("-", 15));
    ct.insert(pair<string, int>("/", 16));
    ct.insert(pair<string, int>("{", 17));
    ct.insert(pair<string, int>("}", 18));
    ct.insert(pair<string, int>("(", 19));
    ct.insert(pair<string, int>(")", 20));
    ct.insert(pair<string, int>("[", 21));
    ct.insert(pair<string, int>("]", 22));
    ct.insert(pair<string, int>("<", 23));
    ct.insert(pair<string, int>(">", 24));
    ct.insert(pair<string, int>("==", 25));
    ct.insert(pair<string, int>("=", 26));
    ct.insert(pair<string, int>("!=", 27));
    ct.insert(pair<string, int>(">>", 28));
    ct.insert(pair<string, int>("<<", 29));
}

void init_ST(map<int, string> &st, int st_len) {
    for (int i = 0; i < st_len; i++) {
        st.insert(pair<int, string>(i, ""));
    }
}

int st_insert(string token, map<int, string> &st, int& st_len);

int rehash(map<int, string> &st, int st_len) {
    cout << "REHASH" << endl;
    int new_len = st_len * 2;
    map<int, string> new_st;
    init_ST(new_st, new_len);
    for (int i = 0; i < st_len; i++) {
        string token = st.at(i);
        if (token != "") {
            st_insert(token, new_st, new_len);
        }
    }
    st = new_st;
    return new_len;
}

string &ltrim(string &str, const string &chars = "\t\n\v\f\r ") {
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

string &rtrim(string &str, const string &chars = "\t\n\v\f\r ") {
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

string &trim(string &str, const string &chars = "\t\n\v\f\r ") {
    return ltrim(rtrim(str, chars), chars);
}

bool isIdentifier(string const &token) {
    return (regex_match(token, regex("([A-Za-z][A-Za-z0-9]*)")) && token.length() < 250);
}

bool isConstant(string const &token) {
    return regex_match(token, regex(R"(0|(\+|-)?[1-9][0-9]*|'[0-9A-Za-z]')"));
}

vector<string> tokenize(string exp) {
    vector<string> tokens1;
    vector<string> tokens;
    // (\|\||<<|>>|==|!=|[();:,\[\]\{\+\=\}\/\*<>' ]|(?<!\= *)\-)
    regex rx(R"(\|\||<<|>>|==|!=|-|[();:,\[\]\{\+\=\}\/\*<> ])");
    sregex_token_iterator srti(exp.begin(), exp.end(), rx, {-1, 0});
    remove_copy_if(srti, std::sregex_token_iterator(),
                   std::back_inserter(tokens1),
                   [](std::string const &s) { return s.empty(); });

    bool afterEq = false;
    bool negative = false;
    for (auto &p : tokens1) {
        bool insert = true;
        p = trim(p);
        if (!p.empty()) {
            if (p == "=" || p == "<<") afterEq = true;
            if (p == "-" && afterEq) {
                negative = true;
                afterEq = false;
                insert = false;
            }
            if (isConstant(p) && negative) {
                p = "-"+p;
                negative = false;
            }
            else if (isIdentifier(p) && negative){
                tokens.push_back("-");
                negative = false;
            }
            if(insert){
                tokens.push_back(p);
                cout << "token: " << p << " " << endl;
            }
        }
    }

    return tokens;
}

int hash_token(const string& token, int st_len) {
    int h = 0;
    for (char j : token) {
        h += (int) j;
    }
    h = h % st_len;
    return h;
}

bool in_st(const string& token, map<int, string> &st, int st_len) {
    int pos = hash_token(token, st_len);
    while (st.count(pos) && st.at(pos) != token) {
        pos++;
    }
    if (pos < st_len)
        return true;
    return false;
}

int get_st_pos(const string& token, map<int, string> &st, int st_len) {
    int pos = hash_token(token, st_len);
    while (st.count(pos) && st.at(pos) != token) {
        pos++;
    }
    if (pos < st_len)
        return pos;
    return -1;
}

int find_first_free(const string& token, map<int, string> &st, int st_len){
    int init_pos = hash_token(token, st_len);
    int pos = init_pos;
    while (pos < st_len && !st.at(pos).empty()) {
        pos++;
    }
    if (pos >= st_len){
        pos = 0;
        while (pos < init_pos && !st.at(pos).empty()) {
            pos++;
        }
        if (pos >= init_pos)
            pos = -1;
    }
    return pos;
}

int st_insert(string token, map<int, string> &st, int& st_len) {
    // if the the token is already in the symbol table
    if (in_st(token, st, st_len))
        return -1;

    int pos = find_first_free(token, st, st_len);
    if (pos > -1){
    st.at(pos) = token;
    return pos;
    }
    st_len = rehash(st, st_len);
    return st_insert(token, st, st_len);
}

map<int,vector<string>> readFile(){
    map<int, vector<string>> tokens;
    ifstream file("src.txt");
    if (file.is_open()) {
        string line;
        int i = 1;
        while (getline(file, line)) {
            vector<string> line_tokens = tokenize(line);
            tokens.insert(pair<int, vector<string>>(i, line_tokens));
            i++;
        }
        file.close();
        return tokens;
    } else {
        throw "The source file couldn't be opened!";
    }
}

void print_st(map<int, string> st){
    for (auto const& x : st)
    {
        cout << x.first  // int key
                  << ':'
                  << x.second // string value
                  << endl ;
    }
}

void print_ct(map<string, int> ct){
    for (auto const& x : ct)
    {
        cout << x.first  // string (key)
             << ':'
             << x.second // string's value
             << endl ;
    }
}

void print_pif(vector<pair<int, int>> pif){
    for (auto const& x : pif)
    {
        cout << x.first  // string (key)
             << ':'
             << x.second // string's value
             << endl ;
    }
}

int get_token_code(string token, map<string, int> ct){
    int token_code;
    if (ct.count(token)) {
        token_code = ct.at(token);
    } else if (isIdentifier(token)) {
        token_code = ct.at("identifier");
    }
    else if (isConstant(token)) {
        token_code = ct.at("constant") ;
    } else {
        token_code = -1;
    }
    return token_code;
}

int main() {
    map<string, int> ct;
    vector<pair<int, int>> pif;
    map<int, string> id_st, const_st, temp;
    int id_st_len = 23;
    int const_st_len = 23;

    init_ST(id_st, id_st_len);
    init_ST(const_st, const_st_len);
    initCT(ct);

    try{
    map<int,vector<string>> tokens = readFile();
    int idx = 0;
        // build  STs
        for(auto const& lt : tokens){
            for (vector<string>::const_iterator i = lt.second.begin(); i != lt.second.end(); ++i) {
                // cout << *i << '~';
                int token_code = 0;
                string token = *i;
                if (ct.count(token)) {
                    // cout << token_code<< endl;
                } else if (isIdentifier(token)) {
                    st_insert(token, id_st, id_st_len);
                }
                else if (isConstant(token)) {
                    st_insert(token, const_st, const_st_len);
                } else {
                    cout << "Lexical error on line " << lt.first << ": Undefined token " << token << endl;
                }
                temp.insert(pair<int, string>(idx, token));
                idx++;
            }
        }

        // build pif
        for (auto const& x : temp)
        {
            idx = x.first;
            string token = x.second;
            int st_pos=-1;
            if (isIdentifier(token))
                st_pos = get_st_pos(token, id_st, id_st_len);
            else if (isConstant(token))
                st_pos = get_st_pos(token, const_st, const_st_len);

            int token_code = get_token_code(token, ct);
            pif.push_back(pair<int, int>(token_code, st_pos));
        }

        cout << "IDENTIFIER SYMBOL TABLE" << endl;
        print_st(id_st);
        cout << "CONSTANTS SYMBOL TABLE" << endl;
        print_st(const_st);
        cout << "PROGRAM INTERNAL FORM" << endl;
        print_pif(pif);
    }
    catch (exception& e) {
        cout << e.what();
    }

    return 0;
}