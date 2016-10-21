/* ScriptData
SDName: event_elemental_invasion
SD%Complete: 0
SDComment: Ingame Event, time based, multi-respawn
SDCategory: World
EndScriptData */

/* ContentData
mob_yenniku
EndContentData */
#include "precompiled.h"
#include "../system/system.h"

struct SpawnData
{
    float x;
    float y;
    float z;
    float o;
    uint32 id;
};

enum Fire
{
    NPC_FIRE_BOSS = 14461,
    NPC_FIRE_ELEMENT = 14460,
    GO_FIRE_PORTAL = 179666,
    GO_FIRE_RIFT = 181652,

    SAY_FIRE_SPAWN = -1800012,
    SAY_FIRE_PATROL = -1800013,
    SAY_FIRE_DEATH = -1800014,

    MAX_FIRE_ELEMENTS = 20,
    MAX_FIRE_RIFTS = 5,

    SPELL_FLAMEBUFFET = 9574,
    SPELL_FIREBALL_VOLLEY = 15285
};

static const SpawnData FireElements[MAX_FIRE_ELEMENTS]
{
    { -7247.11, -1515.04, -263.792,	5.28631, 0 }, // 0
    { -7287.91, -1379.01, -271.118,	5.56129, 1 }, // 1
    { -7330.06, -1196.26, -264.576,	6.20492, 2 }, // 2
    { -7120.95, -1106.33, -272.032,	4.06774, 3 }, // 3
    { -7021.83, -1220.72, -269.942,	5.02418, 4 }, // 4
    { -7332.42, -1521.57, -271.993,	0.22251, 5 }, // 0
    { -7280.55, -1461.65, -239.053,	5.79944, 6 }, // 1
    { -7359.25, -1274.49, -269.691,	1.79318, 7 }, // 2
    { -7185.42, -1168.75, -245.561,	0.00000, 8 }, // 3
    { -7092.33, -1161.35, -257.475,	4.79712, 9 }, // 4
    { -7394.65, -1555.74, -274.181,	3.24069, 10 }, // 0
    { -7351.23, -1334.02, -261.605,	2.86978, 11 }, // 1
    { -7311.29, -1260.97, -258.635,	1.41592, 12 }, // 2
    { -7301.81, -1128.43, -270.212,	0.52835, 13 }, // 3
    { -7012.58, -1281.30, -268.221,	0.29712, 14 }, // 4
    { -7345.89, -1489.16, -258.394,	2.48492, 15 }, // 0
    { -7345.81, -1423.28, -267.242,	6.12818, 16 }, // 1
    { -7305.86, -1325.00, -240.584,	4.63849, 17 }, // 2
    { -7195.39, -1113.97, -269.617,	4.02591, 18 }, // 3
    { -7087.97, -1227.53, -256.635,	2.29172, 19 } // 4
};

static const SpawnData FireRifts[MAX_FIRE_RIFTS]
{
    { -7309.862, -1489.183, -259.020, 4.616, 0 },
    { -7315.924, -1401.638, -265.837, 5.943, 1 },
    { -7312.944, -1267.705, -258.924, 2.046, 2 },
    { -7233.052, -1131.563, -259.326, 3.879, 3 },
    { -7060.810, -1221.056, -268.959, 4.221, 4 }
};

enum Earth
{
    NPC_EARTH_BOSS = 14464,
    NPC_EARTH_ELEMENT = 14462,
    GO_EARTH_RIFT = 179664,

    SAY_EARTH_SPAWN = -1800009,
    SAY_EARTH_AGGRO = -1800010,
    SAY_EARTH_DEATH = -1800011,

    MAX_EARTH_ELEMENTS = 15,
    MAX_EARTH_RIFTS = 5,

    SPELL_BOULDER = 23392,
    SPELL_GROUND_TREMOUR = 6524,
    SPELL_TRAMPLE = 5568
};

static const SpawnData EarthElements[MAX_EARTH_ELEMENTS]
{
    { 4563.175, -7202.549, 126.316, 2.306, 0 }, // 0
    { 4695.323, -7117.361,  91.089, 4.839, 1 }, // 1
    { 4760.879, -7354.858,  93.584, 4.646, 2 }, // 2
    { 4664.068, -7412.840,  87.300, 2.011, 3 }, // 3
    { 4544.597, -7388.971,  91.405, 1.336, 4 }, // 4
    { 4543.270, -7144.341, 102.294, 6.276, 5 }, // 0
    { 4688.912, -7181.620,  96.879, 3.001, 6 }, // 1
    { 4672.121, -7354.617,  88.013, 2.459, 7 }, // 2
    { 4733.183, -7462.884,  80.773, 4.222, 8 }, // 3
    { 4494.843, -7360.687,  88.044, 1.627, 9 }, // 4
    { 4535.007, -7239.225, 113.749, 1.269, 10 }, // 0
    { 4791.082, -7225.153,  83.041, 5.475, 11 }, // 1
    { 4598.665, -7397.903,  75.616, 3.068, 12 }, // 2
    { 4648.443, -7466.443,  78.420, 2.730, 13 }, // 3
    { 4550.363, -7290.652, 108.684, 0.668, 14 } // 4
};

static const SpawnData EarthRifts[MAX_EARTH_RIFTS]
{
    { 4539.212, -7162.922, 104.609, 5.839, 0 },
    { 4738.797, -7166.208,  82.199, 3.534, 1 },
    { 4719.237, -7348.103,  69.849, 4.701, 2 },
    { 4672.411, -7428.126,  78.675, 0.907, 3 },
    { 4539.252, -7336.881,  97.823, 1.576, 4 }
};

enum Water
{
    NPC_WATER_BOSS = 14457,
    NPC_WATER_ELEMENT = 14458,
    GO_WATER_PORTAL = 179665,
    GO_WATER_RIFT = 181650,

    SAY_WATER_SPAWN = -1800006,
    SAY_WATER_PATROL = -1800007,
    SAY_WATER_DEATH = -1800008,

    MAX_WATER_ELEMENTS = 15,
    MAX_WATER_RIFTS = 4,

    SPELL_CONE_OF_COLD = 22746,
    SPELL_FROST_NOVA = 14907,
    SPELL_FROSTBOLT = 21369,
    SPELL_GEYSER = 10987
};

static const SpawnData WaterElements[MAX_WATER_ELEMENTS]
{
    { 6471.33, -3979.88, 658.408, 0.7081, 0 }, // 0
    { 6596.17, -4117.68, 658.408, 5.7022, 1 }, // 1
    { 6549.48, -4238.61, 658.408, 1.3365, 2 }, // 2
    { 6488.49, -4132.46, 658.658, 3.7991, 3 },
    { 6489.79, -3940.59, 658.811, 4.9618, 4 }, // 0
    { 6624.57, -4147.06, 658.661, 4.8082, 5 }, // 1
    { 6558.33, -4191.67, 658.561, 5.4308, 6 }, // 2
    { 6529.84, -4106.39, 662.496, 4.8258, 7 },
    { 6518.35, -3978.55, 658.408, 1.9493, 8 }, //0
    { 6588.08, -4148.49, 658.412, 0.3522, 9 }, // 1
    { 6510.32, -4225.75, 658.866, 4.2865, 10 }, // 2
    { 6511.11, -4164.21, 658.658, 1.1062, 11 },
    { 6457.31, -4025.85, 658.379, 5.7095, 12 }, // 0
    { 6572.01, -4092.23, 658.408, 5.1901, 13 }, // 1
    { 6477.94, -4190.74, 658.585, 3.6867, 14 } // 2
};

static const SpawnData WaterRifts[MAX_WATER_RIFTS]
{
    { 6495.74, -3959.88, 659.03, 0.0f, 0 },
    { 6583.39, -4107.20, 658.35, 0.0f, 1 },
    { 6525.83, -4200.67, 658.52, 0.0f, 2 },
    { 6488.49, -4132.46, 658.65, 0.0f, 3 }
};

enum Air
{
    NPC_AIR_BOSS = 14454,
    NPC_AIR_ELEMENT = 14455,
    GO_AIR_PORTAL = 179667,
    GO_AIR_RIFT = 181651,

    SAY_AIR_SPAWN = -1800015,

    MAX_AIR_ELEMENTS = 19,
    MAX_AIR_RIFTS = 5,

    SPELL_CHAIN_LIGHTNING = 23106,
    SPELL_ENVELOPING_WINDS = 23103,
    SPELL_LIGHTNING_CLOUD = 23105,
    SPELL_SHOCK = 23104
};

static const SpawnData AirElements[MAX_AIR_ELEMENTS]
{
    {	-6585.60, 1484.22, 1.70683,	0.89804, 0	}, 
    {	-6422.83, 1449.37, -1.2419,	2.05490, 1	}, 
    {	-6359.24, 1387.87, 3.22410,	4.14147, 2 }, 
    {	-6351.52, 1446.28, 3.06546,	5.34808, 3 }, 
    {	-6276.35, 1507.35, 8.31717,	5.61645, 4 },
    {	-6551.15, 1522.68, 2.48736,	3.18851, 5	},
    {	-6388.69, 1488.35, 3.97523,	1.79615, 6	},
    {	-6318.51, 1345.91, 4.74018,	4.07670, 7	}, 
    {	-6356.89, 1516.24, 4.55574,	1.16308, 8	}, 
    {	-6287.66, 1453.90, 9.22652,	2.42662, 9 },
    {	-6542.88, 1439.42, 3.76747,	2.18280, 10	}, 
    {	-6424.80, 1520.72, 8.71915,	3.28073, 11	}, 
    {	-6279.99, 1379.50, 8.86034,	2.77161, 12 }, 
    {   -6387.24, 1425.19, 4.40161,	4.73470, 13 },
    {	-6317.45, 1549.88, 1.76343,	5.57594, 14 },
    {	-6618.98, 1516.43, 3.82220,	2.45010, 15	}, 
    {	-6487.20, 1514.47, 4.56502,	4.40158, 16	}, 
    {	-6250.34, 1345.46, 16.3242,	5.86496, 17	}, 
    {	-6317.01, 1485.68, 3.96418,	3.73830, 18 }
};

static const SpawnData AirRifts[MAX_AIR_RIFTS]
{
    { -6565.017, 1493.648, 2.422, 2.477, 0 },
    { -6445.457, 1484.902, 6.334, 0.209, 1 },
    { -6317.990, 1380.148, 4.953, 0.233, 2 },
    { -6358.628, 1476.956, 6.341, 5.514, 3 },
    { -6318.573, 1522.038, 3.082, 2.860, 4 }
};

enum Timer // TODO: set correct timers
{
    ELEMENT_SPAWN_TIMER = 1 * MINUTE*IN_MILLISECONDS / 2,
    ELEMENT_RESPAWN_TIMER = 1 * MINUTE*IN_MILLISECONDS / 4,
    SAY_PATROL_TIMER = 2 * MINUTE*IN_MILLISECONDS,
    INVASION_SPAWN_TIMER_MIN = 2 * MINUTE,
    INVASION_SPAWN_TIMER_MAX = 3 * MINUTE
};

struct mob_elementary_bossAI : public ScriptedAI
{
    uint32 m_uiSayPatrolTimer = SAY_PATROL_TIMER;
    uint32 m_uiSpawnTimer = ELEMENT_SPAWN_TIMER;
    uint32 m_uiSpawnIndex = 0;

    std::map<uint32, ObjectGuid> guidlist;
    std::map<uint32, uint32> respawnList;

    uint32 MAX_RIFTS = 0;
    uint32 MAX_ELEMENTS = 0;
    uint32 SAY_DEATH = 0;
    uint32 NPC_ELEMENT = 0;
    uint32 GO_RIFT = 0;
    uint32 SAY_SPAWN = 0;
    uint32 SAY_PATROL = 0;
    
    GameObject* gObjects[10];
    bool areObjectsSpawned = false;
    
    mob_elementary_bossAI(Creature* pCreature, uint32 maxRifts, uint32  maxElements, uint32 rift, uint32 element, uint32 sayDeath, uint32 sayPatrol, uint32 saySpawn) : ScriptedAI(pCreature)
    {
       MAX_RIFTS = maxRifts;
       MAX_ELEMENTS = maxElements;
       SAY_DEATH = sayDeath;
       NPC_ELEMENT = element;
       GO_RIFT = rift;
       SAY_SPAWN = saySpawn;
       SAY_PATROL = sayPatrol;
       m_creature->SetActiveObjectState(true);
    }

    virtual void UpdateElementAI(const uint32) { ; }

    void JustRespawned()
    {
        if (guidlist.size() == 0)
        {
            areObjectsSpawned = false;
            m_uiSpawnIndex = 0;
        }
        m_uiSayPatrolTimer = SAY_PATROL_TIMER;
    }

    void JustDied(Unit* pKiller)
    {
        respawnList.clear();
        guidlist.clear();
        for (uint32 i = 0; i < MAX_RIFTS; i++)
        {
            gObjects[i]->RemoveFromWorld();
            gObjects[i] = nullptr;
        }
        DoScriptText(SAY_DEATH, m_creature);
        uint32 spawntime = urand(INVASION_SPAWN_TIMER_MIN, INVASION_SPAWN_TIMER_MAX);
        m_creature->SetRespawnDelay(spawntime);
        m_creature->SaveRespawnTime();
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        if (m_creature->isDead())
            return;

        for (std::map<uint32, ObjectGuid>::iterator itr = guidlist.begin(); itr != guidlist.end(); itr++)
        {
            if (pSummoned->GetObjectGuid() == itr->second)
            {
                respawnList[itr->first] = ELEMENT_RESPAWN_TIMER;
                guidlist[itr->first] = ObjectGuid();
                return;
            }
        }
    }

    virtual void MovementInform(uint32 /*uiMovementType*/, uint32 /*uiData*/)
    {
        CreatureCreatePos pos(m_creature->GetMap(), m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation());
        m_creature->SetRespawnCoord(pos);
        m_creature->SaveToDB();
    }

    void DoSpawnElement(uint32 index)
    {
        SpawnData entry = FindSpawnPosition(index);
        SpawnData targetPosition = FindTargetPosition(index);
        Creature* creature = m_creature->SummonCreature(NPC_ELEMENT, entry.x, entry.y, entry.z, entry.o, TEMPSUMMON_DEAD_DESPAWN, 0);
        creature->GetMotionMaster()->MoveRandomAroundPoint(targetPosition.x, targetPosition.y, targetPosition.z, 5.0f);
        CreatureCreatePos pos(m_creature->GetMap(), targetPosition.x, targetPosition.y, targetPosition.z, targetPosition.o);
        creature->SetRespawnCoord(pos);
        guidlist[index] = creature->GetObjectGuid();
    }

    virtual SpawnData FindSpawnPosition(uint32 creatureSpawnId) { return { 0.0f, 0.0f, 0.0f, 0.0f, 0 }; }

    virtual SpawnData FindTargetPosition(uint32 creatureSpawnId) { return { 0.0f, 0.0f, 0.0f, 0.0f, 0 }; }
    
    void UpdateAI(uint32 uiDiff)
    {
        InitiallySpawnRifts();
        SpawnElements(uiDiff);
        RespawnElements(uiDiff);

        if (m_uiSayPatrolTimer <= uiDiff)
        {
            DoScriptText(SAY_PATROL, m_creature);
            m_uiSayPatrolTimer = SAY_PATROL_TIMER;
        }
        else
            m_uiSayPatrolTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        UpdateElementAI(uiDiff);
        DoMeleeAttackIfReady();
    }

    void InitiallySpawnRifts()
    {
        if (!areObjectsSpawned)
        {
            for (uint32 i = 0; i < MAX_RIFTS; i++)
            {
                SpawnData position = FindSpawnPosition(i);
                GameObject* go = m_creature->SummonGameObject(GO_RIFT, position.x, position.y, position.z, 0, 172800);
                gObjects[i] = go;
            }
            areObjectsSpawned = true;
            DoScriptText(SAY_SPAWN, m_creature);
        }
    }

    void SpawnElements(uint32 uiDiff)
    {
        if (m_uiSpawnTimer <= uiDiff)
        {
            if (m_uiSpawnIndex < MAX_ELEMENTS)
            {
                DoSpawnElement(m_uiSpawnIndex);
                m_uiSpawnIndex++;
            }
            m_uiSpawnTimer = ELEMENT_SPAWN_TIMER;
        }
        else
            m_uiSpawnTimer -= uiDiff;
    }

    void RespawnElements(uint32 uiDiff)
    {
        if (respawnList.size() == 0)
            return;
        
        for (std::map<uint32, uint32>::iterator itr = respawnList.begin(); itr != respawnList.end(); itr++)
        {
            if (itr->second < uiDiff)
            {
                DoSpawnElement(itr->first);
                respawnList.erase(itr->first);
            }
            else
                itr->second -= uiDiff;
        }
    }
};

struct mob_boss_fireAI : public mob_elementary_bossAI
{
    mob_boss_fireAI(Creature* pCreature) : mob_elementary_bossAI(pCreature, MAX_FIRE_RIFTS, MAX_FIRE_ELEMENTS,GO_FIRE_RIFT,NPC_FIRE_ELEMENT,SAY_FIRE_DEATH, SAY_FIRE_PATROL, SAY_FIRE_SPAWN)
    {
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    uint32 m_uiFlameBuffetTimer;
    uint32 m_uiFireballVolleyTimer;

    void Reset() override
    {
        m_uiFlameBuffetTimer = 5000;
        m_uiFireballVolleyTimer = 10000;
    }

    SpawnData FindSpawnPosition(uint32 creatureSpawnId)
    {
        return FireRifts[creatureSpawnId % MAX_RIFTS];
    }

    SpawnData FindTargetPosition(uint32 creatureSpawnId)
    {
        return FireElements[creatureSpawnId];
    }

    void UpdateElementAI(uint32 uiDiff)
    {
        // Combat
        if (m_uiFlameBuffetTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FLAMEBUFFET) == CAST_OK)
                m_uiFlameBuffetTimer = urand(6000, 10000);
        }
        else
            m_uiFlameBuffetTimer -= uiDiff;

        if (m_uiFireballVolleyTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FIREBALL_VOLLEY) == CAST_OK)
                m_uiFireballVolleyTimer = urand(8000, 12000);
        }
        else
            m_uiFireballVolleyTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_fire(Creature* pCreature)
{
    return new mob_boss_fireAI(pCreature);
}

struct mob_boss_earthAI : public mob_elementary_bossAI
{
    mob_boss_earthAI(Creature* pCreature) : mob_elementary_bossAI(pCreature, MAX_EARTH_RIFTS, MAX_EARTH_ELEMENTS, GO_EARTH_RIFT, NPC_EARTH_ELEMENT, SAY_EARTH_DEATH, 0, SAY_EARTH_SPAWN)
    {
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    uint32 m_uiBoulderTimer;
    uint32 m_uiGroundTremorTimer;
    uint32 m_uiTrampleTimer;

    void Reset() override
    {
        m_uiBoulderTimer = 5000;
        m_uiGroundTremorTimer = 15000;
        m_uiTrampleTimer = 8000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_EARTH_AGGRO, m_creature);
    }

    SpawnData FindSpawnPosition(uint32 creatureSpawnId)
    {
        return EarthRifts[creatureSpawnId % MAX_RIFTS];
    }

    SpawnData FindTargetPosition(uint32 creatureSpawnId)
    {
        return EarthElements[creatureSpawnId];
    }

    void UpdateElementAI(uint32 uiDiff)
    {
        if (m_uiBoulderTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BOULDER) == CAST_OK)
                m_uiBoulderTimer = urand(6000, 10000);
        }
        else
            m_uiBoulderTimer -= uiDiff;

        if (m_uiGroundTremorTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_GROUND_TREMOUR) == CAST_OK)
                m_uiGroundTremorTimer = urand(15000, 20000);
        }
        else
            m_uiGroundTremorTimer -= uiDiff;

        if (m_uiTrampleTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_TRAMPLE) == CAST_OK)
                m_uiTrampleTimer = urand(10000, 15000);
        }
        else
            m_uiTrampleTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_earth(Creature* pCreature)
{
    return new mob_boss_earthAI(pCreature);
}

struct mob_boss_airAI : public mob_elementary_bossAI
{
    mob_boss_airAI(Creature* pCreature) : mob_elementary_bossAI(pCreature, MAX_AIR_RIFTS, MAX_AIR_ELEMENTS, GO_AIR_RIFT, NPC_AIR_ELEMENT, 0, 0, SAY_AIR_SPAWN)
    {
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    uint32 m_uiShockTimer;
    uint32 m_uiLightningCloudTimer;
    uint32 m_uiEnvelopingWindsTimer;
    uint32 m_uiChainLightingTimer;

    void Reset() override
    {
        m_uiShockTimer = 5000;
        m_uiLightningCloudTimer = 15000;
        m_uiEnvelopingWindsTimer = 20000;
        m_uiChainLightingTimer = 10000;
    }

    SpawnData FindSpawnPosition(uint32 creatureSpawnId)
    {
        return AirRifts[creatureSpawnId % MAX_RIFTS];
    }
    
    SpawnData FindTargetPosition(uint32 creatureSpawnId)
    {
        return AirElements[creatureSpawnId];
    }

    void UpdateElementAI(uint32 uiDiff)
    {
        if (m_uiShockTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_SHOCK) == CAST_OK)
                m_uiShockTimer = urand(6000, 10000);
        }
        else
            m_uiShockTimer -= uiDiff;

        if (m_uiLightningCloudTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_LIGHTNING_CLOUD) == CAST_OK)
                m_uiLightningCloudTimer = urand(10000, 15000);
        }
        else
            m_uiLightningCloudTimer -= uiDiff;

        if (m_uiEnvelopingWindsTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ENVELOPING_WINDS) == CAST_OK)
                m_uiEnvelopingWindsTimer = urand(15000, 20000);
        }
        else
            m_uiEnvelopingWindsTimer -= uiDiff;

        if (m_uiChainLightingTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CHAIN_LIGHTNING) == CAST_OK)
                m_uiChainLightingTimer = urand(8000, 20000);
        }
        else
            m_uiChainLightingTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_air(Creature* pCreature)
{
    return new mob_boss_airAI(pCreature);
}

struct mob_boss_waterAI : public mob_elementary_bossAI
{
    mob_boss_waterAI(Creature* pCreature) : mob_elementary_bossAI(pCreature, MAX_WATER_RIFTS, MAX_WATER_ELEMENTS, GO_WATER_RIFT, NPC_WATER_ELEMENT, SAY_WATER_DEATH, SAY_WATER_PATROL, SAY_WATER_SPAWN)
    {
        m_creature->SetActiveObjectState(true);
        Reset();
    }

    uint32 m_uiConeOfColdTimer;
    uint32 m_uiFrostNovaTimer;
    uint32 m_uiFrostboltTimer;
    uint32 m_uiGeyserTimer;

    void Reset() override
    {
        m_uiConeOfColdTimer = 5000;
        m_uiFrostNovaTimer = 10000;
        m_uiFrostboltTimer = 1000;
        m_uiGeyserTimer = 15000;
    }

    SpawnData FindSpawnPosition(uint32 creatureSpawnId)
    {
        return WaterRifts[creatureSpawnId % MAX_RIFTS];
    }

    SpawnData FindTargetPosition(uint32 creatureSpawnId)
    {
        return WaterElements[creatureSpawnId];
    }

    void UpdateElementAI(uint32 uiDiff)
    {
        if (m_uiFrostboltTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROSTBOLT) == CAST_OK)
                m_uiFrostboltTimer = urand(4000, 10000);
        }
        else
            m_uiFrostboltTimer -= uiDiff;

        if (m_uiFrostNovaTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FROST_NOVA) == CAST_OK)
                m_uiFrostNovaTimer = urand(10000, 15000);
        }
        else
            m_uiFrostNovaTimer -= uiDiff;

        if (m_uiConeOfColdTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CONE_OF_COLD) == CAST_OK)
                m_uiConeOfColdTimer = urand(15000, 20000);
        }
        else
            m_uiConeOfColdTimer -= uiDiff;

        if (m_uiGeyserTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_GEYSER) == CAST_OK)
                m_uiGeyserTimer = urand(20000, 30000);
        }
        else
            m_uiGeyserTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_water(Creature* pCreature)
{
    return new mob_boss_waterAI(pCreature);
}

void AddSC_elemental_invasion()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_water";
    pNewScript->GetAI = &GetAI_boss_water;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_fire";
    pNewScript->GetAI = &GetAI_boss_fire;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_earth";
    pNewScript->GetAI = &GetAI_boss_earth;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_air";
    pNewScript->GetAI = &GetAI_boss_air;
    pNewScript->RegisterSelf();

  /*  pNewScript = new Script;
    pNewScript->Name = "mob_elemental_invasion_spawn";
    pNewScript->GetAI = &GetAI_elemental_invasion_spawn;
    pNewScript->RegisterSelf();*/
}