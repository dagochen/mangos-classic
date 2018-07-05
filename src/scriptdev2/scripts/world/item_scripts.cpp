/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Item_Scripts
SD%Complete: 0
SDComment: Empty
SDCategory: Items
EndScriptData */

/* ContentData
EndContentData */


#include "precompiled.h"
#include "Language.h"

bool On_ItemUse_battleCom(Player* pPlayer, Item* pItem, SpellCastTargets const& targets)
{
    if (!pPlayer)
        return false;
   
    ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_INITIALIZING);
   
    if (pPlayer->HasSpellCooldown(29154))
    {
        ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_COOLDOWN);
        return false;
    }

    if (!pPlayer->GetMap()->IsContinent())
    {
        ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_DUNGEON);
        return false;
    }

    if (pPlayer->getLevel() < 20)
    {
        if (pPlayer->InBattleGroundQueue(BATTLEGROUND_WS))
        {
            ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_IN_QUEUE);
            return false;
        }
    }
    else if (pPlayer->getLevel() < 50)
    {
        if (pPlayer->InBattleGroundQueue(BATTLEGROUND_WS) && pPlayer->InBattleGroundQueue(BATTLEGROUND_AB))
        {
            ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_IN_QUEUE);
            return false;
        }
    }
    else
    {
        if (pPlayer->InBattleGroundQueue(BATTLEGROUND_WS) && pPlayer->InBattleGroundQueue(BATTLEGROUND_AB) && pPlayer->InBattleGroundQueue(BATTLEGROUND_AV))
        {
            ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_IN_QUEUE);
            return false;
        }
    }
 
    
    if (pPlayer->isInCombat())
    {
        ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_INFIGHT);
        return false;
    }
    if (pPlayer->GetMoney() < (pPlayer->getLevel() * 10))
    {
        ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_NO_MONEY);
        return false;
    }

    if (pPlayer->GetBattleGround())
    {
        ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_IN_BG);
        return false;
    }
  
    if (pPlayer->HasAura(26013))
    {
        ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_DESERTER);
        return false;
    }

    uint32 bgs[] = { 489, 30, 529 };

    for (uint32 i = 0; i < 3; ++i)
    {
        BattleGroundTypeId bgTypeId = BATTLEGROUND_TYPE_NONE;
        uint32 minlevel = 50;
        switch (bgs[i])
        {
            case 489:
            {
                bgTypeId = BATTLEGROUND_WS;
                minlevel = 10;
                break;
            }
            case 30:
            {
                bgTypeId = BATTLEGROUND_AV;
                minlevel = 50;
                break;
            }
            case 529:
            {
                bgTypeId = BATTLEGROUND_AB;
                minlevel = 20;
                break;
            }
            default:
                break;
        }
        if (pPlayer->getLevel() >= minlevel && !pPlayer->InBattleGroundQueue(bgTypeId))
            pPlayer->GetSession()->HackBattlemasterJoinOpcode(bgs[i], pPlayer);
    }
    ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_ESTABLISHED);
    ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_QUEUES_JOINED);
    ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_FEE, float(pPlayer->getLevel() / 10.0f));
    pPlayer->ModifyMoney(pPlayer->getLevel() * -10);
    ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_PVP_STATUS);
    pPlayer->UpdatePvP(true, true);
    ChatHandler(pPlayer).PSendSysMessage(LANG_BATTLECOM_SHUTDOWN);
    
    pPlayer->AddSpellCooldown(29154, pItem->GetEntry(), 30);
    
    return true;
}

void AddSC_item_scripts()
{
    Script* newScript = new Script;
    newScript->Name = "item_Battle_Communicator";
    newScript->pItemUse = &On_ItemUse_battleCom;
    newScript->RegisterSelf();
}