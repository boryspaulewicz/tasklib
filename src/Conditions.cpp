// -*- coding: utf-8 -*-

#include "Conditions.hpp"

void Conditions::fill_conditions() {
    // Liczba powtórzeń poziomów czynnika f = iloczyn liczb poziomów
    // poprzednich czynników. Liczba kolejnych serii powtórzeń poziomów
    // (pierwsza seria: a, a, a, druga: b, b, b, ...) = liczba warunków
    // / (liczba poziomów * liczba powtórzeń)
    int rep = 1;
    for (auto& f : names) {
        for (int i = 0; i < nof_cnds / (factors[f].size() * rep); i++)
            for (auto& l : factors[f])
                for (int j = 0; j < rep; j++)
                    conditions[f].push_back(l);
        rep *= factors[f].size();
    }
}

Conditions::Conditions(vector<pair<string, vector<PType> > > design) {
    nof_cnds = 1;
    for (auto& d : design) {
        nof_cnds *= d.second.size();
        names.push_back(d.first);
        factors[d.first] = d.second;
    }
    fill_conditions();
}

void Conditions::print() {
    string msg = "Czynniki:\n";
    for (auto& f : factors) {
        msg += f.first + ":";
        for (auto& l : f.second)
            msg += " " + l;
        msg += "\n";
    }
    log(msg);
    msg = "Warunki zadania:\n";
    for (int c = 0; c < nof_cnds; c++) {
        for (auto& f : names)
            msg += (string) get(f, c) + " ";
        msg += "\n";
    }
    log(msg);
}
