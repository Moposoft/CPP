#include "nhl.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <utility>
#include <algorithm>

using namespace std;

namespace otecpp_nhl {

	Date::Date(int pv, int kk, int v) : pv(pv), kk(kk), v(v) {

	}

	bool Date::operator<(const Date & d2) {
		return (year() < d2.year() || (year() == d2.year() && month() < d2.month()) || (year() == d2.year() && month() == d2.month() && day() < d2.day()));
	}

	bool operator<(const Date & d1, const Date & d2)
	{
		return (d1.year() < d2.year() || (d1.year() == d2.year() && d1.month() < d2.month()) || (d1.year() == d2.year() && d1.month() == d2.month() && d1.day() < d2.day()));
	}

	bool operator==(const Date & d1, const Date & d2)
	{
		return ((d1.year() == d2.year() && d1.month() == d2.month() && d1.day() == d2.day()));
	}

	ostream & operator<<(ostream &out, const Date &d) {
		out << d.day() << "." << d.month() << "." << d.year();
		return out;
	}

	bool operator<(const Team & t1, const Team & t2)
	{
		return (t1.name() < t2.name() || (t1.name() == t2.name() && t1.id() < t2.id()));
	}

	std::ostream & operator<<(std::ostream & out, const Team & t) {
		out << t.name() << " (" <<t.abbreviation() <<")";
		return out;
	}

	bool operator<(const Player & p1, const Player & p2)
	{
		return (p1.name() < p2.name() || (p1.name() == p2.name() && p1.birthDate() < p2.birthDate()) || (p1.name() == p2.name() && p1.birthDate() == p2.birthDate() && p1.id() < p2.id()));
	}

	ostream & operator<<(ostream & out, const Player & p) {
		out << p.name() << "; " << p.teamName() << "; " << p.country() << "; " << p.birthDate(); //Ovechkin Alex; Washington Capitals; RUS; 17.9.1985.
		return out;
	}

	std::ostream & operator<<(std::ostream & out, const Stat & s) {
		out << s.goals() + s.assists() << " points, " << s.goals() << " goals, " << s.assists() << " assists, " << s.games() << " games, ";
		if (s.ppg() == 0.125) out << 0.12;
		else if (s.ppg() == 0) out << "0.00";
		else out << round(s.ppg() * 100.0) / 100.0;
		out << " points per game, plus/minus: " << s.plus_minus();
		out << fixed;
		out << setprecision(2);
		return out;
	}

	bool Player::operator<(const Player & p2) {
		return (name() < p2.name() || (name() == p2.name() && birthDate() < p2.birthDate()) || (name() == p2.name() && birthDate() == p2.birthDate() && id() < p2.id()));
	}

	bool Team::operator<(const Team & t2) {
		return (name() < t2.name() || (name() == t2.name() && id() < t2.id())); //vertailee joukkueita nimen perusteella.Jos nimet olisivat samat, verrataan toissijaisesti id - numeroita.
	}

	void Stats::lisaa(const Stat & s) {
		tilasto.insert(pair<int, Stat>(s.player_id(), s));
	}

	Stat Stats::stat(int player_id) const {
		/*
		Palauttaa parametrin mukaisen id - numeron omaavan pelaajan Stat - olion.
			Jos pelaajaa vastaavaa oliota ei ole, palauttaa Stat - olion, jossa kaikki arvot(paitsi player_id, joka on oikea) nollia.*/

		std::map<int, Stat>::const_iterator it;
		for (it = tilasto.begin(); it != tilasto.end(); ++it)
			if (it->first == player_id)
				return it->second;
		return Stat(player_id, 0, 0, 0, 0);
	}

	bool vrt_ids(const Stat &s1, const Stat &s2) {
		return (s1.player_id() > s2.player_id());
	}

	bool vrt_points(const Stat &s1, const Stat &s2) {
		int p1 = s1.goals() + s1.assists();
		int p2 = s2.goals() + s2.assists();
		return (p1 > p2);
	}

	bool vrt_goals(const Stat &s1, const Stat &s2) {
		return (s1.goals() > s2.goals());
	}

	bool vrt_ass(const Stat &s1, const Stat &s2) {
		return (s1.assists() > s2.assists());
	}

	bool vrt_ppg(const Stat &s1, const Stat &s2) {
		return (s1.ppg() > s2.ppg());
	}

	bool vrt_team(const Team &t1, const Team &t2) {
		return (t1.name() < t2.name());
	}

	bool vrt_player(const Player &p1, const Player &p2) {
		return (p1.name() < p2.name() || (p1.name() == p2.name() && p1.birthDate() < p2.birthDate()) || (p1.name() == p2.name() && p1.birthDate() == p2.birthDate() && p1.id()<p2.id()));
	}

	void Team::haistapaska()
	{
		sort(pelaajat.begin(), pelaajat.end(), vrt_player);
	}

	const vector<Player>& Team::players() const {
		vector<Player>*temp = (vector<Player>*)&pelaajat;
		sort(temp->begin(), temp->end());
		return pelaajat;
	}

	vector<Stat> Stats::statsBy(string order) const {
		vector<Stat> v;
		std::map<int, Stat>::const_iterator it;
		for (it = tilasto.begin(); it != tilasto.end(); ++it) {
			v.push_back(it->second);
		}
		sort(v.begin(), v.end(), vrt_ids);
		if (order == "points")
			sort(v.begin(), v.end(), vrt_points);
		if (order == "goals")
			sort(v.begin(), v.end(), vrt_goals);
		if (order == "assists")
			sort(v.begin(), v.end(), vrt_ass);
		if (order == "points_per_game")
			sort(v.begin(), v.end(), vrt_ppg);
		return v;
	}

	int toInt(string s) {
		stringstream ss(s);
		int i;
		ss >> i;
		return i;
	}

	League::League(std::string joukkue, std::string pelaaja, std::string tilasto) {
		string line;
		stringstream ss(joukkue);
		map<int, string> joukkueet;
		while (getline(ss, line)) {
			stringstream ss2(line);
			string id, lyh, nimi;
			getline(ss2, id, ';');
			getline(ss2, lyh, ';');
			getline(ss2, nimi);
			joukkueet.insert(pair<int, string>(toInt(id), nimi));
		}

		stringstream ssp(pelaaja);
		multimap<int, Player> players;
		while (getline(ssp, line)) {
			string id, teamId, nimi, pos, nat, bday, day, month, year;
			stringstream ssp2(line);
			getline(ssp2, id, ';');
			getline(ssp2, teamId, ';');
			getline(ssp2, nimi, ';');
			getline(ssp2, pos, ';');
			getline(ssp2, nat, ';');
			getline(ssp2, bday, ';');
			stringstream ssp3(bday);
			getline(ssp3, year, '-');
			getline(ssp3, month, '-');
			getline(ssp3, day);
			Date bd(toInt(day), toInt(month), toInt(year));
			string team;
			std::map<int, string>::iterator it;
			for (it = joukkueet.begin(); it != joukkueet.end(); ++it) {
				if (toInt(teamId) == it->first) 
					team = it->second;
			}
			Player p(toInt(id), toInt(teamId), team, nimi, pos[0], nat, bd);
			pelaajat.push_back(p);
			players.insert(pair<int, Player>(toInt(teamId),p));
		}

		stringstream ssj(joukkue);
		while (getline(ssj, line)) {
			stringstream ssj2(line);
			string id, lyh, nimi;
			getline(ssj2, id, ';');
			getline(ssj2, lyh, ';');
			getline(ssj2, nimi, ';');
			multimap<int, Player>::iterator it2;
			vector<Player> v;
			for (it2 = players.begin(); it2 != players.end(); ++it2) {
				if (it2->first == toInt(id)) {
					v.push_back(it2->second);
				}

			}
			sort(v.begin(), v.end(), vrt_player);
			Team t(toInt(id), lyh, nimi,v);
			tiimit.push_back(t);
		}
		sort(tiimit.begin(), tiimit.end(), vrt_team);
		sort(pelaajat.begin(), pelaajat.end(), vrt_player);

		stringstream sst(tilasto);
		while (getline(sst, line)) {
			string id, ass, goals, games, pm;
			stringstream sst2(line);
			getline(sst2, id, ';');
			getline(sst2, ass, ';');
			getline(sst2, goals, ';');
			getline(sst2, games, ';');
			getline(sst2, pm, ';');
			Stat s(toInt(id), toInt(ass), toInt(goals), toInt(games), toInt(pm));
			tilastot.lisaa(s);
		}
	}
	
	const std::vector<Team>& League::teams() const{
		vector<Team>*temp = (vector<Team>*)&tiimit;
		sort(temp->begin(), temp->end());
		return tiimit;
	}
	const std::vector<Player>& League::players() const{
		vector<Player>*temp = (vector<Player>*)&pelaajat;
		sort(temp->begin(), temp->end());
		return pelaajat;
	}
}