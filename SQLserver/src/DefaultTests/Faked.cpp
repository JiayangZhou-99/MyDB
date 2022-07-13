
#include "Faked.hpp"
#include <sstream>
#include <ctime>
#include <map>
#include <algorithm>
#include <cmath>
#include <iostream>

namespace MyDB {

  Fake::Fake() {
  }

  std::string Fake::Animals::petName() {
    static const char* theNames[]= {
      "Ace","Bo","Chili","Cocoa","Dash","Denver","Duke","Garbo",
      "Harley","Hux","Indie","Jax","Jazz","Kai","Latte","Oakley",
      "Pixel","Rebel","Remy","Rex","Riley","Roscoe","Rumor","Sam",
      "Scamp","Shep","Tux","Vex","Wiz"
    };
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    return theNames[(rand() % theSize)];
  }

  std::string Fake::Animals::type() {
    static const char* theNames[]= {
      "Bird","Cat","Dow","Dog","Fish","Fox","Goat","Horse",
      "Mouse","Pig","Rabbit","Turtle","Whale"
    };
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    return theNames[(rand() % theSize)];
  }

  void Fake::Animals::autoTest(std::ostream &anOut, size_t aMax) {
    for(size_t i=0;i<aMax;i++) {
      anOut << petName() << "\n";
      anOut << type() << "\n";
    }
  }

  //-------------------------------------------------

  int Fake::Counting::integer(size_t maxdigits) {
    size_t theMod = std::pow(10,maxdigits)+1;
    return rand() % theMod;
  }

  size_t Fake::Counting::range(size_t aMin, size_t aMax) {
    return aMin+ (rand() % (aMax-aMin));
  }

  double Fake::Counting::real(size_t maxdigits) {
    double num=(double)integer(maxdigits);
    double sub=(double)integer(3);
    return num+(1000.0/sub);
  }

  static std::string ones[] =
      { "", "one ", "two ", "three ", "four ", "five ", "six ",
        "seven ", "eight ", "nine ", "ten ", "eleven ", "twelve ",
        "thirteen ", "fourteen ", "fifteen ","sixteen ",
        "seventeen ", "eighteen ","nineteen " };
   
  // strings at index 0 and 1 are not used, they is to
  // make array indexing simple
  static std::string tens[] = {
    "", "", "twenty ", "thirty ", "forty ", "fifty ", "sixty ",
     "seventy ", "eighty ", "ninety " };

  std::string numToWords(size_t aValue, std::string aSuffix) {
      std::string theResult;

      if (aValue > 19)
        theResult += (tens[aValue / 10] + ones[aValue % 10]);
      else theResult += ones[aValue];
   
      // if n is non-zero
      if (aValue) theResult += aSuffix;
      return theResult;
  }
 
  std::string Fake::Counting::spoken(size_t maxdigits) {
    int theInt=integer(3);

    std::string theResult;
 
    // millions places (if any)
    theResult += numToWords((theInt / 10000000), "million ");
 
    // hundred thousands and one millions places (if any)
    theResult += numToWords(((theInt / 100000) % 100), "hundred thousand ");
 
    // thousands and tens thousands
    theResult += numToWords(((theInt / 1000) % 100), "thousand ");
 
    // hundreds places (if any)
    theResult += numToWords(((theInt / 100) % 10), "hundred ");
 
    if (theInt > 100 && theInt % 100)
      theResult += "and ";
 
    theResult += numToWords((theInt % 100), "");
    return theResult;
  }

  static std::string ith[] = {
    "th","st","nd","rd","th","th","th","th","th","th"};

  std::string Fake::Counting::nth() {
    size_t theInt=integer(2);
    std::string theResult(std::to_string(theInt));
    if(theInt>10 && theInt<20) {
      theResult+="th"; //teens
    }
    else theResult+=ith[theInt % 10];
    return theResult;
  }

  std::string Fake::Counting::sizes() {
    static const char* theSizes[] = {"xsmall","small","medium","large","xlarge"};
    static size_t theSize = sizeof(theSizes)/sizeof(char*);
    return theSizes[(rand() % theSize)];
  }

  void Fake::Counting::autoTest(std::ostream &anOut, size_t aMax) {
    for(size_t i=0;i<aMax;i++) {
      anOut << integer(5) << "\n";
      anOut << real(5) << "\n";
      anOut << spoken(3) << "\n";
      anOut << nth() << "\n";
      anOut << sizes() << "\n";
    }
  }

  //-------------------------------------------------

  size_t getCurrentYear() {
    std::time_t t = std::time(nullptr);
    std::tm *const pTInfo = std::localtime(&t);
    return 1900 + pTInfo->tm_year;
  }

  size_t Fake::Events::day() {
    return (rand() % 31)+1;
  }

  size_t Fake::Events::month() {
    return (rand() % 12)+1;
  }

  size_t Fake::Events::year() {
    return 1900+(rand() % 121)+1;
  }

  size_t Fake::Events::daysInMonth(size_t aMonth, size_t aYear) {
    static size_t theDIM[]= {
      0,31,28,31,30,31,30,31,31,30,31,30,31};
      
    if(!aYear) aYear=getCurrentYear();
    size_t theSize=theDIM[aMonth];
    if(2==aMonth) {
      if((aYear%400==0) || (aYear%4==0 && aYear%100!=0))
        theSize=29;
    }
    return theSize;
  }

  void getRandomDate(size_t &aDay, size_t &aMonth, size_t &aYear) {
    aYear=Fake::Events::year();
    aMonth=Fake::Events::month();
    size_t theDIM=Fake::Events::daysInMonth(aMonth,aYear);
    aDay = (rand() % theDIM)+1;
  }

  std::string padzero(size_t aValue) {
    static const char* thePads[]={"","0"};
    std::string theResult(thePads[aValue<10]);
    theResult+=std::to_string(aValue);
    return theResult;
  }

  std::string Fake::Events::dateYMD() {
    size_t theMonth,theDay,theYear;
    getRandomDate(theDay,theMonth,theYear);
    std::string theResult(std::to_string(theYear)+'-');
    theResult+=padzero(theMonth)+'-';
    theResult+=padzero(theDay);
    return theResult;
  }
    
  std::string Fake::Events::dateMDY() {
    size_t theMonth,theDay,theYear;
    getRandomDate(theDay,theMonth,theYear);
    std::string theResult;
    theResult+=padzero(theMonth)+'-';
    theResult+=padzero(theDay)+'-';
    theResult+=std::to_string(theYear);
    return theResult;
  }

  void getRandomTime(size_t &anHour, size_t &aMin, size_t &aSec) {
    anHour=(rand() % 24)+1;
    aMin=(rand() % 60)+1;
    aSec=(rand() % 60)+1;
  }

  std::string Fake::Events::timestamp() {
    size_t theMonth,theDay,theYear;
    getRandomDate(theDay,theMonth,theYear);
    size_t theHr,theMin,theSec;
    getRandomTime(theHr,theMin,theSec);
    std::string theResult(std::to_string(theYear)+'-'+
                          padzero(theMonth)+'-'+
                          padzero(theDay)+' '+
                          padzero(theHr)+':'+
                          padzero(theMin)+':'+
                          padzero(theSec));
    return theResult;
  }

  std::string Fake::Events::dayOfWeek(bool aShort) {
    static const char* theNames[] = {
      "Sunday","Monday","Tuesday","Wednesday","Thursday",
      "Friday","Saturday"};
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    size_t theDay=(rand() % theSize);
    std::string theResult(theNames[theDay]);
    if(aShort) theResult.erase(3, std::string::npos);
    return theResult;
  }

  std::string Fake::Events::monthName(bool aShort) {
    static const char* theNames[] = {
      "January","February","March","April","May","June","July",
      "August","September","October","Novemeber","December"};
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    size_t theMonth=(rand() % theSize);
    std::string theResult(theNames[theMonth]);
    if(aShort) theResult.erase(3, std::string::npos);
    return theResult;
  }

  void Fake::Events::autoTest(std::ostream &anOut, size_t aMax) {
    for(size_t i=0;i<aMax;i++) {
      anOut << dateYMD() << "\n";
      anOut << dateMDY() << "\n";
      anOut << timestamp() << "\n";
      anOut << day() << "\n";
      anOut << dayOfWeek() << "\n";
      anOut << daysInMonth(month()) << "\n";
      anOut << monthName() << "\n";
      anOut << year() << "\n";
    }
  }

  //-------------------------------------------------

  std::string Fake::People::first_name() {
    static const char* theNames[] = {
      "Aditya","Anirudh","Angela","Anne","Barb","Bill","Carl",
      "Cindy","David","Deana","Deng","Fae","Edward","Felipe",
      "Frank","Gill","Ge","Hanna","Henry","Isabel","Jason",
      "Jen","Ji","Jon","Julian","Justine","Kevin","Kim",
      "Lana","Leo","Li","Madison","Marie","Martin","Megan",
      "Mohammed","Nadine","Nathan","Nick","Nicole","Orson",
      "Pu","Qing","Rick","Rita","Serena","Sienna","Stephen",
      "Tiffany","Tim","Trevon","Truong","Vincent","Vicki","Ya"};
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    return theNames[(rand() % theSize)];
  }

  std::string Fake::People::last_name() {
    static const char* theNames[]={
      "Allen","Brown","Carter","Chan","Cheng","Cooper",
      "Davis","Deng","Diaz","Evans","Foster","Garcia",
      "Guha","Gupta","Harris","He","Hernandez","Hill",
      "Hong","Huang","James","Johnson","Khan","King",
      "Kim","Lee","Liang","Liu","Lopez","Martinez",
      "Miller","Morris","Murphy","Nguyen","Park","Patel",
      "Perez","Pham","Price","Qiu","Reed","Roberts",
      "Robinson","Sanchez","Scott","Singh","Smith","Tan",
      "Taylor","Tran","Turner","Wade","Walker","Williams",
      "Wilson","Xu","Yang","Young","Zhou"};
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    return theNames[(rand() % theSize)];
  }

  std::string Fake::People::full_name() {
    return std::string(first_name()+" "+last_name());
  }

  size_t Fake::People::age(size_t aMin, size_t aMax) {
    return Fake::Counting::range(aMin, aMax);
  }

  std::string Fake::People::mobile() {
    size_t theArea=100+(rand() % 900);
    size_t thePre=100+(rand() % 900);
    size_t thePost=1000+(rand() % 9000);
    return std::string(std::to_string(theArea)+"-"+
                       std::to_string(thePre)+"-"+
                       std::to_string(thePost));
  }

  std::string Fake::People::email() {
    const char* theTLNs[] ={
      ".biz",".com",".edu",".gov",".net",".org"
    };
    static size_t theTLNSize = sizeof(theTLNs)/sizeof(char*);
        
    std::string thePre,thePost;
    size_t theOption=rand() % 2;
    switch(theOption) {
      case 0:
        thePre=first_name();
        thePost=last_name();
        break;
      case 1:
        thePre=first_name()[0];
        thePre+=last_name();
        thePost=Writing::adjective()+Writing::noun();
        break;
    }
    
    auto theTLN=theTLNs[rand() % theTLNSize];
    return std::string(thePre+"@"+thePost+theTLN);
  }

  void Fake::People::autoTest(std::ostream &anOut, size_t aMax) {
    for(size_t i=0;i<aMax;i++) {
      anOut << first_name() << "\n";
      anOut << last_name() << "\n";
      anOut << full_name() << "\n";
      anOut << email() << "\n";
      anOut << mobile() << "\n";
    }
  }

  //-------------------------------------------------

  //this must stay in sync with cityState()...
  std::string Fake::Places::city() {
    static const char* theNames[]={
      "Albuquerque","Arlington","Atlanta","Austin","Baltimore",
      "Boston","Charlotte","Chicago","Colorado Springs","Columbus",
      "Dallas","Denver","Detroit","El Paso","Fort Worth","Fresno",
      "Houston","Indianapolis","Jacksonville","Kansas City",
      "Las Vegas","Long Beach","Los Angeles","Louisville",
      "Memphis","Mesa","Miami","Milwaukee","Minneapolis",
      "Nashville","New Orleans","New York","Oakland",
      "Oklahoma City","Omaha","Philadelphia","Phoenix",
      "Portland","Raleigh","Sacramento","San Antonio",
      "San Diego","San Francisco","San Jose","Seattle",
      "Tucson","Tulsa","Virginia Beach","Washington","Wichita"};
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    return theNames[(rand() % theSize)];
  }

  std::string Fake::Places::state() {
    static const char* theNames[] = {
      "Alabama","Alaska","Arizona","Arkansas","California",
      "Colorado","Connecticut","Delaware","Florida","Georgia",
      "Hawaii","Idaho","Illinois","Indiana","Iowa","Kansas",
      "Kentucky","Louisiana","Maine","Maryland","Massachusetts",
      "Michigan","Minnesota","Mississippi","Missouri",
      "Montana","Nebraska","Nevada","New Hampshire","New Jersey",
      "New Mexico","New York","North Carolina","North Dakota",
      "Ohio","Oklahoma","Oregon","Pennsylvania","Rhode Island",
      "South Carolina","South Dakota","Tennessee","Texas","Utah",
      "Vermont","Virginia","Washington","West Virginia",
      "Wisconsin","Wyoming"};
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    return theNames[(rand() % theSize)];
  }

  std::string Fake::Places::state2() {
    static const char* theNames[]={
      "AL","AK","AZ","AR","CA","CO","CT","DE","FL","GA",
      "HI","ID","IL","IN","IA","KS","KY","LA","ME","MD",
      "MA","MI","MN","MS","MO","MT","NE","NV","NH","NJ",
      "NM","NY","NC","ND","OH","OK","OR","PA","RI","SC",
      "SD","TN","TX","UT","VT","VA","WA","WV","WI","WY"};
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    return theNames[(rand() % theSize)];
  }

  static std::map<std::string,std::string> theCityStates={
    {"Albuquerque","NM"},{"Arlington","VA"},{"Atlanta","GA"},
    {"Austin","TX"},{"Baltimore","MD"},{"Boston","MA"},
    {"Charlotte","NC"},{"Chicago","IL"},{"Colorado Springs","CO"},
    {"Columbus","OH"},{"Dallas","TX"},{"Denver","CO"},
    {"Detroit","MI"},{"El Paso","TX"},{"Fort Worth","TX"},
    {"Fresno","CA"},{"Houston","TX"},{"Indianapolis","IN"},
    {"Jacksonville","FL"},{"Kansas City","MO"},{"Las Vegas","NV"},
    {"Long Beach","CA"},{"Los Angeles","CA"},{"Louisville","KY"},
    {"Memphis","TN"},{"Mesa","AZ"},{"Miami","FL"},{"Milwaukee","WI"},
    {"Minneapolis","MN"},{"Nashville","TN"},{"New Orleans","LA"},
    {"New York","NY"},{"Oakland","CA"},{"Oklahoma City","OK"},
    {"Omaha","NE"},{"Philadelphia","PA"},{"Phoenix","AZ"},
    {"Portland","OR"},{"Raleigh","SC"},{"Sacramento","CA"},
    {"San Antonio","TX"},{"San Diego","CA"},{"San Francisco","CA"},
    {"San Jose","CA"},{"Seattle","WA"},{"Tucson","AZ"},{"Tulsa","OK"},
    {"Virginia Beach","VA"},{"Washington","DC"},{"Wichita","KS"}};

  size_t Fake::Places::zipcode() {
    return 10000+(rand() % 89999);
  }

  size_t Fake::Places::house_number() {
    return 10+(rand() % 9999);
  }

  std::string Fake::Places::direction() {
    static const char* theNames[]= {"North","South","East","West"};
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    return theNames[(rand() % theSize)];
  }

  std::string Fake::Places::street_type() {
    static const char* theNames[]= {
      "Ave.","Blvd.","Cir.","Ct.","Dr.","Pl.","Rd.","St.","Way"
    };
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    return theNames[(rand() % theSize)];
  }

  std::string Fake::Places::street() {
    static const char* theNames[]= {
      "Bluff","Canterbury","Cedar","Central","College",
      "Eighth","Elm","Fifth","First","Fourth","Hill",
      "Jefferson","King","Lake","Lincoln","Main","Maple",
      "Ninth","Oak","Park","Pine","Redwood","Sahuaro","Second ",
      "Seventh","Sixth","Third","University","View","Washington"};
    static size_t theSize = sizeof(theNames)/sizeof(char*);
    std::string theResult;
    if(rand() % 2) {
      theResult=direction()[0]; //N,S,E,W
      theResult+=". ";
    }
    
    theResult+=theNames[rand() % theSize];
    return theResult + " " + street_type();
  }


  std::string Fake::Places::address() {
    std::stringstream theOut;
    std::string theCity(city());
    theOut << house_number() << ' ' << street() << ", "
              << theCity << " " << theCityStates[theCity]
              << ", " << zipcode();
    return theOut.str();
  }

  void Fake::Places::autoTest(std::ostream &anOut, size_t aMax) {
    for(size_t i=0;i<aMax;i++) {
      anOut << state() << "\n";
      anOut << state2() << "\n";
      anOut << address() << "\n";
      //all the others fn's get tested with these...
    }
  }

  //-------------------------------------------------

  std::string Fake::Writing::book_title() {
    std::string theResult("The YouTube guide to your ");
    switch(rand() % 4) {
      case 0:
        theResult+=noun();
        break;
      case 1:
        theResult="A closer look at your "+noun();
        break;
      case 2:
        theResult="Happiness is a "+adjective()+" "+noun();
        break;
      case 3:
        theResult=noun()+" repair for newbies";
        theResult[0]=toupper(theResult[0]);
        break;
      default:
        theResult="Examining your "+noun()+" for profit";
        break;
    }
    return theResult;
  }

  std::string Fake::Writing::author() {
    return std::string(author_first()+" "+author_last());
  }

  std::string Fake::Writing::author_first() {
    return People::first_name();
  }

  std::string Fake::Writing::author_last() {
    return People::last_name();
  }

  std::string Fake::Writing::pronoun() {
    static const char* theWords[] ={
      "my","your","his","her","its","our","they","their"};
    static size_t theSize = sizeof(theWords)/sizeof(char*);
    return theWords[(rand() % theSize)];
  }

  std::string Fake::Writing::demonstrative() {
    static const char* theWords[] ={
      "this","that","these","those",
    };
    static size_t theSize = sizeof(theWords)/sizeof(char*);
    return theWords[(rand() % theSize)];
  }

  std::string Fake::Writing::determiner() {
    static const char* theWords[] ={
      "a","an","the"};
    static size_t theSize = sizeof(theWords)/sizeof(char*);
    return theWords[(rand() % theSize)];
  }

  std::string Fake::Writing::predetermine() {
    static const char* theWords[] ={
      "such", "what", "rather", "quite"};
    static size_t theSize = sizeof(theWords)/sizeof(char*);
    return theWords[(rand() % theSize)];
  }

  std::string Fake::Writing::definite() {
    return std::string("the");
  }

  std::string Fake::Writing::indefinite() {
    static const char* theWords[] ={"a","an"};
    static size_t theSize = sizeof(theWords)/sizeof(char*);
    return theWords[(rand() % theSize)];
  }

  std::string Fake::Writing::distributive() {
    static const char* theWords[] ={
      "all", "both", "half", "either", "neither", "each", "every"
    };
    static size_t theSize = sizeof(theWords)/sizeof(char*);
    return theWords[(rand() % theSize)];
  }

  std::string Fake::Writing::difference() {
    static const char* theWords[] ={"other","another"};
    static size_t theSize = sizeof(theWords)/sizeof(char*);
    return theWords[(rand() % theSize)];
  }

  std::string Fake::Writing::noun() {
    static const char* theWords[] = {
      "air","algorithm","area","art","back","body","book",
      "business","car","cat","chair","child","city",
      "community","company","country","day","device","door",
      "end","eye","face","fact","family","father","foo",
      "friend","game","group","hawk","health","history","home",
      "hour","house","idea","information","issue","job","kangaroo",
      "kid","laptop","law","level","life","line","laundry","man",
      "media","minute","moment","money","month","morning","mother",
      "mouse","name","night","number","office","others","parent",
      "part","party","people","person","phone","place","point",
      "power","problem","product","program","question","reason",
      "research","result","right","room","school","service",
      "side","state","story","student","study","system","teacher",
      "team","thing","time","walrus","water","way","week","woman",
      "word","work","world","year"};
    static size_t theSize = sizeof(theWords)/sizeof(char*);
    return theWords[(rand() % theSize)];
  }

  std::string Fake::Writing::verb() {
    static const char* theWords[] = {
      "add","allow","appear","ask","be","become","begin",
      "believe","bring","build","buy","call","can","change",
      "come","consider","continue","could","create","cut",
      "die","do","expect","fall","feel","find","follow","get",
      "give","go","grow","happen","have","hear","help","hold",
      "include","keep","kill","know","lead","learn","leave",
      "let","like","live","look","lose","love","make","may",
      "mean","meet","might","move","must","need","offer",
      "open","pay","play","provide","put","reach","read",
      "remain","remember","run","say","see","seem","send",
      "serve","set","should","show","sit","speak","spend",
      "stand","start","stay","stop","take","talk","tell",
      "think","try","turn","understand","use","wait","walk",
      "want","watch","will","win","work","would","write"};
    static size_t theSize = sizeof(theWords)/sizeof(char*);
    return theWords[(rand() % theSize)];
  }

  std::string Fake::Writing::adjective() {
    static const char* theWords[] = {
      "able","appropriate","available","bad","best","better",
      "big","black","blue","central","certain","clear","close",
      "cold","common","current","dark","dead","democratic",
      "different","difficult","early","easy","economic","entire",
      "environmental","federal","final","financial","fine",
      "foreign","free","full","general","good","great","green",
      "happy","hard","high","hot","huge","human","important",
      "international","large","late","left","legal","likely",
      "little","local","long","low","main","major","medical",
      "military","national","natural","new","nice","old","only",
      "open","other","past","personal","physical","political",
      "poor","popular","possible","private","public","ready",
      "real","recent","red","religious","right","serious",
      "short","significant","similar","simple","single","small",
      "social","special","strong","sure","traditional","true",
      "ultural","various","white","whole","wrong","young"};
    static size_t theSize = sizeof(theWords)/sizeof(char*);
    return theWords[(rand() % theSize)];
  }

  std::string Fake::Writing::word() {
    switch(rand() % 21) {
      case 0 : return predetermine();
      case 1 : return determiner();
      case 2 : return demonstrative();
      case 3 : return definite();
      case 4 : return indefinite();
      case 5 : return distributive();
      case 6 : return difference();
      case 7 : return pronoun();
      default:
        switch(rand() % 4) {
          case 0 : return verb();
          case 1 : return adjective();
          default: return noun();
        }
    }
    return noun();
  }

  std::string Fake::Writing::fakeword() {
    size_t theLen = 2+(rand() % 6);
    std::string theResult;
    for(size_t thePos=0;thePos<theLen;thePos++) {
      theResult+=(char)('a' + (rand() % 26));
    }
    return theResult;
  }

  std::string Fake::Writing::sentence(size_t maxwords) {
    size_t theMax = std::max((size_t)20,maxwords);
    std::string theResult;
    static const char* thePrefix[]={""," "};
    for(size_t thePos=0;thePos<theMax;thePos++) {
      theResult+=thePrefix[thePos>0]+word();
    }
    theResult[0]=toupper(theResult[0]);
    return theResult+".";
  }

  void Fake::Writing::autoTest(std::ostream &anOut, size_t aMax) {
    for(size_t i=0;i<aMax;i++) {
      anOut << book_title() << "\n";
      anOut << author() << "\n";
      anOut << author_first() << "\n";
      anOut << author_last() << "\n";

      anOut << determiner() << "\n";
      anOut << predetermine() << "\n";
      anOut << demonstrative() << "\n";
      anOut << definite() << "\n";
      anOut << indefinite() << "\n";
      anOut << distributive() << "\n";
      anOut << difference() << "\n";

      anOut << pronoun() << "\n";
      anOut << noun() << "\n";
      anOut << verb() << "\n";
      anOut << adjective() << "\n";
      anOut << word() << "\n";
      anOut << fakeword() << "\n";
      anOut << sentence() << "\n";
    }
  }
}
