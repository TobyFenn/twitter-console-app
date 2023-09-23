#ifndef TWITENG_H
#define TWITENG_H
#include <map>
#include <string>
#include <set>
#include <vector>
#include "user.h"
#include "datetime.h"

class TwitEng
{
 public:
  TwitEng();
  ~TwitEng();
  /**
   * Parses the Twitter database and populates internal structures
   * @param istr input stream (ifstream or stringstream) that has the 
   *             twitter database contents. If it is an ifstream, open
   *             the file and pass the ifstream to parse.
   * @return true if there is an error, false if successful
   */
  bool parse(std::istream& istr);

  /**
   * Allocates a tweet from its parts and adds it to internal structures
   * @param username of the user who made the tweet
   * @param timestamp of the tweet
   * @param text is the actual text of the tweet as a single string
   */
  void addTweet(const std::string& username, const DateTime& time, const std::string& text);

  /**
   * Searches for tweets with the given words and strategy
   * @param words is the hashtag terms in any case without the '#'
   * @param strategy 0=AND, 1=OR
   * @return the tweets that match the search
   */
  std::vector<Tweet*> search(std::vector<std::string>& terms, int strategy);

  /**
   * Dump feeds of each user to their own file
   */
  void dumpFeeds();

  /** 
   * Returns true if the given name corresponds to an
   * existing User, and false otherwise 
   */
  bool validUser(const std::string& name) const;

  /* You may add other member functions */

  std::vector<Tweet*> findUnion(std::vector<std::string>& terms);

  std::vector<Tweet*> findIntersection(std::vector<std::string>& terms);


 private:
  /* Add any other data members or helper functions here  */

  //key: name (string)  value: user* (ptr)
  std::map<std::string, User*> users_;
  std::map<std::string, std::vector<Tweet*> > tags_;

  

};


#endif
