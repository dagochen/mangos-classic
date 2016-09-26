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
SDName: Stranglethorn_Vale
SD%Complete: 100
SDComment: Quest support: 592
SDCategory: Stranglethorn Vale
EndScriptData */

/* ContentData
mob_yenniku
EndContentData */
#include "GameEventMgr.h"
#include "precompiled.h"

/*######
## mob_yenniku
######*/

enum
{
    SPELL_YENNIKUS_RELEASE      = 3607,

    QUEST_ID_SAVING_YENNIKU     = 592,

    FACTION_ID_HORDE_GENERIC    = 83,                       // Note: faction may not be correct!
};

struct mob_yennikuAI : public ScriptedAI
{
    mob_yennikuAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiResetTimer;

    void Reset() override { m_uiResetTimer = 0; }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell) override
    {
        if (pSpell->Id == SPELL_YENNIKUS_RELEASE && pCaster->GetTypeId() == TYPEID_PLAYER)
        {
            if (!m_uiResetTimer && ((Player*)pCaster)->GetQuestStatus(QUEST_ID_SAVING_YENNIKU) == QUEST_STATUS_INCOMPLETE)
            {
                m_uiResetTimer = 60000;
                EnterEvadeMode();
            }
        }
    }

    void EnterEvadeMode() override
    {
        if (m_uiResetTimer)
        {
            m_creature->RemoveAllAurasOnEvade();
            m_creature->DeleteThreatList();
            m_creature->CombatStop(true);
            m_creature->LoadCreatureAddon(true);

            m_creature->SetLootRecipient(nullptr);

            m_creature->HandleEmote(EMOTE_STATE_STUN);
            m_creature->SetFactionTemporary(FACTION_ID_HORDE_GENERIC, TEMPFACTION_RESTORE_REACH_HOME);
        }
        else
            ScriptedAI::EnterEvadeMode();
    }

    void UpdateAI(const uint32 uiDiff) override
    {
        if (m_uiResetTimer)
        {
            if (m_uiResetTimer <= uiDiff)
            {
                m_creature->HandleEmote(EMOTE_STATE_NONE);
                m_uiResetTimer = 0;
                EnterEvadeMode();
            }
            else
                m_uiResetTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_yenniku(Creature* _Creature)
{
    return new mob_yennikuAI(_Creature);
}

enum
{
    SPELL_CREATE_HEART_OF_HAKKAR_RIFT = 24202,
    SPELL_HEART_OF_HAKKAR_RITUAL_CAST = 24217,
    SPELL_HEART_OF_HAKKAR_IMPLODE = 24204,
    SPELL_HELLFIRE_CAST_VISUAL = 24207,
    SPELL_HEART_OF_HAKKAR_SUMMON_CIRCLE = 24602,
    SPELL_HEART_OF_HAKKAR_BANNING = 24203,
    SPELL_HEART_OF_HAKKAR_MOLTHOR_CHUCKS_THE_HEART = 24214,
    SPELL_CREATE_HEART_OF_HAKKAR_EXPLOISON = 24215,
    SPELL_SPIRIT_OF_ZANDALAR = 24425,

    SPELL_TELEPORT_SPAWN_OUT = 24221,

    SPELL_SPAWN_RED_LIGHTNING = 24240,

    SPELL_QUEST_TROLL_HERO_SUMMON_VISUAL = 24258,

    NPC_TARGET_DUMMY = 15069,
    NPC_SERVANT = 15080,
    GO_HEART_OF_HAKKAR = 180402
};


const float dummy_position[4] = { -11818.9f, 1343.2f, 7.9f, 4.35f };
const float heart_position[4] = { -11818.9f, 1342.5f, 9.9f, 4.35f };

const float servant_positions[4][4] =
{
    { -11835.03f, 1349.51f, 2.01f, 5.90f },
    { -11831.56f, 1331.27f, 1.84f, 1.14f },
    { -11817.59f, 1324.89f, 1.46f, 1.14f },
    { -11801.14f, 1335.25f, 1.26f, 2.87f },
};

struct mob_molthor : public ScriptedAI
{
    mob_molthor(Creature *c) : ScriptedAI(c)
    {
        Reset();
    }

    uint32 mEventStep = 0;
    bool mJustStepped = true;
    uint32 mEventTimer = 0;

    Player* target;
    Creature* targetDummy;
    GameObject* heart;

    std::vector<Creature*> servants;

    void Reset() override
    {
        target = nullptr;
        targetDummy = nullptr;
        heart = nullptr;
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

        mEventStep = 0;
        mJustStepped = true;
        mEventTimer = 0;

        m_creature->m_movementInfo.AddMovementFlag(MOVEFLAG_WALK_MODE);
        servants.clear();
    }

    void MovementInform(uint32, uint32) override
    {
        ++mEventStep;
        mJustStepped = false;
    }

    void StartQuestEvent(Player* plr)
    {

        target = plr;
        mJustStepped = false;
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        m_creature->m_movementInfo.RemoveMovementFlag(MOVEFLAG_WALK_MODE);

        targetDummy = m_creature->SummonCreature(NPC_TARGET_DUMMY, dummy_position[0], dummy_position[1], dummy_position[2], dummy_position[3], TEMPSUMMON_MANUAL_DESPAWN, 0);
        heart = plr->SummonGameObject(GO_HEART_OF_HAKKAR, heart_position[0], heart_position[1], heart_position[2], heart_position[3], 90000);

        if (!targetDummy)
        {
            m_creature->MonsterSay("Etwas lief falsch, bitte beim Team melden!", 0);
            m_creature->Respawn();
            Reset();
            return;
        }

    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
    }

    void UpdateAI(uint32 timeDelta) override
    {
        if (mJustStepped)
            return;

        if (mEventTimer > timeDelta)
        {
            mEventTimer -= timeDelta;
            return;
        }

        if (mEventStep == 0)
        {
            m_creature->GetMotionMaster()->MovePoint(0, -11817.48f, 1250.02f, 2.64f);
            mJustStepped = true;
        }
        else if (mEventStep == 1)
        {
            m_creature->GetMotionMaster()->MovePoint(0, -11829.73f, 1258.05f, 1.88f);
            mJustStepped = true;
        }
        else if (mEventStep == 2)
        {
            m_creature->GetMotionMaster()->MovePoint(0, -11837.02f, 1293.10f, 0.69f);
            mJustStepped = true;
        }
        else if (mEventStep == 3)
        {
            m_creature->GetMotionMaster()->MovePoint(0, -11825.56f, 1322.88f, 0.29f);
            mJustStepped = true;
            mEventTimer = 1000;
        }
        else if (mEventStep == 4)
        {
            m_creature->CastSpell(m_creature, SPELL_QUEST_TROLL_HERO_SUMMON_VISUAL, false);
            ++mEventStep;
            mEventTimer = 30000;
        }
        else if (mEventStep == 5)
        {
            for (int i = 0; i < 4; ++i)
            {
                auto cr = m_creature->SummonCreature(NPC_SERVANT, servant_positions[i][0], servant_positions[i][1], servant_positions[i][2], servant_positions[i][3],
                    TEMPSUMMON_MANUAL_DESPAWN, 0);
                if (!cr)
                {
                    m_creature->MonsterSay("Etwas lief falsch, bitte beim Team melden!", 0);
                    m_creature->Respawn();
                    Reset();
                    return;
                }

                cr->CastSpell(cr, SPELL_SPAWN_RED_LIGHTNING, true);
                servants.push_back(cr);
            }


            ++mEventStep;
            mEventTimer = 2000;
        }
        else if (mEventStep == 6)
        {

            m_creature->CastSpell(targetDummy, SPELL_HEART_OF_HAKKAR_MOLTHOR_CHUCKS_THE_HEART, true);

            ++mEventStep;
            mEventTimer = 5000;
        }


        else if (mEventStep == 7)
        {

            targetDummy->CastSpell(targetDummy, SPELL_CREATE_HEART_OF_HAKKAR_RIFT, true);

            ++mEventStep;
            mEventTimer = 500;
        }
        else if (mEventStep == 8)
        {
            DoScriptText(-1200000, m_creature);
            for (int i = 0; i < 4; ++i)
            {
                servants[i]->CastSpell(servants[i], SPELL_HEART_OF_HAKKAR_SUMMON_CIRCLE, true);
                servants[i]->CastSpell(servants[i], SPELL_HEART_OF_HAKKAR_RITUAL_CAST, true);

            }
            m_creature->CastSpell(m_creature, SPELL_HEART_OF_HAKKAR_BANNING, true);
            ++mEventStep;
            mEventTimer = 30000;
        }
        else if (mEventStep == 9)
        {
            m_creature->CastSpell(targetDummy, SPELL_CREATE_HEART_OF_HAKKAR_EXPLOISON, true);
            m_creature->CastSpell(targetDummy, SPELL_HELLFIRE_CAST_VISUAL, true);
            heart->SetLootState(GO_JUST_DEACTIVATED);
            heart->RemoveFromWorld();
            ++mEventStep;
            mEventTimer = 4000;
        }
        else if (mEventStep == 10)
        {
            DoScriptText(-1200001, m_creature);
            ++mEventStep;
            mEventTimer = 1000;
        }
        else if (mEventStep == 11)
        {
            m_creature->CastSpell(target, SPELL_SPIRIT_OF_ZANDALAR, false);
            ++mEventStep;
            mEventTimer = 4000;
        }
        else if (mEventStep == 12)
        {
            m_creature->GetMotionMaster()->MovePoint(0, -11837.02f, 1293.10f, 0.69f);
            mJustStepped = true;
        }
        else if (mEventStep == 13)
        {
            m_creature->GetMotionMaster()->MovePoint(0, -11829.73f, 1258.05f, 1.88f);
            mJustStepped = true;
        }
        else if (mEventStep == 14)
        {
            m_creature->GetMotionMaster()->MovePoint(0, -11817.48f, 1250.02f, 2.64f);
            mJustStepped = true;
        }
        else if (mEventStep == 15)
        {
            auto a = m_creature->GetMotionMaster()->top();

            float x, y, z;
            m_creature->GetRespawnCoord(x, y, z);
            m_creature->GetMotionMaster()->MovePoint(0, x, y, z);
            mJustStepped = true;
        }
        else if (mEventStep == 16)
        {
            m_creature->SetFacingTo(4.24f);
            targetDummy->ForcedDespawn();
            for (int i = 0; i < 4; ++i)
                servants[i]->ForcedDespawn();
            targetDummy->ForcedDespawn();

            Reset();
        }
    }


};

bool onHakkarQuestComplete(Player* plr, Creature* cr, Quest const*)
{
    if (cr->AI() && plr && cr)
        static_cast<mob_molthor*>(cr->AI())->StartQuestEvent(plr);

    return true;
}


enum Riggle
{
        SAY_START = 0,
        SAY_WINNER = 1,
        SAY_END = 2,
        QUEST_MASTER_ANGLER = 8193
};



struct mob_riggle_bassbait : public ScriptedAI
{
    mob_riggle_bassbait(Creature *c) : ScriptedAI(c)
    {
        Reset();
    }
    
    time_t currenttime_raw;
    struct tm *currenttime;
    uint32 uiSecondsIntoDay;
    ObjectGuid winner;
    bool EndEvent;
    void Reset() override
    {
        EndEvent = false;
    }
       
            
    void UpdateAI(uint32 diff)
    {
        time(&currenttime_raw);
        currenttime = localtime(&currenttime_raw);

        if (EndEvent)
            return;

        if ((currenttime->tm_hour == 14) && (currenttime->tm_sec == 0) && IsHolidayActive(HOLIDAY_FISHING_EXTRAVAGANZA))
        {
            DoScriptText(SAY_START, m_creature);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        }
                
        if ((currenttime->tm_hour == 16) && (currenttime->tm_sec == 0) || winner)
        {
            if (Player* pPlayer = m_creature->GetMap()->GetPlayer(winner))
                DoScriptText(SAY_WINNER, m_creature);
            
            DoScriptText(SAY_END, m_creature);
            EndEvent = true;
        }
     }

        
    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
        {
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());
            pPlayer->SEND_GOSSIP_MENU(7614, pCreature->GetGUID());//Gossip DB
            return true;
        }
            
        pPlayer->SEND_GOSSIP_MENU(7714, pCreature->GetGUID());//Gossip DB
        return true;
    }
        
    bool ChooseReward(Player* pPlayer, Creature* pCreature, const Quest *pQuest, uint32 item)
    {
        if (pQuest->GetQuestId() == QUEST_MASTER_ANGLER)
        {
            pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            ((mob_riggle_bassbait*)(pCreature->AI()))->winner = pPlayer->GetObjectGuid();
        }
        return true;
    }
        
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_riggle_bassbait(pCreature);
    }
};


/*######
##
######*/

void AddSC_stranglethorn_vale()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "mob_yenniku";
    pNewScript->GetAI = &GetAI_mob_yenniku;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_molthor";
    pNewScript->GetAI = [](Creature* c)->CreatureAI* { return new mob_molthor(c); };
    pNewScript->pQuestRewardedNPC = &onHakkarQuestComplete;
    pNewScript->RegisterSelf();
}
