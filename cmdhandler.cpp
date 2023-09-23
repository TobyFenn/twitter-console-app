#include "cmdhandler.h"
#include "util.h"
#include <ctime>
using namespace std;


// QUIT
QuitHandler::QuitHandler()
{

}
QuitHandler::QuitHandler(Handler* next)
  : Handler(next)
{

}
bool QuitHandler::canHandle(const std::string& cmd) const
{
	return cmd == "QUIT";

}
Handler::HANDLER_STATUS_T QuitHandler::process(TwitEng* eng, std::istream& instr) const
{
	eng->dumpFeeds();
	return HANDLER_QUIT;
}



// AND
AndHandler::AndHandler()
{

}
AndHandler::AndHandler(Handler* next)
  : Handler(next)
{

}
bool AndHandler::canHandle(const std::string& cmd) const
{
	return cmd == "AND";

}
Handler::HANDLER_STATUS_T AndHandler::process(TwitEng* eng, std::istream& instr) const
{

	std::vector<std::string> terms;
	std::string term;
	while (instr >> term) {
		terms.push_back(term);
	}

	std::vector<Tweet*> hits = eng->search(terms, 0); 
	displayHits(hits); 
	return HANDLER_OK;
}


// OR
OrHandler::OrHandler()
{

}
OrHandler::OrHandler(Handler* next)
  : Handler(next)
{

}
bool OrHandler::canHandle(const std::string& cmd) const
{
	return cmd == "OR";

}
Handler::HANDLER_STATUS_T OrHandler::process(TwitEng* eng, std::istream& instr) const
{
	std::vector<std::string> terms;
	std::string term;
	while (instr >> term) {
		terms.push_back(term);
	}

	std::vector<Tweet*> hits = eng->search(terms, 1); 
	displayHits(hits); 
	return HANDLER_OK;
}


// TWEET command
TweetHandler::TweetHandler()
{

}
TweetHandler::TweetHandler(Handler* next)
  : Handler(next)
{

}
bool TweetHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TWEET";

}
Handler::HANDLER_STATUS_T TweetHandler::process(TwitEng* eng, std::istream& instr) const
{

	std::string user;
	std::string msg;

	instr >> user;

	if (eng->validUser(user) == false) 
	{
		return HANDLER_ERROR;
	} 

	instr >> std::ws;
	getline(instr, msg);

	DateTime currTime; 
	eng->addTweet(user, currTime, msg);

	return HANDLER_OK;
}







