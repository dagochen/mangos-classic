#include "World.h"
#include "RaidStatsMgr.h"
#include "Language.h"

void RaidStatsMgr::AddRaidEvent(RaidStatsData& e)
{
    raidStats.emplace_back(e);
}



void RaidStatsMgr::WriteAll()
{

    if (raidStats.empty())
        return;

    static SqlStatementID insAutoattack;
    static SqlStatementID insSpell;
    static SqlStatementID insHeal;
    static SqlStatementID insLoot;
    static SqlStatementID insCombatStart;
    static SqlStatementID insUnitDied;
    static SqlStatementID insEvade;
    static SqlStatementID insLootPick;
    static SqlStatementID insCombatStop;

    CharacterDatabase.BeginTransaction();
    
    for each (RaidStatsData current in raidStats)
    {
        switch (current.raidEvent)
        {
            case RaidStatsEvent::AUTOATTACK:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(insAutoattack, "INSERT INTO raidstats_autoattack (raidId,zoneId,timestamp,attackerGUID,targetGUID,damage,attackType,hitoutcome,attackerType,targetType) VALUES (?,?, ?, ?, ?, ?, ?, ?, ?, ?)");
                stmt.addUInt32(current.raidId);
                stmt.addUInt32(current.zone);
                stmt.addString(current.timestamp);
                stmt.addUInt32(GetId(current.autoattack.attacker));
                stmt.addUInt32(GetId(current.autoattack.target));
                stmt.addUInt32(current.autoattack.damage);
                stmt.addUInt32(current.autoattack.attackType);
                stmt.addUInt32(current.autoattack.hitoutcome);
                stmt.addUInt32(static_cast<uint32>(GetRaidStatsType(current.autoattack.attacker)));
                stmt.addUInt32(static_cast<uint32>(GetRaidStatsType(current.autoattack.target)));
                stmt.Execute();
                break;
            }
            case RaidStatsEvent::SPELL:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(insSpell, "INSERT INTO raidstats_spell (raidId,zoneId,timestamp,casterGUID,targetGUID,spellId,damage,absorb,resist,blocked,casterType,targetType, hitresult, missinfo, resistance, school, isOverTime) VALUES (?,?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                stmt.addUInt32(current.raidId);
                stmt.addUInt32(current.zone);
                stmt.addString(current.timestamp);
                stmt.addUInt32(GetId(current.spells.caster));
                stmt.addUInt32(GetId(current.spells.target));
                stmt.addUInt32(current.spells.spellId);
                stmt.addUInt32(current.spells.damage);
                stmt.addUInt32(current.spells.absorb);
                stmt.addUInt32(current.spells.resist);
                stmt.addUInt32(current.spells.blocked);
                stmt.addUInt32(static_cast<uint32>(GetRaidStatsType(current.spells.caster)));
                stmt.addUInt32(static_cast<uint32>(GetRaidStatsType(current.spells.target)));
                stmt.addUInt32(current.spells.hitresult);
                stmt.addUInt32(current.spells.missInfo);
                stmt.addUInt32(current.spells.resistance);
                stmt.addUInt32(current.spells.school);
                stmt.addBool(current.spells.isOverTime);
                stmt.Execute();
                break;
            }
            case RaidStatsEvent::HEAL:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(insHeal, "INSERT INTO raidstats_heal (raidId,zone,timestamp,healerGUID,targetGUID,spellId,healerType,targetType,rawheal,overheal,efficientheal, isOverTime) VALUES (?,?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                stmt.addUInt32(current.raidId);
                stmt.addUInt32(current.zone);
                stmt.addString(current.timestamp);
                stmt.addUInt32(GetId(current.heal.healer));
                stmt.addUInt32(GetId(current.heal.target));
                stmt.addUInt32(current.heal.spellId);
                stmt.addUInt32(static_cast<uint32>(GetRaidStatsType(current.heal.healer)));
                stmt.addUInt32(static_cast<uint32>(GetRaidStatsType(current.heal.target)));
                stmt.addUInt32(current.heal.rawHeal);
                stmt.addUInt32(current.heal.overHeal);
                stmt.addUInt32(current.heal.efficientHeal);
                stmt.addBool(current.heal.isOverTime);
                stmt.Execute();
                break;
            }
            case RaidStatsEvent::DISPEL:
                break;
            case RaidStatsEvent::COMBAT_START:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(insCombatStart, "INSERT INTO raidstats_combat (raidId,zone,timestamp,attackerGUID,victimGUID,attackerType,victimType) VALUES (?,?, ?, ?, ?, ?, ?)");
                stmt.addUInt32(current.raidId);
                stmt.addUInt32(current.zone);
                stmt.addString(current.timestamp);
                stmt.addUInt32(GetId(current.combatStart.attacker));
                stmt.addUInt32(GetId(current.combatStart.victim));
                stmt.addUInt32(static_cast<uint32>(GetRaidStatsType(current.combatStart.attacker)));
                stmt.addUInt32(static_cast<uint32>(GetRaidStatsType(current.combatStart.victim)));
                stmt.Execute();
                break;
            }
            case RaidStatsEvent::KILL:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(insUnitDied, "INSERT INTO raidstats_kill (raidId,zone,timestamp,killerGUID,victimGUID,killerType,victimType) VALUES (?,?, ?, ?, ?, ?, ?)");
                stmt.addUInt32(current.raidId);
                stmt.addUInt32(current.zone);
                stmt.addString(current.timestamp);
                stmt.addUInt32(current.kill.killer);
                stmt.addUInt32(current.kill.victim);
                stmt.addUInt32(current.kill.killerType);
                stmt.addUInt32(current.kill.victimType);
                stmt.Execute();
                break;
            }
            case RaidStatsEvent::EVADE:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(insEvade, "INSERT INTO raidstats_evade (raidid,zone,timestamp,unitGUID) VALUES (?,?,?,?)");
                stmt.addUInt32(current.raidId);
                stmt.addUInt32(current.zone);
                stmt.addString(current.timestamp);
                stmt.addUInt32(current.evade.unit);
                stmt.Execute();
                break;
            }
            case RaidStatsEvent::LOOT:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(insLoot, "INSERT INTO raidstats_loot (raidId,zone,timestamp,sourceGUID,sourceType,gold,loot1,loot2,loot3,loot4,loot5,loot6,loot7,loot8,loot9,loot10) VALUES (?, ?,?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                stmt.addUInt32(current.raidId);
                stmt.addUInt32(current.zone);
                stmt.addString(current.timestamp);
                stmt.addUInt32(current.lootdrop.source);
                stmt.addUInt32(current.lootdrop.sourceType);
                stmt.addUInt32(current.lootdrop.gold);
                for (uint32 i = 0; i < 10; i++)
                {
                    stmt.addUInt32(current.lootdrop.item[i]);
                }
                stmt.Execute();
                break;
            }
            case RaidStatsEvent::LOOT_PICK:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(insLootPick, "INSERT INTO raidstats_lootpick (raidId,zone,timestamp,looterGUID,sourceGUID,sourceType,loot) VALUES (?,?,?,?,?,?,?)");
                stmt.addUInt32(current.raidId);
                stmt.addUInt32(current.zone);
                stmt.addString(current.timestamp);
                stmt.addUInt32(current.lootpickup.looter);
                stmt.addUInt32(current.lootpickup.source);
                stmt.addUInt32(current.lootpickup.sourceType);
                stmt.addUInt32(current.lootpickup.item);
                stmt.Execute();
                break;
            }
            case RaidStatsEvent::COMBAT_STOP:
            {
                SqlStatement stmt = CharacterDatabase.CreateStatement(insCombatStop, "INSERT INTO raidstats_combatstop (raidId,zone,timestamp,bossGuid,playerGuid) VALUES (?,?,?,?,?)");
                stmt.addUInt32(current.raidId);
                stmt.addUInt32(current.zone);
                stmt.addString(current.timestamp);
                stmt.addUInt32(current.combatStop.bossGuid);
                stmt.addUInt32(current.combatStop.playerGuid);
                stmt.Execute();
            }
        default:
            break;
        }
    }
    if (CharacterDatabase.CommitTransaction())
    {
        sLog.outError("RaidStatCount is : %i", raidStats.size());
        std::stringstream out;

        //put arbitrary formatted data into the stream
        out << "RaidStatCount is : " << raidStats.size();
        //convert the stream buffer into a string
        std::string ausgabe = out.str();
        sWorld.SendWorldText(LANG_SYSTEMMESSAGE, ausgabe.c_str());
        raidStats.clear();
    }

}

uint32 RaidStatsMgr::GetId(WorldObject* u)
{
    switch (u->GetObjectGuid().GetHigh())
    {
        case HIGHGUID_PET:
        {
            return ((Unit*)u)->GetCharm()->GetCharmInfo()->GetPetNumber();
        }
        case HIGHGUID_GAMEOBJECT:
        case HIGHGUID_DYNAMICOBJECT:
            return u->GetObjectGuid().GetCounter();
        case HIGHGUID_UNIT:
        case HIGHGUID_PLAYER:
        {
            return u->GetObjectGuid().GetCounter();
        }
    }

    return 0;
}

uint32 RaidStatsMgr::GetId(ObjectGuid guid)
{
    switch (guid.GetHigh())
    {
        case HIGHGUID_PET:
        {
            return 0;
        }
        case HIGHGUID_GAMEOBJECT:
        case HIGHGUID_DYNAMICOBJECT:
        case HIGHGUID_UNIT:
        case HIGHGUID_PLAYER:
        {
            return guid.GetCounter();
        }
    }

    return 0;
}


RaidStatsUnitType RaidStatsMgr::GetRaidStatsType(WorldObject* o)
{
    return (GetRaidStatsType(o->GetObjectGuid()));
}

RaidStatsUnitType RaidStatsMgr::GetRaidStatsType(ObjectGuid o)
{
    if (o.IsPlayer())
        return RaidStatsUnitType::PLAYER;
    else if (o.IsCreature())
        return RaidStatsUnitType::CREATURE;
    else if (o.IsPet())
        return RaidStatsUnitType::PET;
    else if (o.IsGameObject())
        return RaidStatsUnitType::GAMEOBJECT;
    else 
        return RaidStatsUnitType::NONE;
}

void RaidStatsMgr::Update(uint32 diff)
{
    if (m_saveIntervallTimer <= diff || raidStats.size() > 5000)
    {
        WriteAll();
        m_saveIntervallTimer = 10000;
    }
    else
        m_saveIntervallTimer -= diff;

}

bool RaidStatsMgr::CanTrackThis(Unit* a, Unit* b)
{
    if ((a && a->IsInBossfight()) || (b && b->IsInBossfight()))
        return true;

    bool playerInvolved = false;
    bool bossInvolved = false;

    
    if (a)
    {
        Player* pA = a->GetCharmerOrOwnerPlayerOrPlayerItself();

        if (pA)
            playerInvolved = true;

        if (a->GetObjectGuid().IsCreature() && ((Creature*)a)->GetCreatureInfo()->Rank == 3)
            bossInvolved = true;
    }
   
    if (b)
    {
        Player* pB = b->GetCharmerOrOwnerPlayerOrPlayerItself();

        if (pB)
            playerInvolved = true;

        if (b && b->GetObjectGuid().IsCreature() && ((Creature*)b)->GetCreatureInfo()->Rank == 3)
            bossInvolved = true;
    }

    if ((playerInvolved && bossInvolved) || bossInvolved)
        return true;

    return false;
}

bool RaidStatsMgr::IsTrackingEnabled(RaidStatsEvent e, Unit* a, WorldObject* b)
{
    // TODO: remove
    return false; 
    switch (e)
    {
        case RaidStatsEvent::AUTOATTACK:
        {
            if (!IsAutoattackEnabled())
                return false;

            return CanTrackThis(a, (Unit*)b);
        }
        case RaidStatsEvent::SPELL:
        { 
            if (!IsSpellEnabled())
                return false;

            return CanTrackThis(a, (Unit*)b);
        }
        case RaidStatsEvent::HEAL:
        {  
            if (!IsHealEnabled())
                return false;

            return CanTrackThis(a, (Unit*)b);
        }
        case RaidStatsEvent::KILL:
        {   
            if (!IsKillEnabled())
                return false;

            return CanTrackThis(a, (Unit*)b);
        }
        case RaidStatsEvent::DISPEL:
        {
            return false;
        }
        case RaidStatsEvent::COMBAT_START:
        {
            if (!IsCombatEnabled())
                return false;

            return CanTrackThis(a, (Unit*)b);
            break;
        }
        case RaidStatsEvent::EVADE:
        {
            if (!IsEvadeEnabled())
                return false;

            return CanTrackThis(a, (Unit*)b);
        }
        case RaidStatsEvent::LOOT:
        {
            if (!IsLootEnabled())
                return false;

            if (a->GetMap()->IsRaid() || ((Creature*)a)->GetCreatureInfo()->Rank == 3)
                return true;
        }
        case RaidStatsEvent::LOOT_PICK:
        {
            if (!IsLootpickEnabled())
                return false;

            if (a->GetMap()->IsRaid())
                return true;
        }
        case RaidStatsEvent::COMBAT_STOP:
        {
            if (!IsCombatEnabled())
                return false;

            if (a->GetTypeId() == TYPEID_PLAYER && a->IsInBossfight())
                return true;
        }
    default:
        break;
    }
   
    return false;
}


