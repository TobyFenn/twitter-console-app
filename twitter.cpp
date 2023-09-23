#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "twiteng.h"
#include "util.h"
#include "cmdhandler.h"

using namespace std;

Handler* createHandlers();

Handler* createHandlers()
{
  return new AndHandler(
    new OrHandler(
      new TweetHandler(
        new QuitHandler
      )
    )
  );
}

int main(int argc, char* argv[])
{
  if(argc < 2){
    cerr << "Please provide the twitter data file" << endl;
    return 1;
  }

  TwitEng twit;

  ifstream ifile(argv[1]);
  if (ifile.fail()) {
    cerr << "Unable to open " << argv[1] << endl;
    return 1;
  }

  if ( twit.parse(ifile) ){ //if error then parse returns true
    cerr << "Unable to parse " << argv[1] << endl;
    return 1;
  }

  ifile.close();

  // Initialize command handlers
  Handler* handlers = createHandlers();

  cout << "=====================================" << endl;
  cout << "Menu: " << endl;
  cout << "  AND term term ...                  " << endl;
  cout << "  OR term term ...                   " << endl;
  cout << "  TWEET username tweet_text" << endl;
  cout << "  QUIT (and write feed files)        " << endl;
  cout << "=====================================" << endl;

  Handler::HANDLER_STATUS_T status = Handler::HANDLER_OK;
  while(status != Handler::HANDLER_QUIT && 
        status != Handler::HANDLER_KILL)
  {
    cout << "\nEnter command: " << endl;
    string line;
    getline(cin,line);
    stringstream ss(line);
    string cmd;
    if((ss >> cmd)){
      // convert to upper case
      convUpper(cmd); 
      // Invoke the chain of responsibility
      status = handlers->handle(&twit, cmd, ss);
    }
  }
  delete handlers;
  return 0;
}