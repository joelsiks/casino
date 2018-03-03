
# Dr. Joel Casino

![Casino for life. rite?](https://raw.githubusercontent.com/joelsmithz/DuckyToC/casino/game.png "Dr. Joel Casino")

## Building from source

To build the game you need to have a C++ compiler that supports at least c++11 and above, ncurses, make and git installed and working.
For some reason I can't get the project to compile with any other compiler than g++-7 (g++ version 7.1.0). So make sure you've g++ version 7 or above.

To get ncruses on Ubuntu, execute the following command:

```
sudo apt-get install libncurses-dev
```

When everything's ready to go, clone the repository and build it using the 'make' command.

```
git clone https://github.com/joelsmithz/casino.git
make
```

Voilà! The game's ready.


## Games

 - Roulette, options [Red, Black, Green]
 - Slots, three in a row wins.
 - Dice, options [Even, Odd, Three of a kind, Above 12, Below 10]

