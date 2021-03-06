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
SDName: Tirisfal_Glades
SD%Complete: 100
SDComment: Quest support: 590, 1819
SDCategory: Tirisfal Glades
EndScriptData */

/* ContentData
go_mausoleum_door
go_mausoleum_trigger
npc_calvin_montague
npc_darkcaller_yanka
EndContentData */
#include "precompiled.h"

/*######
## go_mausoleum_door
## go_mausoleum_trigger
######*/

enum
{
    QUEST_ULAG      = 1819,
    NPC_ULAG        = 6390,
    GO_TRIGGER      = 104593,
    GO_DOOR         = 176594
};


bool GOUse_go_mausoleum_door(Player* pPlayer, GameObject* /*pGo*/)
{
    if (pPlayer->GetQuestStatus(QUEST_ULAG) != QUEST_STATUS_INCOMPLETE)
        return false;

    if (GameObject* pTrigger = GetClosestGameObjectWithEntry(pPlayer, GO_TRIGGER, 30.0f))
    {
        pTrigger->SetGoState(GO_STATE_READY);
        pPlayer->SummonCreature(NPC_ULAG, 2390.26f, 336.47f, 40.01f, 2.26f, TEMPSUMMON_TIMED_OOC_OR_DEAD_DESPAWN, 300000);
        return false;
    }

    return false;
}

bool GOUse_go_mausoleum_trigger(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetQuestStatus(QUEST_ULAG) != QUEST_STATUS_INCOMPLETE)
        return false;

    if (GameObject* pDoor = GetClosestGameObjectWithEntry(pPlayer, GO_DOOR, 30.0f))
    {
        pGo->SetGoState(GO_STATE_ACTIVE);
        pDoor->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_INTERACT_COND);
        return true;
    }

    return false;
}

/*######
## npc_calvin_montague
######*/

enum
{
    SAY_COMPLETE        = -1000356,
    SPELL_DRINK         = 2639,                             // possibly not correct spell (but iconId is correct)
    QUEST_590           = 590,
    FACTION_HOSTILE     = 168
};

struct npc_calvin_montagueAI : public ScriptedAI
{
    npc_calvin_montagueAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiPhase;
    uint32 m_uiPhaseTimer;
    ObjectGuid m_playerGuid;

    void Reset() override
    {
        m_uiPhase = 0;
        m_uiPhaseTimer = 5000;
        m_playerGuid.Clear();
    }

    void AttackedBy(Unit* pAttacker) override
    {
        if (m_creature->getVictim() || m_creature->IsFriendlyTo(pAttacker))
            return;

        AttackStart(pAttacker);
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage) override
    {
        if (uiDamage > m_creature->GetHealth() || ((m_creature->GetHealth() - uiDamage) * 100 / m_creature->GetMaxHealth() < 15))
        {
            uiDamage = 0;

            m_creature->CombatStop(true);

            m_uiPhase = 1;

            if (pDoneBy->GetTypeId() == TYPEID_PLAYER)
                m_playerGuid = pDoneBy->GetObjectGuid();
        }
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_uiPhase)
        {
            if (m_uiPhaseTimer < uiDiff)
                m_uiPhaseTimer = 7500;
            else
            {
                m_uiPhaseTimer -= uiDiff;
                return;
            }

            switch (m_uiPhase)
            {
                case 1:
                    DoScriptText(SAY_COMPLETE, m_creature);
                    ++m_uiPhase;
                    break;
                case 2:
                    if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                        pPlayer->AreaExploredOrEventHappens(QUEST_590);

                    m_creature->CastSpell(m_creature, SPELL_DRINK, true);
                    ++m_uiPhase;
                    break;
                case 3:
                    EnterEvadeMode();
                    break;
            }

            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_calvin_montague(Creature* pCreature)
{
    return new npc_calvin_montagueAI(pCreature);
}

bool QuestAccept_npc_calvin_montague(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_590)
    {
        pCreature->SetFactionTemporary(FACTION_HOSTILE, TEMPFACTION_RESTORE_COMBAT_STOP | TEMPFACTION_RESTORE_RESPAWN);
        pCreature->AI()->AttackStart(pPlayer);
    }
    return true;
}

/*######
## npc_darkcaller_yanka
######*/

enum
{
	SAY_30_MIN_NOTICE       = -1000891,
	SAY_15_MIN_NOTICE       = -1000892,
	SAY_5_MIN_NOTICE        = -1000893,
	SAY_FESTIVAL_START      = -1000894,
    
    EMOTE_DANCE             = 10,
    EMOTE_STOP              = 0,
    
    ENTRY_BANSHEE_QUEEN     = 15193,
    
    GO_WICKERMAN            = 180433,
    GO_WICKERMAN_EMBER      = 180437,
	

    GUID_WICKERMAN_EMBER_1  = 37875,
    GUID_WICKERMAN_EMBER_2  = 37876, 
    GUID_WICKERMAN_EMBER_3  = 37877, 
    GUID_WICKERMAN_EMBER_4  = 37878, 
    GUID_WICKERMAN_EMBER_5  = 37886, 
    GUID_WICKERMAN_EMBER_6  = 37887,
    
    DANCING_STOP_SECONDS    = 21600, /* Stops at 6am so 6*3600 seconds */
    DANCING_START_SECONDS   = 72068 /* Starts at 8:01:08 so (20*3600) + 60 + 10 seconds */
	 
};

struct SpawnPos
{         
    float x;
    float y;
    float z;
    float o;
};     

static const SpawnPos spawnCoords[6] =
{
    { 1721.62, 499.971, 41.4038, 0.03217 },
    { 1722.44, 508.141, 40.4945, 5.35507 },
    { 1733.45, 494.315, 42.1595, 1.44606 },
    { 1734.53, 510.872, 40.6531, 4.60175 },
    { 1739.41, 508.949, 40.7728, 4.31508 },
    { 1743.79, 501.708, 41.4234, 3.42758 },
};



struct npc_darkcaller_yankaAI : public ScriptedAI
{
	npc_darkcaller_yankaAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
	}
	time_t currenttime_raw;
	struct tm *currenttime;
	bool   bAnnounced = false;
	uint32 uiSecondsIntoDay;
    GuidList pGuids;
    GameObject* wickedEmber[6];
    
	void Reset() override
	{
		uiSecondsIntoDay = 0;
	}

	void UpdateAI(const uint32 uiDiff) override
	{

		time(&currenttime_raw);
		currenttime = localtime(&currenttime_raw);
		uiSecondsIntoDay = (currenttime->tm_hour * 3600) + (currenttime->tm_min * 60) + (currenttime->tm_sec);
		GameObject* pWickerman = GetClosestGameObjectWithEntry(m_creature, GO_WICKERMAN, 30.0f);

		/* Yanka Pre-Festival Announcements at 19:30, 19:45 and 19:55*/

		if ((currenttime->tm_hour == 19) && (currenttime->tm_sec == 0))
		{
			switch (currenttime->tm_min)
			{
			case 30:
				if (!bAnnounced)
				{
					DoScriptText(SAY_30_MIN_NOTICE, m_creature);
					bAnnounced = true;
				}
				break;

			case 45:
				if (!bAnnounced)
				{
					DoScriptText(SAY_15_MIN_NOTICE, m_creature);
					bAnnounced = true;
				}
				break;

			case 55:
				if (!bAnnounced)
				{
					DoScriptText(SAY_5_MIN_NOTICE, m_creature);
					bAnnounced = true;
				}
				break;

			default:
				bAnnounced = false;
				break;
			}
		}

		/* Festival Start at 20:00:00 */

		if ((currenttime->tm_hour == 20) && (currenttime->tm_min == 0) && (currenttime->tm_sec == 0))
		{
			if (!bAnnounced)
			{
				DoScriptText(SAY_FESTIVAL_START, m_creature);
                m_creature->SummonCreature(ENTRY_BANSHEE_QUEEN, 1734.6f, 508.803f, 41.2847f, 1.6173f, TEMPSUMMON_TIMED_OOC_OR_CORPSE_DESPAWN, 69000);
				bAnnounced = true;			
			}
		}

        // GameObject* SummonGameObject(uint32 id, float x, float y, float z, float angle, uint32 despwtime);
        
        if (pWickerman->GetGoState() == GO_STATE_ACTIVE)
        {
            for (uint16 i = 0; i < 6; i++)
            {
                if (!wickedEmber[i])
                    wickedEmber[i] = m_creature->SummonGameObject(GO_WICKERMAN_EMBER, spawnCoords[i].x, spawnCoords[i].y, spawnCoords[i].z, spawnCoords[i].o, 36000);
            }
        }
        else if (pWickerman->GetGoState() == GO_STATE_READY)
        {
            for (uint16 i = 0; i < 6; i++)
            {
                if (wickedEmber[i])
                {
                    wickedEmber[i]->RemoveFromWorld();
                    delete wickedEmber[i];
                    wickedEmber[i] = nullptr;
                }
            }
        }

		/* Start-Stop Dancing and Ignite-Putout Wickerman */

		if ((uiSecondsIntoDay < DANCING_STOP_SECONDS) || (uiSecondsIntoDay > DANCING_START_SECONDS))
		{
            
			m_creature->HandleEmote(EMOTE_DANCE);
            pWickerman->SetGoState(GO_STATE_ACTIVE);
		}
		else
		{
			m_creature->HandleEmote(EMOTE_STOP);
            pWickerman->SetGoState(GO_STATE_READY);
		}
    }

};
    

	CreatureAI* GetAI_npc_darkcaller_yanka(Creature* pCreature)
	{
		return new npc_darkcaller_yankaAI(pCreature);
	}

	void AddSC_tirisfal_glades()
	{
		Script* pNewScript;

		pNewScript = new Script;
		pNewScript->Name = "go_mausoleum_door";
		pNewScript->pGOUse = &GOUse_go_mausoleum_door;
		pNewScript->RegisterSelf();

		pNewScript = new Script;
		pNewScript->Name = "go_mausoleum_trigger";
		pNewScript->pGOUse = &GOUse_go_mausoleum_trigger;
		pNewScript->RegisterSelf();

		pNewScript = new Script;
		pNewScript->Name = "npc_calvin_montague";
		pNewScript->GetAI = &GetAI_npc_calvin_montague;
		pNewScript->pQuestAcceptNPC = &QuestAccept_npc_calvin_montague;
		pNewScript->RegisterSelf();

		pNewScript = new Script;
		pNewScript->Name = "npc_darkcaller_yanka";
		pNewScript->GetAI = &GetAI_npc_darkcaller_yanka;
		pNewScript->RegisterSelf();
}
