#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "twiteng.h"
#include "util.h"
#include "user.h"
#include "tweet.h"

using namespace std;

TwitEng::TwitEng()
{

}

TwitEng::~TwitEng()
{
  //delete all users
  for(std::map<std::string, User*>::iterator it = users_.begin() ; it != users_.end(); ++it){ 

    std::list<Tweet*> vec = it->second->tweets();

    for(std::list<Tweet*>::iterator it2 = vec.begin() ; it2 != vec.end(); ++it2){ 
      delete *it2; //delete the user's tweets
    }

    delete it->second; //delete the user

  }
}

bool TwitEng::parse(std::istream& istr) 
{
  int numUsers;
  string userName;
  istr >> numUsers;
  if (istr.fail()) {
    cerr << "Unable to read number of users " << endl;
    return true;
  }

  // consume the newline left by >>
  std::string dummy;
  std::getline(istr, dummy);

  //for each of the numUsers first lines, make a new user named xxx and updating their following list
  for (int i = 0; i < numUsers; i++) {

    std::string line;
    getline(istr, line);

    std::stringstream ss(line);

    std::string name;
    ss >> name;

    User* subject;

    if (!validUser(name)) { //if this person doesn't exist already, create them
        subject = new User(name);
        users_.insert(make_pair(name, subject)); // Insert into map as soon as it's created
    } else {
        subject = users_[name];
    }

    //deal with any remaining users (i.e. the first user's following list)

    std::string followName; 
    while (ss >> followName) {

      User* following;

      if (!validUser(followName)) { //if this person doesn't exist already, create them
          following = new User(followName);
          users_.insert(make_pair(followName, following)); // Insert into map asap
      } else {
          following = users_[followName];
      }

      //add to following either the new user or the existing one based on name
      subject->addFollowing(following);
    }
  }

  std::string line;

  while (getline(istr, line)) {
    //for each of the remaining lines, add this tweet to its user (unsorted)
    //there can be a blank line here and if you encounter it then skip
    //i.e. keep repeating until you hit the end of the document

    if (line.size() == 0) {
      continue;
    }

    std::stringstream ss(line);

    //make a new datetime, assign it new values, later put it into the new tweet

    //DATE
    DateTime tweetTime;
    ss >> tweetTime;
    if (ss.fail()) {
      return true;
    }

    //AUTHOR
    std::string author;
    ss >> author;
    if (ss.fail()) {
      return true;
    }

    if (validUser(author) == false) {
      return true;
    }

    author = trim(author);

    //CONTENTS
    std::string tweetText;
    ss >> std::ws;
    getline(ss, tweetText);

    //make the new tweet
    addTweet(author, tweetTime, tweetText);

    //return false if successful, true otherwise
    if (istr.fail()) {
      return true;
    }
  }

  return false;

}


  /**
   * Allocates a tweet from its parts and adds it to internal structures
   * @param username of the user who made the tweet
   * @param timestamp of the tweet
   * @param text is the actual text of the tweet as a single string
   */
void TwitEng::addTweet(const std::string& username, const DateTime& time, const std::string& text) {

    User* tweetAuthor = users_[username]; //username bad vs invalid?

    Tweet* newTweet = new Tweet(tweetAuthor, time, text);

    //HASHTAGS (can be located anywhere)
    std::stringstream tweetline(text);
    std::string dummy;

    while (getline(tweetline, dummy, '#')) { 
      //gets rid of everything leading up to pound sign
      //puts pound sign word into a tag and adds it
      //tries to repeat and doesnt if theres nothing after

      std::string tag;
      tweetline >> tag;
      convLower(tag); //to lowercase
      newTweet->addHashtag(tag);

    //handle tags_ updates:

    //if hashtag is new, add it
    std::map<std::string, std::vector<Tweet*> >::iterator it = tags_.find(tag);
    if (it != tags_.end()) {
        //tag exists in map

        //update it:
        it->second.push_back(newTweet);
    } else {
        //tag does not exist in map
        //add it:
        std::vector<Tweet*> tweetVector; //this used to be dynamically allocated ?
        tweetVector.push_back(newTweet);

        tags_.insert(make_pair(tag, tweetVector));
    }
  }

    //add to person
    tweetAuthor->addTweet(newTweet);
}

std::vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy) {

  std::vector<Tweet*> hits;

  if (strategy == 0) { //AND
    return findIntersection(terms);
  } else { //OR
    return findUnion(terms);
  }
}

  /**
   * Dump feeds of each user to their own file
   */
void TwitEng::dumpFeeds() {

  for(std::map<std::string, User*>::iterator it = users_.begin() ; it != users_.end(); ++it){

    std::string filename = (it->first) + ".feed";
    std::ofstream ofile(filename.c_str());

    std::vector<Tweet*> userFeed = it->second->getFeed(); //?

    ofile << it->first << endl;

    for(vector<Tweet*>::iterator it = userFeed.begin(); it != userFeed.end(); ++it){
      ofile << **it << endl;
    }

    ofile.close();
  }

}

  /** 
   * Returns true if the given name corresponds to an
   * existing User, and false otherwise 
   */
 bool TwitEng::validUser(const std::string& name) const {

  if (users_.find(name) == users_.end()){
    return false;
  } else {
    return true;
  }
}

// union -- OR
std::vector<Tweet*> TwitEng::findUnion(std::vector<std::string>& terms) {

  std::set<Tweet*> result;
  std::vector<Tweet*> retVec;

  std::string tag;

  // returning an empty set.
  if (terms.size() == 0) {
    return retVec;
  }

  //iterate through terms vector
  std::vector<std::string>::iterator term;
  for (term = terms.begin(); term != terms.end(); ++term) {

    tag = *term;
    convLower(tag); //to lowercase

    //find tag's instance in tags_ map
    std::map<std::string, std::vector<Tweet*> >::iterator it = tags_.find(tag); //logn time

    if (it != tags_.end()) {
        //key is found

      for(std::vector<Tweet*>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2){ //constant time
        //check each tweet* in tag's vector
        //this is 'OR' 
        result.insert(*it2); //checks for duplicates etc... takes logT time (constant)
      }
    }
  }

  //copy from set to vector
  for(std::set<Tweet*>::iterator it = result.begin(); it != result.end(); ++it) {
      retVec.push_back(*it);
  }

  return retVec;
}

// intersection: AND
std::vector<Tweet*> TwitEng::findIntersection(std::vector<std::string>& terms) {

  std::set<Tweet*> result;
  std::vector<Tweet*> retVec;

  std::string tag;

  //take the first term's vector and put it in the result set
  //then compare that result set to every other term's vector and
  //remove each element from the set that is not in the vector

  // returning an empty set.
  if (terms.size() == 0) {
    return retVec;
  }

  std::map<std::string, std::vector<Tweet*> >::iterator firstTerm = tags_.find(terms.front());

  if (firstTerm != tags_.end()) {
    // Found the term in tags_
    for (std::vector<Tweet*>::iterator it = firstTerm->second.begin(); it != firstTerm->second.end(); ++it) {
      result.insert(*it);
    }
  }

  //iterate through terms vector
  std::vector<std::string>::iterator term;
  for(term = terms.begin(); term != terms.end(); ++term) {

      tag = *term;
      convLower(tag);

      std::map<std::string, std::vector<Tweet*> >::iterator tagit = tags_.find(tag); 

      if (tagit != tags_.end()) {
          std::set<Tweet*> tmp; 

          for(std::vector<Tweet*>::iterator it1 = tagit->second.begin(); it1 != tagit->second.end(); ++it1) {
              //find matching tweets in result
              if(result.find(*it1) != result.end()) {
                  tmp.insert(*it1);
              }
          }

      result = tmp; // Swap tmp with result for next intersection

      } else {
          return retVec;
      }
  }

  //copy from set to vector
  for(std::set<Tweet*>::iterator it = result.begin(); it != result.end(); ++it) {
    retVec.push_back(*it);
  }

  return retVec;

}