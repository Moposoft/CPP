#include <algorithm>
#include <iomanip>
#include <map>
#include <stdio.h>
#include <vector>
#include "nhl.h"

using namespace std;
using otecpp_nhl::Date;
using otecpp_nhl::League;
using otecpp_nhl::Player;
using otecpp_nhl::Stat;
using otecpp_nhl::Stats;
using otecpp_nhl::Team;

int main(int argc, char *argv[]) {
	
	string players_csv = "8469608;22;Smith, Mike;G;CAN;1982-03-22\n8478427;12;Aho, Sebastian;C;FIN;1997-07-26\n8479976;20;Valimaki, Juuso;D;FIN;1998-10-06\n8480222;2;Aho, Sebastian;D;SWE;1996-02-17\n8481655;13;Schwindt, Cole;C;CAN;2001-04-25\n8478403;7;Eichel, Jack;C;USA;1996-10-28\n8480727;54;Whitecloud, Zach;D;CAN; 1996 - 11 - 28\n8481540; 8;Caufield, Cole;R;USA; 2001 - 01 - 02\n8470607; 16;Seabrook, Brent;D;CAN; 1985 - 04 - 20\n8481697; 12;Tieksola, Tuukka;R;FIN; 2001 - 06 - 22\n8479974; 4;Patrick, Nolan;C;CAN; 1998 - 09 - 19\n8476384; 8;Friberg, Max;R;SWE; 1992 - 11 - 20\n8479546; 6;Steen, Oskar;C;SWE; 1998 - 03 - 09\n8479580; 28;Gambrell, Dylan;C;USA; 1996 - 08 - 26\n8476463; 30;Brodin,Jonas;D;SWE; 1993 - 07 - 12\n8478147; 26;Imama, Bokondji;L;CAN; 1996 - 08 - 03\n8478912; 6;Becker, Jack;C;USA; 1997 - 06 - 24\n8474037; 4;van Riemsdyk,James;L;USA; 1989 - 05 - 04\n8481093; 8;Harvey - Pinard,Rafael;L;CAN; 1999 - 01 - 06\n8476982; 3;O'Regan, Danny;C;DEU;1994-01-30\n8471691; 25;Hanzal, Martin;C;CZE; 1987 - 02 - 20\n8482065; 3;Rueschhoff, Austin;R;USA; 1997 - 09 - 07\n8476372; 16;Seeler, Nick;D;USA; 1993 - 06 - 03\n8482063; 23;Michaelis, Marc;L;DEU; 1995 - 07 - 31\n8479985; 8;Fleury, Cale;D;CAN; 1998 - 11 - 19\n8469454; 15;Kovalchuk,Ilya;L;RUS; 1983 - 04 - 15\n8471309; 17;Franzen, Johan;R;SWE; 1979 - 12 - 23\n8477044; 9;Wikstrand, Mikael;D;SWE; 1993 - 11 - 05\n8470753; 5;Isakov, Evgeni;L;RUS; 1984 - 10 - 13\n8476462; 12;Hamilton, Dougie;D;CAN; 1993 - 06 - 17\n8470630; 3;Klein, Kevin;D;CAN; 1984 - 12 - 13\n8477448; 29;Dano, Marko;L;AUT; 1994 - 11 - 30\n8475179; 52;Kulikov,Dmitry;D;RUS; 1990 - 10 - 29\n8481674; 17;Berglund, Gustav;D;SWE; 2001 - 01 - 08\n8459200; 9;Ronnquist, Petter;G;SWE; 1973 - 02 - 07\n8476397; 22;Lowe, Keegan;D;USA; 1993 - 03 - 29\n8476396; 3;Fogarty, Steven;C;USA; 1993 - 04 - 19\n8480213; 1;Senn, Gilles;G;CHE; 1996 - 03 - 01\n8475220; 30;Foligno, Marcus;L;USA; 1991 - 08 - 10\n8478442; 22;Marody, Cooper;C;USA; 1996 - 12 - 20\n8477961; 16;Quenneville, John;C;CAN; 1996 - 04 - 16\n8481715; 3;Skinner, Hunter;D;USA; 2001 - 04 - 29\n8481649; 52;Luoto, Joona;L;FIN; 1997 - 09 - 26\n8480467; 26;Strand, Austin;D;CAN; 1997 - 02 - 17\n8474027; 4;Braun, Justin;D;USA; 1987 - 02 - 10\n8473614; 23;Bachman, Richard;G;USA; 1987 - 07 - 25\n8480083; 7;Smith, C.J.;L;USA; 1994 - 12 - 01\n8479536; 15;Jonsson - Fjallby,Axel;L;SWE; 1998 - 02 - 10\n8481665; 25;Porco, Nicholas;L;CAN; 2001 - 03 - 12\n8469493; 20;Taratukhin, Andrei;C;RUS; 1983 - 02 - 22\n";
	string stats_csv = "8478427;28;38;68;10\n8478403;42;36;68;5\n8480727;1;0;16;3\n8470607;1;3;32;1\n8479580;6;5;50;-8\n8476463;26;2;69;15\n8474037;21;19;66;5\n8476372;1;0;12;-8\n8479985;0;1;41;-4\n8469454;16;10;46;-4\n8476462;26;14;47;30\n8477448;0;0;3;-1\n8475179;8;2;51;-4\n8476396;0;0;7;-1\n8475220;14;11;59;8\n8477961;0;0;9;-1\n8481649;0;0;16;-3\n8474027;16;3;62;-2\n";
	string teams_csv = "14;TBL;Tampa Bay Lightning\n10;TOR;Toronto Maple Leafs\n53;ARI;Arizona Coyotes\n26;LAK;Los Angeles Kings\n1;NJD;New Jersey Devils\n6;BOS;Boston Bruins\n3;NYR;New York Rangers\n12;CAR;Carolina Hurricanes\n9;OTT;Ottawa Senators\n21;COL;Colorado Avalanche\n13;FLA;Florida Panthers\n4;PHI;Philadelphia Flyers\n22;EDM;Edmonton Oilers\n5;PIT;Pittsburgh Penguins\n18;NSH;Nashville Predators\n20;CGY;Calgary Flames\n16;CHI;Chicago Blackhawks\n23;VAN;Vancouver Canucks\n29;CBJ;Columbus Blue Jackets\n2;NYI;New York Islanders\n24;ANA;Anaheim Ducks\n8;MTL;Montréal Canadiens\n7;BUF;Buffalo Sabres\n25;DAL;Dallas Stars\n54;VGK;Vegas Golden Knights\n30;MIN;Minnesota Wild\n28;SJS;San Jose Sharks\n52;WPG;Winnipeg Jets\n19;STL;St.Louis Blues\n15;WSH;Washington Capitals\n17;DET;Detroit Red Wings\n";
	
	League league(teams_csv, players_csv, stats_csv);
	Stats stats = league.stats();
	vector<Team> teams = league.teams();
	for (unsigned i = 0; i < teams.size(); ++i) {
		cout << teams[i] << endl;
		const vector<Player> &players = teams[i].players();
		for (unsigned j = 0; j < players.size(); ++j) {
			cout << players[j] << endl;
			cout << stats.stat(players[j].id()) << endl << endl;
		}
		cout << endl << endl;
	}
	sort(teams.rbegin(), teams.rend());
	for (unsigned i = 0; i < teams.size(); ++i) {
		cout << teams[i].id() << " " << teams[i].abbreviation() << " "
			<< teams[i].name() << endl;
	}
	cout << endl << endl;

	vector<Date> dates;
	vector<Player> players = league.players();
	for (unsigned i = 0; i < players.size(); ++i) {
		cout << players[i] << endl;
		dates.push_back(players[i].birthDate());
	}
	cout << endl << endl;

	sort(players.rbegin(), players.rend());
	for (unsigned i = 0; i < players.size(); ++i) {
		cout << players[i].id() << " " << players[i].team_id() << " "
			<< players[i].name() << " " << players[i].position() << " "
			<< players[i].country() << " " << players[i].birthDate() << endl;
	}
	cout << endl << endl;

	std::map<int, std::vector<Player>::size_type> playerMap;
	for (unsigned int i = 0; i < players.size(); ++i) {
		playerMap[players[i].id()] = i;
	}
	vector<Stat> stat_vec = league.stats().statsBy();
	for (unsigned i = 0; i < stat_vec.size(); ++i) {
		cout << players[playerMap[stat_vec[i].player_id()]] << endl;
		cout << "points: " << stat_vec[i].goals() + stat_vec[i].assists()
			<< endl << endl;
	}
	cout << endl << endl;

	stat_vec = league.stats().statsBy("goals");
	for (unsigned i = 0; i < stat_vec.size(); ++i) {
		cout << players[playerMap[stat_vec[i].player_id()]] << endl;
		cout << "goals: " << stat_vec[i].goals() << endl << endl;
	}
	cout << endl << endl;

	stat_vec = league.stats().statsBy("assists");
	for (unsigned i = 0; i < stat_vec.size(); ++i) {
		cout << players[playerMap[stat_vec[i].player_id()]] << endl;
		cout << "assists: " << stat_vec[i].assists() << endl << endl;
	}
	cout << endl << endl;

	stat_vec = league.stats().statsBy("points_per_game");
	for (unsigned i = 0; i < stat_vec.size(); ++i) {
		cout << players[playerMap[stat_vec[i].player_id()]] << endl;
		float ppg = 0;
		if (stat_vec[i].games() > 0) {
			ppg = stat_vec[i].assists() + stat_vec[i].goals();
			ppg /= stat_vec[i].games();
		}
		cout << "points per game: " << fixed << setprecision(2)
			<< ppg << endl << endl;
	}
	cout << endl << endl;

	sort(dates.begin(), dates.end());
	for (unsigned i = 0; i < dates.size(); ++i) {
		cout << dates[i].day() << "." << dates[i].month() << "."
			<< dates[i].year() << " == " << dates[i] << endl;
	}
	cout << endl << endl;

	return 0;
}
