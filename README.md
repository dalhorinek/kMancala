# kMancala game 

This is implementation of mancala game using kde-games. 

![kmancala](https://raw.githubusercontent.com/dalhorinek/kMancala/master/screenshort.png "kMancala Game")

## How to install

### On debian/ubuntu
You have to install package `libkdegames-dev` 

```
mkdir build 
cd build 
cmake ..
make 
su -c 'make install' # or sudo make install 
``` 

Then you can run the application by typing kmancala to terminal or it should appear in applications menu
