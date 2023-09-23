#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
#include <set>
#include <list>
#include <vector>
#include <istream>
#include <algorithm>
#include "twiteng.h"
#include "user.h"
#include "tweet.h"

//ctor
User::User(std::string name) {
    name_ = name;

}

//dtor
User::~User(){
    
}

//returns name
std::string User::name() const {
    return name_;
}

//returns set of users who follow this user
std::set<User*> User::followers() const {
    return followers_;
}

//returns set of users who this user follows
std::set<User*> User::following() const {
    return following_;
}

//returns list of tweets this user has posted
std::list<Tweet*> User::tweets() const {
    return tweets_;
}

//add a user to this user's list of followers
void User::addFollower(User* u) {
    followers_.insert(u);
}

//add a user to this user's list of who they are following
void User::addFollowing(User* u) {
    following_.insert(u);
}

//adds given tweet as a post from this user
void User::addTweet(Tweet* t) {
    tweets_.push_back(t);
}

//returns vector of pointers to all the tweets from this user and those they follow in timestamp order
std::vector<Tweet*> User::getFeed(){

    //ptrs to all the tweets
    std::vector<Tweet*> userFeed;

    // all the user's own tweets
    for(std::list<Tweet*>::iterator it = tweets_.begin(); it != tweets_.end(); ++it){
        userFeed.push_back(*it);
    } 

    // all the user's followings' tweets
    for(std::set<User*>::iterator it = following_.begin(); it != following_.end(); ++it){ //for each person they follow
        for(std::list<Tweet*>::iterator it2 = (*it)->tweets_.begin(); it2 != (*it)->tweets_.end(); ++it2){
            userFeed.push_back(*it2);
        }
    }

    sort(userFeed.begin(), userFeed.end(), TweetComp()); //sort

    return userFeed;
}
