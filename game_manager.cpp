/*
 * Copyright (C) 2012 Paul Grégoire
 *
 * This file is part of AIWar.
 *
 * AIWar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AIWar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with AIWar.  If not, see <http://www.gnu.org/licenses/>. 
 */

#include "game_manager.hpp"

#include "base.hpp"
#include "miningship.hpp"
#include "fighter.hpp"
#include "missile.hpp"
#include "mineral.hpp"

#include <iostream>
#include <stdexcept>

using namespace aiwar::core;


GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::baseCreated(const Base* b)
{
    TeamInfo &info = _teamMap[b->team()];
    info.nb_base++;
    info.nb_base_max++;
}

void GameManager::baseDestroyed(const Base* b)
{
    TeamInfo &info = _teamMap[b->team()];
    info.nb_base--;
}

void GameManager::miningShipCreated(const MiningShip* m)
{
    TeamInfo &info = _teamMap[m->team()];
    info.nb_miningShip++;
    info.nb_miningShip_max++;
}

void GameManager::miningShipDestroyed(const MiningShip* m)
{
    TeamInfo &info = _teamMap[m->team()];
    info.nb_miningShip--;
}

void GameManager::fighterCreated(const Fighter* f)
{
    TeamInfo &info = _teamMap[f->team()];
    info.nb_fighter++;
    info.nb_fighter_max++;
}

void GameManager::fighterDestroyed(const Fighter* f)
{
    TeamInfo &info = _teamMap[f->team()];
    info.nb_fighter--;
}

void GameManager::missileCreated(const Missile*)
{
}

void GameManager::missileDestroyed(const Missile*)
{
}

void GameManager::mineralCreated(const Mineral*)
{
}

void GameManager::mineralDestroyed(const Mineral*)
{
}

void GameManager::itemDestroyed(const Item* item)
{
    const aiwar::core::Mineral *mineral;
    const aiwar::core::Missile *missile;
    const aiwar::core::MiningShip *miningShip;
    const aiwar::core::Base *base;
    const aiwar::core::Fighter *fighter;

    if((mineral = dynamic_cast<const aiwar::core::Mineral*>(item)))
	mineralDestroyed(mineral);
    else if((missile = dynamic_cast<const aiwar::core::Missile*>(item)))
	missileDestroyed(missile);
    else if((miningShip = dynamic_cast<const aiwar::core::MiningShip*>(item)))
	miningShipDestroyed(miningShip);
    else if((base = dynamic_cast<const aiwar::core::Base*>(item)))
	baseDestroyed(base);
    else if((fighter = dynamic_cast<const aiwar::core::Fighter*>(item)))
	fighterDestroyed(fighter);
}


void GameManager::printStat() const
{
    TeamMap::const_iterator cit;
    for(cit = _teamMap.begin() ; cit != _teamMap.end() ; ++cit)
    {
	std::cout << "*******************\n"
		  << "TEAM " << cit->first << ":\n"
		  << "\tBases:       " << cit->second.nb_base << " (max: " << cit->second.nb_base_max << ")\n"
		  << "\tMiningShips: " << cit->second.nb_miningShip << " (max: " << cit->second.nb_miningShip_max << ")\n"
		  << "\tFighters:    " << cit->second.nb_fighter << " (max: " << cit->second.nb_fighter_max << ")\n"
		  << "*******************\n";
    }
}


void GameManager::registerTeam(Team team, PlayFunction& pfBase, PlayFunction& pfMiningShip, PlayFunction& pfFighter)
{
    TeamInfo t(pfBase, pfMiningShip, pfFighter);
    _teamMap.insert(std::pair<Team, TeamInfo>(team, t));
}

PlayFunction& GameManager::getBasePF(Team team) const
{
    return _getTeamInfo(team).play_base;
}

PlayFunction& GameManager::getMiningShipPF(Team team) const
{
    return _getTeamInfo(team).play_miningShip;
}

PlayFunction& GameManager::getFighterPF(Team team) const
{
    return _getTeamInfo(team).play_fighter;
}

bool GameManager::gameOver() const
{
    int nbLivingTeam = 0;

    TeamMap::const_iterator cit;
    for(cit = _teamMap.begin() ; cit != _teamMap.end() ; ++cit)
    {
	if(cit->second.nb_base > 0)
	    nbLivingTeam++;
    }

    return (nbLivingTeam < 2);
}

Team GameManager::getWinner() const
{
    Team winner = NO_TEAM;
    int nbLivingTeam = 0;
    
    TeamMap::const_iterator cit;
    for(cit = _teamMap.begin() ; cit != _teamMap.end() ; ++cit)
    {
	if(cit->second.nb_base > 0)
	{
	    nbLivingTeam++;
	    winner = cit->first;
	}
    }

    if(nbLivingTeam > 1) // game not over
	return NO_TEAM;
    else
	return winner;
}

const GameManager::TeamInfo& GameManager::_getTeamInfo(Team team) const
{
    TeamMap::const_iterator it = _teamMap.find(team);
    if(it != _teamMap.end())
	return it->second;
    else
	throw std::runtime_error("Team is not registered");
}
    

GameManager::TeamInfo::TeamInfo(PlayFunction& pfb, PlayFunction& pfm, PlayFunction& pff)
    : play_base(pfb), play_miningShip(pfm), play_fighter(pff),
      nb_base(0), nb_base_max(0),
      nb_miningShip(0), nb_miningShip_max(0),
      nb_fighter(0), nb_fighter_max(0)
{
}
