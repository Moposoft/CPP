#ifndef OTTELUTILASTO_H
#define	OTTELUTILASTO_H

class joukkuetiedot {
    std::string nimi;
    int ottelut;
    int voitot;
    int haviot;
    int ja_haviot;
    int pisteet;
    int saadut;
    int annetut;
public:
    const std::string getNimi() const;
    const int getPisteet() const;
    const int getOttelut() const;
    const int getVoitot() const;
    const int getHaviot() const;
    const int getJaHaviot() const;
    const int getAnnetut() const;
    const int getSaadut() const;
    joukkuetiedot(const std::string &nimi, int saadut, int annetut, bool &ja);
    friend std::ostream& operator<<(std::ostream &out, const joukkuetiedot &jt);
    joukkuetiedot& operator+=(const joukkuetiedot &toinen);
};

struct ottelutiedot{
    std::string ottelu;
    std::string joukkue1;
    std::string joukkue2;
    ottelutiedot(std::string ot, std::string j1, std::string j2) : ottelu(ot), joukkue1(j1), joukkue2(j2){} 
    friend std::ostream& operator<<(std::ostream &out, const ottelutiedot &ot);
};

typedef std::vector<joukkuetiedot> sarjataulu;
typedef std::vector<ottelutiedot> ottelutaulu;
void lue(const std::string &tiedosto, sarjataulu &st, ottelutaulu &ot);
void jarjesta(sarjataulu &st);
void ottelut(ottelutaulu &ot, const std::string &nimi= std::string());
void taulukko(const std::string &param, sarjataulu &st, bool t2);
void taulukko2(const sarjataulu &st);
bool vrt_ot(const ottelutiedot &ot1, const ottelutiedot &ot2);
bool vrt_pisteet(const joukkuetiedot &jt1, const joukkuetiedot &jt2);
bool vrt_nimi(const joukkuetiedot &jt1, const joukkuetiedot &jt2);
bool vrt_ottelut(const joukkuetiedot &jt1, const joukkuetiedot &jt2);
bool vrt_voitot(const joukkuetiedot &jt1, const joukkuetiedot &jt2);
bool vrt_haviot(const joukkuetiedot &jt1, const joukkuetiedot &jt2);
bool vrt_jahaviot(const joukkuetiedot &jt1, const joukkuetiedot &jt2);
bool vrt_saadut(const joukkuetiedot &jt1, const joukkuetiedot &jt2);
bool vrt_annetut(const joukkuetiedot &jt1, const joukkuetiedot &jt2);

#endif