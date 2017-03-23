
# LORA : a backup program

## Why an other backup software ?

### My requirements

Here are my requirements for a backup program.

* A backup program has to work with no graphical interface. Because you want to backup especially when your system is completely crashed, from ssh or by booting from an minimal USB key.
* A backup is not a synchronization. 
* The files have to be recoverable by a simple copy : you do not need the backup program itself for retrieving the data.

Let us be clear : the best backup way is a brand new full copy of your home everyday in a new directory, on an encrypted external disk. This is doable when your home does not exceed a couple of gigis.

### My solution

For the example, let's say I'm making a backup on January 12, 2017 at 09h23 in the directory `/mnt/backup`. This is typically the mount point of an external encrypted disk. Lora will use (and eventually create) the tree following directories :
```bash
/mnt/backup/bakatot
/mnt/backup/bakapurge/2017-January-12/09h23/modified
/mnt/backup/bakapurge/2017-January-12/09h23/removed
```

The first is the backup itself : it will be a copy of you home. The backup loop is the following.

* **First pass : the backup** for each file on you home, look for the corresponding file in `/mnt/backup/bakatot`. 
   * If they are the same (size and last modification time) do nothing
   * If the file does not exist in the backup, copy.
   * If the file exists in the backup but is different :
       * move the backup file to `/mnt/backup/bakapurge/2017-January-12/09h23/modified/path-to-the-file` (so the `modified` directory will contain a partial copy of the directory structure of your home : the parts where there are modifications)
       * copy from home to backup.

     This way, a modified file does not really erase the backup : you keep a copy of the old version.

* **Second pass : the purge** For each file in the backup : check if the corresponding one in the home exists.
   * If it exists, do nothing.
   * If it does not exist, we conclude that this file was deleted between the last backup and the present time. So Lora moves from the backup to `/mnt/backup/bakapurge/2017-January-12/09h23/removed/path-to-the-file`

At this point the directory `/mnt/backup.bakatot` contains a copy of your home. The directory `/mnt/backup/bakapurge` contains a quite well sorted copy of all the old versions of you files, including the deleted ones.

Let is insist on one point : the backup directory is a simple copy : you can browse it with any file browser you want, you can edit the files with any editor you want, and to can retrieve them by hand with a simple copy.

### Small (shameless) hack for the git object files

I'm not sure of the "why", but it turns out that when I `git clone` a repository from one directory to an other of my home, I get in the cloned directory some object files (in `.git/objects/`) with a wrong `last_write` time. The time in the backup of the cloned directory is more recent than the date in the cloned directory.

It may be due to something like `git pull` rewrites the files with the time of the "pulled" directory while `git clone` creates the files with the time of the clone.

Anyway ...

For the files whose path contains `/.git/objects/`, if the size are identical, the copy is done even if the date in the backup is more recent than the one in the home.

Some remarks :
* For other files, such a situations throws an exception.
* Lora is in fact not checking that the "problematic" file is actually a git object file. It only checks that the filename contains the string `/.git/objects/`.
* If you are working in a directory that, by pure chance, contains that string, Lora will not detect time problems.

## INSTALLATION AND COMPILATION

* Download 

        git clone https://github.com/LaurentClaessens/lora

* Configure
   * Create `lora.cfg` taking example on `example.cfg` (self-explaining)
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

By the way, Lora has very bad performance against renaming a directory : in the first pass, it copy everything and in the second pass, it move the whole to the purge directory.
