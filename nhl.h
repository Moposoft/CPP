#ifndef NHL
#define NHL
#include <string>
#include <vector>
#include <iostream>
#include <map>

namespace otecpp_nhl {

	class Date {
		int pv;
		int kk;
		int v;
	public:
		Date(int pv, int kk, int v);
		int day() const { return pv; }
		int month() const { return kk; }
		int year() const { return v; }
		bool operator<(const Date &d2);
		friend bool operator<(const Date &d1, const Date &d2);
		friend bool operator==(const Date &d1, const Date &d2);
	};
	std::ostream & operator<<(std::ostream &out, const Date &d);

	class Player {
		int idnum;
		int teamid;
		std::string team;
		std::string nimi;
		char pelipaikka;
		std::string kotimaa;
		Date syntaika;
	public:
		Player(int id, int teamid, std::string team, std::string nimi, char p, std::string km, Date sa) : idnum(id), teamid(teamid), team(team), nimi(nimi), pelipaikka(p), kotimaa(km), syntaika(sa) {}
		int id() const { return idnum; }
		const std::string & teamName() const { return team; }
		int team_id() const { return teamid; }
		const std::string & name() const { return nimi; }
		char position() const { return pelipaikka; }
		const std::string & country() const { return kotimaa; }
		const Date & birthDate() const { return syntaika; }
		bool operator<(const Player &p2);
		friend bool operator<(const Player &p1, const Player &p2);
	};
	std::ostream & operator<<(std::ostream &out, const Player &p);

	class Stat {
		int pelaaja_id;
		int syotot;
		int maalit;
		int pelatut;
		int plusmiinus;
		double ppp;
	public:
		Stat(int pid, int s, int m, int p, int pm) : pelaaja_id(pid), syotot(s), maalit(m), pelatut(p), plusmiinus(pm) {
			if (p != 0) ppp = 1.0*(s + m) / p;
			else ppp = 0.00;
		}
		double ppg() const { return ppp; }
		int player_id() const { return pelaaja_id; }
		int assists() const { return syotot; }
		int goals() const { return maalit; }
		int games() const { return pelatut; }
		int plus_minus() const { return plusmiinus; }
	};
	std::ostream & operator<<(std::ostream &out, const Stat &s);

	class Stats {
		std::map<int, Stat> tilasto;
	public:
		void lisaa(const Stat &s);
		Stat stat(int player_id) const;
		std::vector<Stat> statsBy(std::string order = "points") const;
	};

	class Team {
		int joukkue_id;
		std::string lyh;
		std::string nimi;
		std::vector<Player> pelaajat;
	public:
		Team(int id, std::string lyh, std::string nim, std::vector<Player> v) : joukkue_id(id), lyh(lyh), nimi(nim){
			for (unsigned int i = 0; i < v.size(); ++i) {
				pelaajat.push_back(v[i]);
			}
		}
		void haistapaska();
		int id() const { return joukkue_id; }
		const std::string & abbreviation() const { return lyh; }
		const std::string & name() const { return nimi; }
		const std::vector<Player> & players() const;
		bool operator<(const Team &t2);
		friend bool operator<(const Team &t1, const Team &t2);
	};
	std::ostream & operator<<(std::ostream &out, const Team &t);

	class League {
		std::vector<Team> tiimit;
		std::vector<Player> pelaajat;
		Stats tilastot;
	public:
		League(std::string joukkue, std::string pelaaja, std::string tilasto);
		const std::vector<Team> & teams()const;
		const std::vector<Player> & players() const;
		const Stats & stats() const { return tilastot; }
	};
}

#endif
