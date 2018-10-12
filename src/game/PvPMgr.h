#pragma once

class PvPMgr
{
public:
    void Update(uint32 uiDiff);
    std::string GetTopTen() const;
    uint32 GetTextId() const;
    void LoadCustomId();
    void SaveCustomId();
    void UpdateEloRating(Player* winner, Player* loser);
protected:


private:
    std::string generateTopTenText();

    void increaseTextId()
    {
        m_uiTextId++;
    }

    std::string m_topTen;
    uint32 m_uiUpdateTimer = 0;
    uint32 m_uiTextId = 0;
};

#define sPvPMgr MaNGOS::Singleton<PvPMgr>::Instance()