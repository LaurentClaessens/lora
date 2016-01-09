________________________________________- LORA __ A BACKUP AND GIT AID PROGRAM __________________________

Lora will loop over your $HOME directory and 
 - backup it. 
 - check if some subdirectories need some 'git commit' or 'git add'. A small graphical interface help you maintaining your git repositories.




This is libre software. Feel free to contribute, report bugs and donate (in the sense of free beer).



INSTALLATION AND COMPILATION

Download 
    git clone https://github.com/LaurentClaessens/lora
Compile the installation program
    make installation
Launch the installation program
    ./installation
Compile the whole
    make all

NOTES
 
    You can bypass the installation if Boost is correctly located (see where the makefile assumes it is).  
Doing directly
        make all
works at my place (Ubuntu 15.10 Wily) 

    You can directly modify the 'lora.cfg' and 'makefile' file instead of using the installation program. 


DEFAULT USE

./lora 

will backup your $HOME directory using the backup and purge paths found in 'lora.cfg'

BACKUP A PART

./lora ~/foo

will backup the directory ~/foo in   <backup>/foo   where <backup> is the directory found in 'lora.cfg'. This contains the backup of $HOME.


USE AN OTHER CONFIG FILE

./lora --configuration=myconfig.cfg

will read the configuration file 'myconfig.cfg' instead of 'lora.cfg'


WHAT IS DOES -- BACKUP

The main feature is : the backup copy is only a copy. You can browse it with your favorite file browser and get a file back with a simple copy. You do not require a special software for getting back your data.

As an example, let us show how work the backup of $HOME=/home/daniel

We choose :
    backup directory : /mnt/baka/backup
    purge directory  : /mnt/baka/purge

Typically, /mnt/baka is the mount point of an encrypted external disk.


This software loops over the files and repertories of /home/daniel and creates a copy into
/mnt/baka/backup

At the end of the process, the directories /home/daniel and /mnt/baka/backup are the same (except for the excluded directories).

When making the backup of /home/daniel/bar/foo.txt,
* check if /mnt/baka/backup/bar/foo.txt
* if not, create it with a simple copy (but the last write time attribute is conserved)
* if /mnt/baka/backup/bar/foo.txt exists and is different
    - move to /mnt/backup/bakapurge.lora/<today date>/<hour>/modified/bar/foo.txt   (keeping the time attribute)
    - make the copy.

Thus the backup is incremental in the sense that the old version of a modified files is still available in a directory whose name is the date and the hour of the backup.



ATTENTION : There is no mechanism to suppress old file. Thus the size of directory /mnt/baka/purge is always increasing. This is by purpose : I prefer not have an automated way to suppress data.
    Consequence : some segmentation faults are triggered by "disk full". This is the user's responsibility to know what to suppress.


WHAT IT DOES -- PURGE

Lora loops over the directory /mnt/baka/backup and checks if these files correspond to files in /home/daniel.
When looking at /mnt/backa/backup/blah/stuff.tex

* check if /home/daniel/blah/stuff.tex exists
 - if yes, do nothing
 - if not, assume that this file was removed in the home and then move
/mnt/baka/backup/blah/stuff.tex ---> /mnt/baka/purge/<today date>/<hour>/removed/blah/stuff.tex


By the way, Lora has very bad performance against renaming an intere directory : in the first pass, it copy everything and in the second pass, it move the whole to the purge directory.

WHAT IT DOES -- GIT

Since Lora loops over the whole $HOME, it also take time to check for each directory if it is a git repository (has non trivial .git subdirectory) and if this repository
is clean.

A list of directories that are not clean git repository (untracked or modified files) is displayed. Clicking on one of them opens a dialog window that helps you to 
  - add files in .gitignore
  - add files
  - make a 'git commit -a'
  - see git diff
  - ...


TODO

- the purge vector is parsed too many times
- a better command line argument parser
- a better image for "plus.png"
- the installation program should read the configuration file an fill the blanks.
- in the installation, there is a duplicate of information between   BackupWidget::exclude_list and ExcludeChooser::choosers.
- make a version with no graphical interface. One should be able to do the backup even from the most basic debbootstraped USB stick.
