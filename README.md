# Asstest - An assistant for preparing for music listening tests

## Prologue

Long have students of various musical institutions been plagued by the 
listening test, an artifice conceived by the most cunning and evil of 
pedagogues in aid of their endless battle against the students.

The devilish design of the listening test is devised so that there is a 
substantial minimum amount of time (often many hours) the target student must 
spend listening to music.

Being similarly afflicted by endless listening tests, the author has taken
upon himself to devise a weapon to counter somewhat this vile contrivance.
The author has locked himself in isolation and laboured long, perfecting the
weapon. And this weapon is... Asstest (the author apologises for the name,
conceived during times of anger and sorrow)!

The author cannot reduce the minimum amount of time it take to be well 
prepared. However, the author endeavors to optimize the process somewhat.

## Functionality

At the moment, this software only works in Linux with the cvlc command 
installed (the author believes it to be contained in the vlc package).

So here's how you use this thing:

- Compile the binary (see section below)
- Copy the binary (called `main`) into a directory full of .mp3 files
- Open a terminal in this directory and run `./main`
- Tada! Follow the instructions in the terminal. 

In the current version of the program, it is sufficient to input the first few
letters of your guess.

This program is rather basic - it plays a random mp3 from those detected
in the directory from a random point that is guaranteed to be sufficiently
far from the end of the recording.

This program does require the files to be present on the machine. 
Occasionally, the author has encountered tests consisting of YouTube links.
The author knows a... friend... who has successfully used Asstest upon first
obtaining the mp3's using a Python script.

## Compiling

In order to compile the binary, open a terminal in the local instance of this
git repo and simply run `make`. If you are among those lucky enough to have
a system compatible with mine, it should spit out a file called 'main'.

## Improvements

The author realizes that as it stands, this software has a number of flaws.
Lacking the impetus to format it all as git issues, the author instead elects
to place them here as a list.

- Implement fuzzy search for guesses, instead of matching first few chars
- Provide mechanism for resolving ambiguities when multiple search results
- Make it possible to specify a mp3 directory
- Make code less monolithic (and organize directory)
- Portability
