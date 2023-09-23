#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <string>
#include <istream>
#include "twiteng.h"
#include "tweet.h"
#include "user.h"

Tweet::Tweet(){

    time_ = DateTime();
}

Tweet::Tweet(User* user, const DateTime& time, const std::string& text){
    user_ = user;
    time_ = time;
    text_ = text;

}

//returns timestamp of tweet
DateTime const & Tweet::time() const{
    return time_;
}

//returns text in tweet
std::string const & Tweet::text() const{
    return text_;
}

//returns hashtagged words without the pound sign
std::set<std::string> Tweet::hashTags() const{
    return hashTags_;
}

//returns user who authored the tweet
std::string Tweet::author() const{
    return user_->name();
}

//overload < returns result of less-than comparison of tweet's timestamp
bool Tweet::operator<(const Tweet& other) const{

    return this->time() < other.time();

}

//overloads <<
//returns tweet formatted in YYYY-MM-DD HH::MM::SS username tweet_text to the ostream passed in as an argument
std::ostream& operator<<(std::ostream& os, const Tweet& t){

    os << t.time() << " " << t.author() << " " << t.text();
    return os;

}

void Tweet::addHashtag(std::string tag) {
    hashTags_.insert(tag);
}

