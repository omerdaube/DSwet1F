#include "worldcup23a1.h"
world_cup_t::world_cup_t() : playersByID(), playersByGoals(), teams(), nonEmptyTeams(), playersByGoalsList(), totalPlayers(0), totalTeams(0), bestPlayer(nullptr)
{

}


world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId <= 0 || points < 0) {
        return StatusType::INVALID_INPUT;
    }
    if(teams.search(teamId) != nullptr) {
        return StatusType::FAILURE;
    }
    Team newTeam = Team(teamId, points);
    teams.add(newTeam);
    totalTeams++;
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId <= 0)
        return StatusType::INVALID_INPUT;
    AVL<Team> *ret = teams.search(teamId);
    if(ret == nullptr) {
        return StatusType::FAILURE;
    }
    if(ret->data.isEmpty()){
        return StatusType::FAILURE;
    }
    teams.remove(ret->data);
    totalTeams--;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
    bool check1 = false, check2 = false;
	if(playerId <= 0){
        return StatusType::INVALID_INPUT;
    }
    AVL<Player>  *ret = playersByID.search(playerId);
    if(ret == nullptr) {
        return StatusType::FAILURE;
    }
    Player p = ret->data;
    PointerPlayer* pp = p.getPlayerPointer();
    Team* t = p.getTeam();
    if(t->getNumPlayers() >= 11 && t->hasGaurd() == true) {
        check1 = true;
    }
    t->getAVL().remove(*pp);
    delete pp;
    totalPlayers -= 1;
    t->setTotalGoals(t->getTotalGoals() - p.getGoals());
    t->setTotalCards(t->getTotalCards() - p.getCards());
    t->setNumPlayers(t->getNumPlayers() - 1);
    if(p->getIsGaurd()) {
        t->setNumGuards(t->getNumGuards() - 1);
    }
    if(t->getNumPlayers() < 11 || t->hasGaurd() == false) {
        check2 = true;
    }
    if(check1 && check2){
        PointerTeam* pt = t->getNonEmpty()
        nonEmptyTeams.remove(pt);
        delete pt;
    }
    if(p == t->getBestGoals()) {
        t->setBestGoals(t->getAVL().getMostRight()->data->getPlayerP());
    }
    if(p == bestPlayer){
        bestPlayer = playersByGoals->getMostRight()->data->getPlayerP();
    }
    PointerPlayer* ppGoal = p.getPlayerInGoals();
    playersByGoals.remove(ppGoal);
    delete ppGoal;
    playersByID.remove(p);
    delete p;

}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2){
        return StatusType::INVALID_INPUT;
    }
    AVL<Team> *retT1 = teams.search(teamId1);
    AVL<Team> *retT2 = teams.search(teamId2);
    if (retT1 == nullptr || retT2 == nullptr){
        return StatusType::FAILURE;
    }
    if (retT1->data->getNumPlayers() < 11 ||
        retT2->data->getNumPlayers() < 11 ||
        retT1->data->hasGuard() == false ||
        retT2->data->hasGuard() == false){
        return StatusType::FAILURE;
    }
    int calc1 = retT1->getTotalGoals() - retT1->getTotalCards() + retT1->getPoints();
    int calc2 = retT2->getTotalGoals() - retT2->getTotalCards() + retT2->getPoints();
    if(calc1 == calc2){
        retT1->setPoints(retT1->getPoints() + 1);
        retT2->setPoints(retT2->getPoints() + 1);
        retT1->nonEmpty()->setPoints(retT1->getPoints() + 1);
        retT2->nonEmpty()->setPoints(retT2->getPoints() + 1);
    }
    else if (calc1 < calc2){
        retT2->setPoints(retT2->getPoints() + 3);
        retT2->nonEmpty()->setPoints(retT2->getPoints() + 3);
    }
    else{
        retT1->setPoints(retT1->getPoints() + 3);
        retT1->nonEmpty()->setPoints(retT1->getPoints() + 3);
    }
    retT1->setNumGames(retT1->getNumGames() + 1);
    retT2->setNumGames(retT2->getNumGames() + 1);
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if(playerId <= 0){
        return StatusType::INVALID_INPUT;
    }
    AVL<Player>  *ret = playersByID.search(playerId);
    if(ret == nullptr) {
        return StatusType::FAILURE;
    }
    Player p = ret->data;
    Team* t = p.getTeam();
    return output_t<int>(p.getNumGames() + t->getNumGames());
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    AVL<Team> *ret = teams.search(teamId);
    if(ret == nullptr) {
        return StatusType::FAILURE;
    }
    return output_t<int>(ret->data->getPoints());
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if(teamId == 0){
        return StatusType::INVALID_INPUT;
    }
    else if(teamId > 0){
        AVL<Team> *ret = teams.search(teamId);
        if(ret == nullptr) {
            return StatusType::FAILURE;
        }
        return output_t<int>(ret->data->getBestGoals()->getPlayerID());
    }
    if(totalPlayers <= 0){
        return StatusType::FAILURE;
    }
    return output_t<int>(bestPlayer->getPlayerID());

}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if(teamId == 0){
        return StatusType::INVALID_INPUT;
    }
    else if(teamId > 0){
        AVL<Team> *ret = teams.search(teamId);
        if(ret == nullptr) {
            return StatusType::FAILURE;
        }
        return output_t<int>(ret->data->getNumPlayers());
    }
    return output_t<int>(totalPlayers);
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}

