

#ifndef TestSequencer_h
#define TestSequencer_h

#include "Tokenizer.hpp"
#include "keywords.hpp"
#include <initializer_list>

namespace MyDB {


  class TestSequencer {
  public:
    TestSequencer(Tokenizer &aTokenizer)
      : tokenizer(aTokenizer), state{true} {}
    
    struct SeqState {
      bool state;
      TestSequencer &seq;
    };
    
    TestSequencer& nextIs(const KWList &aList) {
      if(state) {
        std::vector<Keywords> theList{aList};
        int thePos{0};
        for(auto theKW: theList) {
          Token &theToken=tokenizer.peek(thePos++);
          if(theKW!=theToken.keyword) {
            state=false;
            break;
          }
        }
        if(state) {
          return skip(theList.size());
        }
      }
      return *this;
    }

    TestSequencer& getIdentifier(std::string &aName) {
      return *this;
    }
    
    TestSequencer& getNumber(int &aValue) {
      Token &theToken=tokenizer.current();
      aValue=std::stoi(theToken.data);
      return *this;
    }
    
    TestSequencer& skipPast(char aChar) {
      if(state) {
        while(tokenizer.next()) {
          auto theToken=tokenizer.current();
          if(theToken.type==TokenType::punctuation) {
            if(theToken.data[0]==aChar) {
              tokenizer.next();//skip it...
              return *this;
            }
          }
        }
        state=false;
      }
      return *this;
    }

    TestSequencer& skip(size_t aCount) {
      if(state) {
        tokenizer.next((int)aCount); //actually skip...
      }
      return *this;
    }

    operator bool() {return state;}

    TestSequencer& clear() {
      state=true;
      return *this;
    }
    
  protected:
    Tokenizer &tokenizer;
    bool      state;
  };

}

#endif /* TestSequencer_h */
