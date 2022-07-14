# MyDB
A Transaction level Concurrent SQL Server


# Code Structure Of SQL Server

```
├── CMakeLists.txt
├── CMakeSettings.json
├── makefile
├── src
│   ├── CMakeLists.txt
│   ├── Controller
│   │   ├── Application.cpp
│   │   ├── Application.hpp
│   │   ├── CommandProcessors
│   │   │   ├── ApplicationProcessor.cpp
│   │   │   ├── ApplicationProcessor.hpp
│   │   │   ├── CommandProcessor.hpp
│   │   │   ├── CommandProcessors.hpp
│   │   │   ├── DatabaseProcessor.cpp
│   │   │   ├── DatabaseProcessor.hpp
│   │   │   ├── SQLProcessor.cpp
│   │   │   └── SQLProcessor.hpp
│   │   ├── Locker
│   │   │   ├── LockManager.cpp
│   │   │   ├── LockManager.hpp
│   │   │   └── Transaction.hpp
│   │   ├── Managers
│   │   │   ├── ApplicationManager.cpp
│   │   │   ├── ApplicationManager.hpp
│   │   │   ├── DatabaseInstanceManager.cpp
│   │   │   ├── DatabaseInstanceManager.hpp
│   │   │   ├── DatabaseManager.cpp
│   │   │   ├── DatabaseManager.hpp
│   │   │   ├── Manager.hpp
│   │   │   ├── Managers.hpp
│   │   │   ├── SQLManager.cpp
│   │   │   └── SQLManager.hpp
│   │   └── ParserHelpers
│   │       ├── Compare.hpp
│   │       ├── ParseHelper.cpp
│   │       ├── ParseHelper.hpp
│   │       ├── Scanner.hpp
│   │       ├── Tokenizer.cpp
│   │       └── Tokenizer.hpp
│   ├── DefaultTests
│   │   ├── Faked.cpp
│   │   ├── Faked.hpp
│   │   ├── ScriptRunner.hpp
│   │   ├── TestAutomatic.hpp
│   │   ├── TestManually.hpp
│   │   └── TestSequencer.hpp
│   ├── Model
│   │   ├── Alter.hpp
│   │   ├── Attribute.cpp
│   │   ├── Attribute.hpp
│   │   ├── Database.cpp
│   │   ├── Database.hpp
│   │   ├── Entity.cpp
│   │   ├── Entity.hpp
│   │   ├── Log.cpp
│   │   ├── Log.hpp
│   │   ├── Row.cpp
│   │   ├── Row.hpp
│   │   ├── SQLQuery.cpp
│   │   ├── SQLQuery.hpp
│   │   ├── Statements
│   │   │   ├── ApplicationStatements
│   │   │   │   ├── ApplicationStatements.hpp
│   │   │   │   ├── HelpApplicationStatement.hpp
│   │   │   │   ├── QuitApplicationStatement.hpp
│   │   │   │   ├── RunScriptStatement.hpp
│   │   │   │   └── VersionApplicationStatement.hpp
│   │   │   ├── DatabaseStatements
│   │   │   │   ├── CreateDatabaseStatement.hpp
│   │   │   │   ├── DatabaseStatements.hpp
│   │   │   │   ├── DropDatabaseStatement.hpp
│   │   │   │   ├── DumpDatabaseStatement.hpp
│   │   │   │   ├── LogDatabaseStatement.hpp
│   │   │   │   ├── ShowDatabasesStatement.hpp
│   │   │   │   └── UseDatabaseStatement.hpp
│   │   │   ├── SQLStatements
│   │   │   │   ├── DeleteStatement.hpp
│   │   │   │   ├── IndexStatement.hpp
│   │   │   │   ├── InsertStatement.hpp
│   │   │   │   ├── SQLStatements.hpp
│   │   │   │   ├── SelectStatement.hpp
│   │   │   │   └── UpdateStatement.hpp
│   │   │   ├── Statement.hpp
│   │   │   ├── Statements.hpp
│   │   │   └── TableStatements
│   │   │       ├── AlterTableStatement.hpp
│   │   │       ├── CreateTableStatement.hpp
│   │   │       ├── DescribeTableStatement.hpp
│   │   │       ├── DropTableStatement.hpp
│   │   │       ├── ShowTablesStatement.hpp
│   │   │       └── TableStatements.hpp
│   │   └── Storage
│   │       ├── BPlusTree.cpp
│   │       ├── BPlusTree.hpp
│   │       ├── Blocks
│   │       │   ├── Block.cpp
│   │       │   ├── Block.hpp
│   │       │   ├── Blocks.hpp
│   │       │   ├── DataBlock.cpp
│   │       │   ├── DataBlock.hpp
│   │       │   ├── EntityBlock.cpp
│   │       │   ├── EntityBlock.hpp
│   │       │   ├── FreeBlock.cpp
│   │       │   ├── FreeBlock.hpp
│   │       │   ├── MetaHeaderBlock.cpp
│   │       │   └── MetaHeaderBlock.hpp
│   │       ├── CustomLinkedList.hpp
│   │       ├── LRUCache.hpp
│   │       ├── LogBlocks
│   │       │   ├── LogMetaBlock.cpp
│   │       │   ├── LogMetaBlock.hpp
│   │       │   ├── OperationBlock.cpp
│   │       │   └── OperationBlock.hpp
│   │       ├── Predefined.hpp
│   │       ├── Storage.cpp
│   │       └── Storage.hpp
│   ├── Server
│   │   ├── SQLServer.cpp
│   │   ├── SQLServer.hpp
│   │   └── Socket
│   │       ├── PracticalSocket.cpp
│   │       └── PracticalSocket.hpp
│   ├── Util
│   │   ├── BasicTypes.hpp
│   │   ├── Config.hpp
│   │   ├── Errors.hpp
│   │   ├── FolderReader.hpp
│   │   ├── FormatString.hpp
│   │   ├── Helpers.hpp
│   │   ├── Timer.hpp
│   │   └── keywords.hpp
│   ├── View
│   │   ├── FolderView.hpp
│   │   ├── QueryResultView.hpp
│   │   ├── TableView.cpp
│   │   ├── TableView.hpp
│   │   ├── TimerView.hpp
│   │   └── View.hpp
│   ├── build
│   │   ├── CMakeCache.txt
│   │   └── CMakeFiles
│   │       ├── 3.23.1
│   │       │   ├── CMakeCCompiler.cmake
│   │       │   ├── CMakeCXXCompiler.cmake
│   │       │   ├── CMakeDetermineCompilerABI_C.bin
│   │       │   ├── CMakeDetermineCompilerABI_CXX.bin
│   │       │   ├── CMakeSystem.cmake
│   │       │   ├── CompilerIdC
│   │       │   │   ├── CMakeCCompilerId.c
│   │       │   │   ├── a.out
│   │       │   │   └── tmp
│   │       │   └── CompilerIdCXX
│   │       │       ├── CMakeCXXCompilerId.cpp
│   │       │       ├── a.out
│   │       │       └── tmp
│   │       ├── CMakeOutput.log
│   │       ├── CMakeTmp
│   │       ├── VerifyGlobs.cmake
│   │       ├── cmake.check_cache
│   │       └── cmake.verify_globs
│   └── main.cpp
├── tests
│   ├── AutograderTests.cpp
│   ├── CMakeLists.txt
│   ├── CustomTests.cpp
│   ├── SanityCheck.cpp
│   └── main.cpp
└── tools
    ├── RunLinter.py
    └── build.sh


```


# Code Structure Of SQL Client


```

├── CMakeLists.txt
├── PracticalSocket.cpp
├── PracticalSocket.hpp
├── client.cpp
├── makefile
└── tools
    └── build.sh
    
```
