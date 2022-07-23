

#ifndef Errors_h
#define Errors_h

#include <stdint.h>

namespace MyDB {

	enum Errors {
		//parse related...
		keywordExpected = 100,
		identifierExpected = 110,
		illegalIdentifier = 115,
		unknownIdentifier = 120,
		unexpectedIdentifier = 125,
		joinTypeExpected = 130,
		keyExpected = 135,
		syntaxError = 140,
		unexpectedKeyword = 145,
		unexpectedValue = 150,
		valueExpected = 155,
		operatorExpected = 160,
		punctuationExpected = 165,
		eofError = 199,

		//statement related...
		statementExpected = 200,
		noDatabaseSpecified = 210,

		//table/DB related...
		tableExists = 300,
		unknownEntity = 310,
		unknownTable = 320,
		unknownDatabase = 330,
		databaseExists = 340,
		databaseCreationError = 350,
		primaryKeyRequired = 360,
        tooManyPrimaryKeys  = 380,
		databaseOpenError = 370,

		//type related...
		unknownType = 400,
		unknownAttribute = 405,
		invalidAttribute = 410,
		invalidArguments = 420,
		keyValueMismatch = 430, //# of fieldnames doesn't match values...

		//storage/io related...
		readError = 500,
		writeError = 510,
		seekError = 520,
		storageFull = 530,
		deleteError = 540,

		//index related
		indexExists = 600,
		cantCreateIndex = 605,
		unknownIndex = 610,

		//command related...
		unknownCommand = 3000,
		invalidCommand = 3010,

		//general purpose...
		userTerminated = 4998,
		notImplemented = 4999,
		noError = 5000,

		//transaction related
		transactionAborted = 6001,

		//log related
		canNotUndoAnyMore = 7001,
		canNotRedoAnyMore = 7002,

		//script related
		scriptError = 8001,


		unknownError = 10000
	};

	struct StatusResult {
		Errors    error;
		uint32_t  value;

		StatusResult(Errors anError = noError) :
			error(anError), value(0) {}
		operator bool() { return Errors::noError == error; }
		bool operator==(Errors anError) { return anError == error; }
	};

}

#endif /* errors */
