
# LORA : a backup and git aid program

Lora will loop over your $HOME directory and 
* backup it. 
* check if some subdirectories need some 'git commit' or 'git add'. A small graphical interface help you maintaining your git repositories.

This is libre software. Feel free to contribute, report bugs and donate (in the sense of free beer).


## INSTALLATION AND COMPILATION

* Download 

        git clone https://github.com/LaurentClaessens/lora

* Configure
   * Create `lora.cfg` taking example on `example.cfg` (self-explaining)
   * Adapt the paths MOC_BIN and BOOST_THREAD_LIB  in `makefile`.
* Compile the whole and test :

        make all
        ./tests.sh

* Read the manual
    in lora/documentation/manual.pdf

# DEFAULT USE

`./lora` will backup your $HOME directory using the backup and purge paths found in 'lora.cfg'


# USE AN OTHER CONFIGURATION FILE

For reading the configuration file `myconfig.cfg` instead of `lora.cfg` :

        ./lora --configuration=myconfig.cfg

# WHAT IS DOES -- BACKUP

The main feature is : the backup copy is only a copy. You can browse it with your favorite file browser and get a file back with a simple copy. You do not require a special software for getting back your data.

As an example, let us show how work the backup of $HOME=/home/daniel

We choose :

*  backup directory : `/mnt/baka/backup`
*  purge directory  : `/mnt/baka/purge`

Typically, `/mnt/baka` is the mount point of an encrypted external disk.

Lora loops over the files and repertories of `/home/daniel` and creates a copy into `/mnt/baka/backup`.

At the end of the process, the directories `/home/daniel` and `/mnt/baka/backup` are the same (except for the excluded directories).

When making the backup of the file `/home/daniel/bar/foo.txt`,

* check if /mnt/baka/backup/bar/foo.txt exists
* if not, create it with a simple copy (but the last write time attribute is conserved)
* if /mnt/baka/backup/bar/foo.txt exists and is different
    - move to /mnt/backup/bakapurge.lora/<today date>/<hour>/modified/bar/foo.txt   (keeping the time attribute)
    - make the copy.

Thus the backup is incremental in the sense that the old version of a modified files is still available in a directory whose name is the date and the hour of the backup.



ATTENTION : There is no mechanism to suppress old file. Thus the size of directory /mnt/baka/purge is always increasing. This is by purpose : I prefer not have an automated way to suppress data.
    Consequence : some segmentation faults are triggered by "disk full". This is the user's responsibility to know what to suppress.

Trying to exclude a non-existing directory throws an error (when reading the configuration file). The reason roots in the following user case. Let suppose I don't want to
backup /home/daniel/videos/dvd (containing .VOB files) because it is very large and not really important. I add the line
exclude=/home/daniel/videos/dvd 
in the configuration file.
Now I change the directory name to /home/daniel/vidéos/dvd  (add an accent on "vidéo"). Then the backup process will copy the whole directory /home/daniel/vidéo and purge <purge>/videos. This is yet not really optimal, but copying the subdirectory /home/daniel/vidéos/dvd is REALLY what we don't want to do. 

When something strange happens, better to crash than silently manage the situation.


WHAT IT DOES -- PURGE

Lora loops over the directory /mnt/baka/backup and checks if these files correspond to files in /home/daniel.
When looking at /mnt/backa/backup/blah/stuff.tex

* check if /home/daniel/blah/stuff.tex exists
 - if yes, do nothing
 - if not, assume that this file was removed in the home and then move
/mnt/baka/backup/blah/stuff.tex ---> /mnt/baka/purge/<today date>/<hour>/removed/blah/stuff.tex


By the way, Lora has very bad performance against renaming an intere directory : in the first pass, it copy everything and in the second pass, it move the whole to the purge directory.

WHAT IT DOES -- GIT

Since Lora loops over the whole $HOME, it also takes time to check for each directory if it is a git repository (has non trivial .git subdirectory) and if this repository
is clean.

A list of directories that are not clean git repository (untracked or modified files) is displayed. Clicking on one of them opens a dialog window that helps you to 
  - add files in .gitignore
  - add files
  - make a 'git commit -a'
  - see git diff
  - ...


