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


void GameManager::registerTeam(Playable::Team team, Playable::PlayFunction pfBase, Playable::PlayFunction pfMiningShip, Playable::PlayFunction pfFighter)
{
    TeamInfo& ti = _teamMap[team];
    ti.play_base = pfBase;
    ti.play_miningShip = pfMiningShip;
    ti.play_fighter = pfFighter;
}

Playable::PlayFunction GameManager::getBasePF(Playable::Team team) const
{
    return _getTeamInfo(team).play_base;
}

Playable::PlayFunction GameManager::getMiningShipPF(Playable::Team team) const
{
    return _getTeamInfo(team).play_miningShip;
}

Playable::PlayFunction GameManager::getFighterPF(Playable::Team team) const
{
    return _getTeamInfo(team).play_fighter;
}

const GameManager::TeamInfo& GameManager::_getTeamInfo(Playable::Team team) const
{
    TeamMap::const_iterator it = _teamMap.find(team);
    if(it != _teamMap.end())
	return it->second;
    else
	throw std::runtime_error("Team is not registered");
}

GameManager::TeamInfo::TeamInfo()
    : play_base(0), play_miningShip(0), play_fighter(0),
      nb_base(0), nb_base_max(0),
      nb_miningShip(0), nb_miningShip_max(0),
      nb_fighter(0), nb_fighter_max(0)
{
}
