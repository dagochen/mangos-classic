#include "pchdef.h"
#include "PvPMgr.h"
#include "Database/DatabaseEnv.h"
#include "Language.h"


void PvPMgr::Update(uint32 uiDiff)
{
    if (m_uiUpdateTimer <= uiDiff)
    {
        m_topTen = generateTopTenText();
        increaseTextId();
        SaveCustomId();
        m_uiUpdateTimer = IN_MILLISECONDS * MINUTE; //* 15;
    }
    else
        m_uiUpdateTimer -= uiDiff;
}

std::string PvPMgr::GetTopTen() const
{
    return m_topTen;
}

uint32 PvPMgr::GetTextId() const
{
    return m_uiTextId;
}


//ALTER TABLE `characters`.`saved_variables`
//ADD COLUMN `pvp_topTen_id` INT(11) DEFAULT 60000 NULL AFTER `cleaning_flags`;

void PvPMgr::LoadCustomId()
{
    QueryResult* result = CharacterDatabase.PQuery("SELECT pvp_topTen_id FROM saved_variables");
    if (!result)
        return;
    m_uiTextId = (*result)[0].GetUInt32();
    delete result;
}


void PvPMgr::SaveCustomId()
{
    CharacterDatabase.PExecute("UPDATE saved_variables SET pvp_topTen_id = %u", m_uiTextId);
}

/*ALTER TABLE `characters`.`characters`
ADD COLUMN `elorating` float(0)  DEFAULT 1000  NOT NULL AFTER `lastPvPBonus`;
*/

void PvPMgr::UpdateEloRating(Player* winner, Player* loser)
{
    // Avoid playing against self
    if (winner->GetSession()->GetRemoteAddress() == loser->GetSession()->GetRemoteAddress() && winner->GetSession()->GetSecurity() == SEC_PLAYER && loser->GetSession()->GetSecurity() == SEC_PLAYER)
        return;

    // Ignore too short games
    if ((time(NULL) - winner->duel->startTime) <= 5)
        return;

    // ignore duels without both players on full hp
    if (winner->duel->initiatorHP < 100.0f && winner->duel->opponentHP < 100.0f)
        return;

    uint32 eloK = 100;
    DuelInfo* duel = winner->duel;
    float winnerGear = winner->GetAverageItemLevel();
    float loserGear = loser->GetAverageItemLevel();


    float winnerGearRating = winnerGear / loserGear;
    float loserGearRating = loserGear / winnerGear;

    float ratingWinner = winnerGearRating * winner->GetEloRating() * (winner->getLevel() / 60);
    float ratingLoser = loserGearRating * loser->GetEloRating() * (loser->getLevel() / 60);

    float winnerHpFactor = duel->initiator == winner ? duel->initiatorHP - winner->GetHealthPercent() : duel->opponentHP - winner->GetHealthPercent();
    winnerHpFactor = 1.0f - (0.25f * winnerHpFactor / 100);

    float changeRating = 1.0f / (1 + pow(10, (ratingWinner - ratingLoser) / 400));

    float ratingUpdate = eloK * (0 - changeRating) * winnerHpFactor;

    winner->ChangeEloRating(-ratingUpdate);
    loser->ChangeEloRating(ratingUpdate);

    ChatHandler(winner).PSendSysMessage(LANG_ELO_UPDATE, winner->GetEloRating(), -ratingUpdate, winnerHpFactor, winnerGearRating, loserGearRating);
    ChatHandler(loser).PSendSysMessage(LANG_ELO_UPDATE, loser->GetEloRating(), ratingUpdate, winnerHpFactor, winnerGearRating, loserGearRating);
    winner->SaveToDB();
    loser->SaveToDB();
}




std::string PvPMgr::generateTopTenText()
{
    std::string Text;
    SYSTEMTIME time;
    GetLocalTime(&time);
    // '2014-09-08 17:51:04.777'
    std::stringstream zeit;
    std::stringstream ss;

    zeit << time.wYear << "-" << time.wMonth << "-" << time.wDay << " " << time.wHour << ":" << time.wMinute;


    QueryResult* result = CharacterDatabase.Query("SELECT elorating, NAME FROM characters ORDER BY elorating DESC LIMIT 10;");
    if (!result)
    {
        Text = "Keine Platzierungen vorhanden!";
    }
    else
    {
        uint32 rank = 1;
        do
        {
            Field* fields = result->Fetch();
            ss << rank << ".  - " << fields[1].GetString() << " - " << fields[0].GetFloat() << "$b$b";
            rank++;
        } while (result->NextRow());
    }
    Text = "Top10-Duell-Rangliste vom:$b " + zeit.str() + "$b$b" + ss.str();

    delete result;

    return Text;
}
