#include "ScriptPCH.h"
#include "Chat.h"

#define FACTION_SPECIFIC 0

std::string GetNameLink(Player* player)
{
	std::string name = player->GetName();
	std::string color;
	switch(player->getClass()) 
	{
	case CLASS_DEATH_KNIGHT:
		color = "|cffC41F3B";
		break;
	case CLASS_DRUID:
		color = "|cffFF7D0A";
		break;
	case CLASS_HUNTER:
		color = "|cffABD473";
		break;
	case CLASS_MAGE:
		color = "|cff69CCF0";
		break;
	case CLASS_PALADIN:
		color = "|cffF58CBA";
		break;
	case CLASS_PRIEST:
		color = "|cffFFFFFF";
		break;
	case CLASS_ROGUE:
		color = "|cffFFF569";
		break;
	case CLASS_SHAMAN:
		color = "|cff0070DE";
		break;
	case CLASS_WARLOCK:
		color = "|cff9482C9";
		break;
	case CLASS_WARRIOR:
		color = "|cffC79C6E";
		break;
	}
	return "|Hplayer:"+name+"|h|cffFFFFFF["+color+name+"|cffFFFFFF]|h|r";
}

class cs_world_chat : public CommandScript
{
	public:
		cs_world_chat() : CommandScript("cs_world_chat"){}

	ChatCommand * GetCommands() const
	{
		static ChatCommand WorldChatCommandTable[] = 
		{
			{"c",	SEC_PLAYER,		true,		&HandleWorldChatCommand,	"", NULL},
			{NULL,		0,				false,		NULL,						"", NULL}
		};

		return WorldChatCommandTable;
	}

	static bool HandleWorldChatCommand(ChatHandler * handler, const char * args)
	{
		if (!handler->GetSession()->GetPlayer()->CanSpeak())
			return false;
		std::string temp = args;

		if (!args || temp.find_first_not_of(' ') == std::string::npos)
			return false;

		std::string msg = "";
		Player * player = handler->GetSession()->GetPlayer();

		switch(player->GetSession()->GetSecurity())
		{
			// Player
			case SEC_PLAYER:
				if (player->GetTeam() == ALLIANCE)
				{
					msg += "|cff0000ff[Alliance] ";
					msg += GetNameLink(player);
					msg += " |cfffaeb00";
				}

				else
				{
					msg += "|cffff0000[Horde] ";
					msg += GetNameLink(player);
					msg += " |cfffaeb00";
				}
				break;
			// VIP
			case SEC_VIP:
				msg += "|cffff8a00[VIP] ";
				msg += GetNameLink(player);
				msg += " |cfffaeb00";
				break;
			// Event Master
			case SEC_EVENTMASTER:
				msg += "|cff00ffff[Event Master] ";
				msg += GetNameLink(player);
				msg += " |cfffaeb00";
				break;
			// Head Event Master
			case SEC_HEVENTMASTER:
				msg += "|cff00ffff[Event Master] ";
				msg += GetNameLink(player);
				msg += " |cfffaeb00";
				break;
			// Game Master
			case SEC_GAMEMASTER:
				msg += "|cff00ffff[GM] ";
				msg += GetNameLink(player);
				msg += " |cfffaeb00";
				break;
			// Head Game Master
			case SEC_HGAMEASTER:
				msg += "|cff00ffff[HGM] ";
				msg += GetNameLink(player);
				msg += " |cfffaeb00";
				break;
			// Developer
			case SEC_DEVELOPER:
				msg += "|cff00ffff[Dev] ";
				msg += GetNameLink(player);
				msg += " |cfffaeb00";
				break;
			// Admin
			case SEC_ADMINISTRATOR:
				msg += "|cfffa9900[Admin] ";
				msg += GetNameLink(player);
				msg += " |cfffaeb00";
				break;
			// Admin
			case SEC_OWNER:
				msg += "|cfffa9900[Owner] ";
				msg += GetNameLink(player);
				msg += " |cfffaeb00";
				break;
			// Admin
			case SEC_CONSOLE:
				msg += "|cfffa9900[Console] ";
				msg += GetNameLink(player);
				msg += " |cfffaeb00";
				break;

		}
			
		msg += args;
		if (FACTION_SPECIFIC)
		{
			SessionMap sessions = sWorld->GetAllSessions();
			for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
				if (Player* plr = itr->second->GetPlayer())
					if (plr->GetTeam() == player->GetTeam())
						ChatHandler(plr).PSendSysMessage(msg.c_str());
		}
		else
			sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), 0);	

		return true;
	}
};

void AddSC_cs_world_chat()
{
	new cs_world_chat();
}