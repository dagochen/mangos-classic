enum class RaidStatsUnitType : uint32
{
    NONE     = 0,
    PLAYER   = 1,
    PET      = 2,
    CREATURE = 3,
    GAMEOBJECT = 4,
};

enum class RaidStatsEvent : uint32
{
    AUTOATTACK    = 0,
    SPELL         = 1,
    HEAL          = 2,
    DISPEL        = 3,
    COMBAT_START  = 4,
    KILL          = 5,
    EVADE         = 6,
    LOOT          = 7,
    LOOT_PICK     = 8,
    COMBAT_STOP   = 9,
};

struct RaidStatsData
{
    RaidStatsData(RaidStatsEvent e, uint32 id, uint32 z) : raidEvent(e), raidId(id), zone(z) {
        SYSTEMTIME time;
        GetLocalTime(&time);
        // '2014-09-08 17:51:04.777'
        std::stringstream zeit;

        zeit << time.wYear << "-" << time.wMonth << "-" << time.wDay << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << "." << time.wMilliseconds;
        timestamp = zeit.str();
    }

    RaidStatsEvent raidEvent;
    uint32 raidId;
    std::string timestamp;
    uint32 zone;

    union
    {
        struct
        {
            ObjectGuid attacker;
            ObjectGuid target;
            uint32 damage;
            MeleeHitOutcome hitoutcome;
            uint32 attackType;
        } autoattack;

        struct
        {
            ObjectGuid caster;
            ObjectGuid target;
            uint32 damage;
            uint32 spellId;
            uint32 hitresult;
            uint32 missInfo;
            uint32 resist;
            uint32 absorb;
            uint32 blocked;
            uint32 school;
            uint32 resistance;
            bool isOverTime;
        } spells;

        struct
        {
            ObjectGuid healer;
            ObjectGuid target;
            uint32 efficientHeal;
            uint32 overHeal;
            uint32 rawHeal;
            uint32 spellId;
            bool isOverTime;
        } heal;

        struct
        {
            ObjectGuid caster;
            ObjectGuid target;
            uint32 debuff;
            uint32 dispelType;
            uint32 spellId;
        } dispel;

        struct
        {
            ObjectGuid attacker;
            ObjectGuid victim;
        } combatStart;

        struct
        {
            uint32 killer;
            uint32 victim;
            uint32 killerType;
            uint32 victimType;
        } kill;
        struct
        {
            uint32 unit;
        } evade;

        struct
        {
            uint32 source;
            uint32 sourceType;
            uint32 gold;
            uint32 item[10];
        } lootdrop;

        struct
        {
            uint32 source;
            uint32 sourceType;
            uint32 looter;
            uint32 item;
        } lootpickup;
       
        struct
        {
            uint32 playerGuid;
            uint32 bossGuid;
        } combatStop;
    };
};


class RaidStatsMgr
{
public:
    RaidStatsMgr() : m_kill(true), m_spell(true), m_autoattack(true), m_evade(true), m_heal(true), m_loot(true), m_lootpick(true), m_combat(true) {}
    ~RaidStatsMgr() {}


    void AddRaidEvent(RaidStatsData& e);

    void WriteAll();
    void Update(uint32 diff);
    bool CanTrackThis(Unit * a, Unit * b);
    bool IsTrackingEnabled(RaidStatsEvent e, Unit* a, WorldObject* b);

    void SetAutoattack(bool val) { m_autoattack = val; };
    void SetSpell(bool val) { m_spell = val; };
    void SetHeal(bool val) { m_heal = val; };
    void SetEvade(bool val) { m_evade = val; };
    void SetCombat(bool val) { m_combat = val; };
    void SetLoot(bool val) { m_loot = val; };
    void SetLootpick(bool val) { m_lootpick = val; };
    void SetKill(bool val) { m_kill = val; };

    bool IsAutoattackEnabled() { return m_autoattack; };
    bool IsSpellEnabled() { return m_spell; };
    bool IsHealEnabled() { return m_heal; };
    bool IsCombatEnabled() { return m_combat; };
    bool IsEvadeEnabled() { return m_evade; };
    bool IsLootEnabled() { return m_loot; };
    bool IsLootpickEnabled() { return m_lootpick; };
    bool IsKillEnabled() { return m_kill; };


    RaidStatsUnitType GetRaidStatsType(WorldObject * o);
    RaidStatsUnitType GetRaidStatsType(ObjectGuid o);
    uint32 GetId(WorldObject* u);
    uint32 GetId(ObjectGuid guid);


private:
    typedef std::vector<RaidStatsData> RaidStatsDataList;

    RaidStatsDataList raidStats;

    uint32 m_saveIntervallTimer;

    bool m_autoattack;
    bool m_spell;
    bool m_heal;
    bool m_combat;
    bool m_evade;
    bool m_loot;
    bool m_lootpick;
    bool m_kill;

};

#define sRaidStatsMgr MaNGOS::Singleton<RaidStatsMgr>::Instance()
