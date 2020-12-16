#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "ottelutilasto.h"

using namespace std;

/* Funktio lukee viiteparametrinä saamasa tiedoston nimen mukaisen tiedoston ja talleentaa
 * tiedot viiteparametreinä saatuihin taulukoihin */
void lue(const string &tiedosto, sarjataulu &st, ottelutaulu &ot) {
    vector<string> syotteet;
    stringstream ss;
    string rivi, osa, pvm, joukkue1, joukkue2;
    bool jatkoaika;
    int maalit1, maalit2;
    ifstream t(tiedosto.c_str());
    //Yritetään lukea tiedostoa.
    if (t.good()) {
        //luoteaan rivi.
        while (getline(t, rivi)) {
            istringstream iss(rivi);
            //jaetaan pilkun eroittamiin osiin.
            while (getline(iss, osa, ',')) {
                syotteet.push_back(osa);
            }
            //Tallentaan osat nimettyihin muuttujiin.
            pvm = syotteet[0];
            joukkue1 = syotteet[1];
            maalit1 = atoi(syotteet[2].c_str());
            joukkue2 = syotteet[3];
            maalit2 = atoi(syotteet[4].c_str());
            //Jos rivillä on jatkoaika tieto.
            if (syotteet.size() == 6) {
                jatkoaika = true;
                ss << pvm << " " << joukkue1 << " vs. " << joukkue2 << " " << maalit1 << "-" << maalit2 << " " << syotteet[5];
            } else {
                jatkoaika = false;
                ss << pvm << " " << joukkue1 << " vs. " << joukkue2 << " " << maalit1 << "-" << maalit2;
            }
            ot.push_back(ottelutiedot(ss.str(), joukkue1, joukkue2));//ottelutauluun tallentaan tarvittavat ottelun tiedot.
            st.push_back(joukkuetiedot(joukkue1, maalit1, maalit2, jatkoaika));//sarjatauluun tallenetaan joukkueiden tiedot
            st.push_back(joukkuetiedot(joukkue2, maalit2, maalit1, jatkoaika));//joukkuetiedot rakennin hoitaa pisteiden laskun. 
            ss.str("");
            syotteet.clear();
        }
    } else {
        //Tiedostoa ei löytynyt.
        t.close();
    }
}

/*Järjestetän sarjataulu niin, että summataan yhteen samannimisten joukkueiden tiedot keskenään*/
void jarjesta(sarjataulu &st) {
    //järjesetään nimen mukaan.
    sort(st.begin(), st.end(), vrt_nimi);
    for (size_t i = 0; i < st.size(); i++) {
        while (i < st.size() - 1 && st[i].getNimi() == st[i + 1].getNimi()) {
            st[i + 1] += st[i]; //listään tiedot keskenään, käytetään kuorimtettua operaattoria.
            st.erase(st.begin() + i); //poistetaan vanha tieto.
        }
    }
}

/*Järjestää ja tulostaa ottelutiedot. Jos parametrin nimi anettu tulostaan sen mukaan */
void ottelut(ottelutaulu &ot, const string &nimi) {
    //Jos tyhjä taulukko, tulosteaan tyhjä rivi ja lopetetaan.
    if (ot.size() == 0) {
        cout << endl;
        return;
    }
    //Järjestetaan ottelutiedot aakkosjärjestyksessä.
    sort(ot.begin(), ot.end(), vrt_ot);
    //Ilman parametriä tulostus.
    if (nimi.size() == 0) {
        for (size_t i = 0; i < ot.size(); i++)
            cout << ot[i] << endl;
    }//Muutoin haetaan parametriä vastaava nimi, ja tulosteaan sen otteluntiedot. 
    else {
        for (size_t i = 0; i < ot.size(); i++) {
            if (ot[i].joukkue1 == nimi || ot[i].joukkue2 == nimi)
                cout << ot[i] << endl;
        }
    }
    cout << endl; //Lopuksi tyhjä rivi.
}

/*Järjestää taulukon parametrin mukaisesti ja tulostaa ruudulle taulukon tiedot*/
void taulukko(const string &param, sarjataulu &st, bool t2) {
    //Jos tyhjä taulukko, tulosteaan tyhjä rivi ja lopetetaan.
    if (st.size() == 0) {
        cout << endl;
        return;
    }
    //Mitä vertailufunktiota käytetään.
    if (param.size() == 0 || param == "pisteet")
        sort(st.begin(), st.end(), vrt_pisteet);
    else if (param == "nimi")
        sort(st.begin(), st.end(), vrt_nimi);
    else if (param == "ottelut")
        sort(st.begin(), st.end(), vrt_ottelut);
    else if (param == "nimi")
        sort(st.begin(), st.end(), vrt_nimi);
    else if (param == "voitot")
        sort(st.begin(), st.end(), vrt_voitot);
    else if (param == "häviöt")
        sort(st.begin(), st.end(), vrt_haviot);
    else if (param == "jatkoaikahäviöt")
        sort(st.begin(), st.end(), vrt_jahaviot);
    else if (param == "saadut")
        sort(st.begin(), st.end(), vrt_saadut);
    else if (param == "annetut")
        sort(st.begin(), st.end(), vrt_annetut);
    else {
        // Tuntematon komento.
        return;
    }
    if (t2) { //talukko2 tulostus.
        taulukko2(st);
    } else {//tavallinen tulostos.
        for (size_t i = 0; i < st.size(); i++)
            cout << st[i] << endl;
    }
    cout << endl; //lopuksi tyhjä rivi.
}

/*Taulukko2 tulostaa talukon siististi tasoitettuna.*/
void taulukko2(const sarjataulu &st) {
    size_t nimi_maxkoko = 0;
    int ottelut_max = 0;
    int voitot_max = 0;
    int haviot_max = 0;
    int ja_haviot_max = 0;
    int pisteet_max = 0;
    int saadut_max = 0;
    int annetut_max = 0;
    //Etsitään sarjataulun suurimmat koot/arvot ja tallennetaan ne.
    for (size_t i = 0; i < st.size(); i++) {
        if (st[i].getNimi().size() > nimi_maxkoko)
            nimi_maxkoko = st[i].getNimi().size();
        if (st[i].getOttelut() > ottelut_max)
            ottelut_max = st[i].getOttelut();
        if (st[i].getVoitot() > voitot_max)
            voitot_max = st[i].getVoitot();
        if (st[i].getHaviot() > haviot_max)
            haviot_max = st[i].getHaviot();
        if (st[i].getJaHaviot() > ja_haviot_max)
            ja_haviot_max = st[i].getJaHaviot();
        if (st[i].getPisteet() > pisteet_max)
            pisteet_max = st[i].getPisteet();
        if (st[i].getSaadut() > saadut_max)
            saadut_max = st[i].getSaadut();
        if (st[i].getAnnetut() > annetut_max)
            annetut_max = st[i].getAnnetut();
    }
    //Käy taulukon läpi ja tulostaa joka arvon kohdalla tarvittavan määrän tyhjiä välilyöntejä.
    for (size_t i = 0; i < st.size(); i++) {
        cout << st[i].getNimi() << string(nimi_maxkoko - st[i].getNimi().size() + 1, ' ');
        if (ottelut_max >= 10 && st[i].getOttelut() < 10) cout << " ";
        cout << st[i].getOttelut() << " ";
        if (voitot_max >= 10 && st[i].getVoitot() < 10) cout << " ";
        cout << st[i].getVoitot() << " ";
        if (haviot_max >= 10 && st[i].getHaviot() < 10) cout << " ";
        cout << st[i].getHaviot() << " ";
        if (ja_haviot_max >= 10 && st[i].getJaHaviot() < 10) cout << " ";
        cout << st[i].getJaHaviot() << " ";
        if (pisteet_max >= 10 && st[i].getPisteet() < 10) cout << " ";
        cout << st[i].getPisteet() << " ";
        if (saadut_max >= 10 && st[i].getSaadut() < 10) cout << " ";
        cout << st[i].getSaadut() << " ";
        if (annetut_max >= 10 && st[i].getAnnetut() < 10) cout << " ";
        cout << st[i].getAnnetut() << endl;
    }
}

/*Vertailu funktiot*/
bool vrt_ot(const ottelutiedot &ot1, const ottelutiedot &ot2) {
    return (ot1.ottelu < ot2.ottelu);
}

bool vrt_nimi(const joukkuetiedot &jt1, const joukkuetiedot &jt2) {
    return (jt1.getNimi() < jt2.getNimi());
}

bool vrt_voitot(const joukkuetiedot &jt1, const joukkuetiedot &jt2) {
    if (jt1.getVoitot() > jt2.getVoitot())
        return true;
    if (jt1.getVoitot() == jt2.getVoitot())
        return vrt_nimi(jt1, jt2);
    return false;
}

bool vrt_haviot(const joukkuetiedot &jt1, const joukkuetiedot &jt2) {
    if (jt1.getHaviot() > jt2.getHaviot())
        return true;
    if (jt1.getHaviot() == jt2.getHaviot())
        return vrt_nimi(jt1, jt2);
    return false;
}

bool vrt_jahaviot(const joukkuetiedot &jt1, const joukkuetiedot &jt2) {
    if (jt1.getJaHaviot() > jt2.getJaHaviot())
        return true;
    if (jt1.getJaHaviot() == jt2.getJaHaviot())
        return vrt_nimi(jt1, jt2);
    return false;
}

bool vrt_saadut(const joukkuetiedot &jt1, const joukkuetiedot &jt2) {
    if (jt1.getSaadut() > jt2.getSaadut())
        return true;
    if (jt1.getSaadut() == jt2.getSaadut())
        return vrt_nimi(jt1, jt2);
    return false;
}

bool vrt_annetut(const joukkuetiedot &jt1, const joukkuetiedot &jt2) {
    if (jt1.getAnnetut() > jt2.getAnnetut())
        return true;
    if (jt1.getAnnetut() == jt2.getAnnetut())
        return vrt_nimi(jt1, jt2);
    return false;
}

bool vrt_ottelut(const joukkuetiedot &jt1, const joukkuetiedot &jt2) {
    if (jt1.getOttelut() > jt2.getOttelut())
        return true;
    if (jt1.getOttelut() == jt2.getOttelut())
        return vrt_nimi(jt1, jt2);
    return false;
}

bool vrt_pisteet(const joukkuetiedot &jt1, const joukkuetiedot &jt2) {
    int j1_maaliero = jt1.getSaadut() - jt1.getAnnetut();
    int j2_maaliero = jt2.getSaadut() - jt2.getAnnetut();
    return (jt1.getPisteet() > jt2.getPisteet()) ||
            (jt1.getPisteet() == jt2.getPisteet() && jt1.getOttelut() < jt2.getOttelut()) ||
            (jt1.getPisteet() == jt2.getPisteet() && jt1.getOttelut() == jt2.getOttelut() && j1_maaliero > j2_maaliero) ||
            (jt1.getPisteet() == jt2.getPisteet() && jt1.getOttelut() == jt2.getOttelut() && j1_maaliero == j2_maaliero && jt1.getSaadut() > jt2.getSaadut());
}

/*Rakennin laskee parameterien avualla tarvittavat tiedot ja alustaa joukkuetiedot luokan muuttujat*/
joukkuetiedot::joukkuetiedot(const string &nimi, int saadut, int annetut, bool &ja) {
    this->nimi = nimi;
    this->saadut = saadut;
    this->annetut = annetut;
    ottelut = 1;
    if (saadut > annetut) {
        voitot = 1;
        haviot = 0;
        ja_haviot = 0;
        pisteet = 2;
    } else {
        voitot = 0;
        ja ? haviot = 0 : haviot = 1;
        ja ? ja_haviot = 1 : ja_haviot = 0;
        ja ? pisteet = 1 : pisteet = 0;
    }
}

joukkuetiedot& joukkuetiedot::operator +=(const joukkuetiedot& toinen) {
    this->ottelut += toinen.ottelut;
    this->voitot += toinen.voitot;
    this->haviot += toinen.haviot;
    this->ja_haviot += toinen.ja_haviot;
    this->pisteet += toinen.pisteet;
    this->saadut += toinen.saadut;
    this->annetut += toinen.annetut;
    return *this;
}

/*Aksessorit*/
const string joukkuetiedot::getNimi() const {
    return nimi;
}

const int joukkuetiedot::getPisteet() const {
    return pisteet;
}

const int joukkuetiedot::getOttelut() const {
    return ottelut;
}

const int joukkuetiedot::getVoitot() const {
    return voitot;
}

const int joukkuetiedot::getHaviot() const {
    return haviot;
}

const int joukkuetiedot::getJaHaviot() const {
    return ja_haviot;
}

const int joukkuetiedot::getAnnetut() const {
    return annetut;
}

const int joukkuetiedot::getSaadut() const {
    return saadut;
}

ostream& operator <<(ostream& out, const joukkuetiedot& jt) {
    out << jt.nimi << " " << jt.ottelut << " " << jt.voitot << " " << jt.haviot << " " << jt.ja_haviot << " " << jt.pisteet << " " << jt.saadut << " " << jt.annetut;
    return out;
}

ostream& operator<<(std::ostream &out, const ottelutiedot &ot) {
    out << ot.ottelu;
    return out;
}

int main() {
    sarjataulu st; //Joukkueiden tiedot.
    ottelutaulu ot; //Otteluiden tiedot.
    string komento, osa1, osa2;
    /*Luetaan käyttäjän syötteet ja toimitaan niiden mukaisesti*/
    while (getline(cin, komento)) {
        stringstream ss(komento);
        getline(ss, osa1, ' '); //Jaetaan komento kahteen osaan välimerkin jälkeen.
        getline(ss, osa2);
        if (komento == "lopeta") {
            break;
        } else if (osa1 == "lue") {
            lue(osa2, st, ot);
            jarjesta(st);
        } else if (osa1 == "taulukko") {
            taulukko(osa2, st, false);
        } else if (osa1 == "taulukko2") {
            taulukko(osa2, st, true);
        } else if (osa1 == "ottelut") {
            ottelut(ot, osa2);
        } else if (komento == "ottelut") {
            ottelut(ot);
        } else if (komento == "resetoi") {
            st.clear();
            ot.clear();
        }
        //tyhjenettän tallenetut syöteet.
        osa1.clear();
        osa2.clear();
    }
}