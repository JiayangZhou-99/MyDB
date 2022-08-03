# MyDB
A Transaction level Concurrent SQL Server


# Code Structure Of SQL Server

```
── CMakeLists.txt
├── CMakeSettings.json
├── makefile
├── scripts
│   ├── final.txt
│   ├── test1.txt
│   └── test2.txt
├── src
│   ├── CMakeLists.txt
│   ├── Controller
│   │   ├── Application.cpp
│   │   ├── Application.hpp
│   │   ├── CommandProcessors
│   │   ├── Locker
│   │   ├── Managers
│   │   └── ParserHelpers
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
│   │   └── Storage
│   ├── Server
│   │   ├── SQLServer.cpp
│   │   ├── SQLServer.hpp
│   │   └── Socket
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
├── client
├── client.cpp
├── makefile
└── tools
    └── build.sh
    
```

# Architecture Overview

<img width="1154" alt="image" src="https://user-images.githubusercontent.com/63135646/182532962-d10673c2-b574-46d3-9a0b-d2408da39c4f.png"style="zoom:50%;">



    
