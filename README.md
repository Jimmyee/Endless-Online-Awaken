# Endless-Online-Awaken
This is Endless Online client/server clone. 


Endless Online Awaken is an open source client/server for Endless Online. It is intended to support all features of the original Endless Online v28 and to be continuation of this game.


### Current version: 0.0.1 pre-alpha


Please see changelog.txt for more information about updates.


# Setup guide
## Client build requirements:
* MinGW 6.1+ (https://sourceforge.net/projects/mingw/files/Installer/)

* Allegro 5.2.3.0

* SFML 2.4.2 Network and System modules.

## Server build requirements:
* MinGW 6.1+ (https://sourceforge.net/projects/mingw/files/Installer/)

* SFML 2.4.2 Network and System modules.


* Project files for Code::Blocks coming soon :)

## How to run client:
```
To run the game client you need to put decompressed EO graphics in client_engine/data/gfx/ folder. You also need to create config.ini in client_engine/data/ folder; config-example.ini may be helpful for that.
```
## How to run server:
```
In order to run the game server you need to create config.ini. There's also config-example.ini for the game server.
You need to create new SQLite3 database. (example database: server_engine/data/eo.db).
```

### Have fun!


This project is released under MIT licence. See LICENCE file for more info.

Copyright © Jimmyee (Karol Woszczek) 2017.
