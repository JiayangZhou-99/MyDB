
#ifndef Faked_hpp
#define Faked_hpp

#include <stdio.h>
#include <string>

namespace MyDB {

  struct Fake {
    Fake();
      
    static void autoTest(std::ostream &anOut, size_t aCount=100) {
      Animals::autoTest(anOut,aCount);
      Counting::autoTest(anOut,aCount);
      Events::autoTest(anOut,aCount);
      People::autoTest(anOut,aCount);
      Places::autoTest(anOut,aCount);
      Writing::autoTest(anOut,aCount);
    }
    
    struct Animals {
      static std::string type();
      static std::string petName();
      static void autoTest(std::ostream &anOut, size_t aCount=100);
    };
    
    struct Counting {
      static int         integer(size_t maxdigits);
      static size_t      range(size_t aMin, size_t aMax);
      static double      real(size_t maxdigits);
      static std::string spoken(size_t maxdigits);
      static std::string nth();
      static std::string sizes();
      static void autoTest(std::ostream &anOut, size_t aCount=100);
    };
    
    struct Events {
      static std::string dateYMD();
      static std::string dateMDY();
      static std::string timestamp();
      static size_t      day();
      static std::string dayOfWeek(bool aShort=false);
      static size_t      daysInMonth(size_t aMonth,size_t aYear=0);
      static size_t      month();
      static std::string monthName(bool aShort=false);
      static size_t      year();
      static void autoTest(std::ostream &anOut, size_t aCount=100);
    };
    
    struct People {
      static std::string first_name();
      static std::string last_name();
      static std::string full_name();
      static std::string email();
      static std::string mobile();
      static size_t      age(size_t aMin, size_t aMax);
      static void autoTest(std::ostream &anOut, size_t aCount=100);
    };
    
    struct Places {
      static std::string direction(); //North,South...
      static std::string city();
      static std::string state();
      static std::string state2();
      static std::string street_type();
      static size_t      house_number();
      static std::string street();
      static size_t      zipcode();
      static std::string address();
      static void autoTest(std::ostream &anOut, size_t aCount=100);
    };
    
    struct Writing {
      static std::string book_title();
      static std::string author();
      static std::string author_first();
      static std::string author_last();
      static std::string determiner();
      static std::string predetermine();
      static std::string demonstrative();
      static std::string definite();
      static std::string indefinite();
      static std::string distributive();
      static std::string difference();
      static std::string pronoun();
      static std::string noun();
      static std::string verb();
      static std::string adjective();
      static std::string word();
      static std::string fakeword();
      static std::string sentence(size_t maxwords=20);
      static void autoTest(std::ostream &anOut, size_t aCount=100);
    };
    
  };

}

#endif /* Faked_hpp */
