Trash Mover
============

Author
------
KangDroid[Jason.HW.Kang]

Description[one-line]
-----------
The File/Folder Trash Management Program.

Details
-------
This was developed because I, sometimes accidentally delete some important forlders like containing personal pictures/memories with rm command. Since my habit is to use "rm -rf", therefore it is kinda almost impossible to recover that files.<br>
(Technically, As soon as I rm-ed files, I could make an image of that disk, and run with autopsy or something forencic tools, but I do not have some external disk that could contain 1tb of image.)<br>

So I came in to question, "What if I can manage trash like other OS does?"<br>
For example: in macOS, if we delete some files with "Command-Delete" in Finder, the files/foders are moved to its trash, usually located on `/Users/$USERS/.Trash`. If we invoke "Command-Z" from finder, it will restore[or cancel for move] latest content from trash.<br>
Same as Windows. If we delete some files/folders, and if we want to restore it, we can access Trashcan and hit "Restore" button. <br>

For users like me, who use CLI a lot then GUI, that Trashcan solution is not very ideal.
Maybe we can move files to specific folders with mv command, but that is not track-able, meaning we cannot restore content to original place in one-click[or command]. I.e  we need to find original place to restore, move, etc..<br>

This program, will help you to manage your personal trashcan, which we can move files/folders to trashcan, and restore it to original place within this program.<br>

The logic[How it works?]
-----------------------
The logic is pretty much simple.

When Removing files:
- we record some information about that file to "TrashData", which is:<br>
  - original location, 
  - deletion time, 
  - Trash location[Location in trashcan], 
  - argument list when executed, 
  - CWD when executed.
- Those information could be seen with `./TrashManager -s`
- Move files/folder to desired trashcan.

When Restoring files:
- Lookup "TrashData" and check whether it is deprecated or not.
- if NOT deprecated, move from trashcan to original location.
- We do not remove its trashdata, but program will mark as "deprecated".
- If you want to remove its deprecated trashdata contents, invoke `./TrashManager -C`

The TrashData[Why this exists?]
---------------------------------
TrashData its form look as follows:
- $TIME = EPOCH Timestamp
- $ARGS = Command Line Arguments when deleted.
- $CWD = Current Working Directory when program executed
- $ORIG = Original file location when removed.
- $TRASH = Trash location[with deleted files]

File may look as follows:<br>
`$TIME    $ARGS    $CWD    $ORIG    $TRASH` <br>
See TrashManager's Destructor code : [TrashManager.cpp#L522](https://github.com/KangDroid/trash_mover/blob/6f9e9751c069d7caf9714f9807ec5efc1b01d39e/src/TrashManager.cpp#L522) <br>

As you might notice within source code, this program will lookup this trash data to restore files from trash - to original place. Also sometimes[especially me], we need to look up "delete histories" because we forget what, when we deleted files. <br>

Trash Data is stored in your wish, you can give compiler arguments option to store.[See building] By default, it will use `/Users/$USERS/.trash_lists`.

BUILDING - Available CMAKE Options
--------
|Options|Meaning|Example|
|:----|:----------|:-------------|
|CUSTOM_TRASH_LOCATION|Specify Custom TRASH Location[folder]|`-DCUSTOM_TRASH_LOCATION=/home/kangdroid/custom_trash`|
|CUSTOM_TRASHDATA_LOCATION|Specify Custom TRASHDATA Location[folder]|`-DCUSTOM_TRASHDATA_LOCATION=/home/kangdroid/custom_trashdata`|

BUILDING - macOS
-----------------
macOS[Big Sur]<br>
You need to have:
- C++17 - standard available compiler.<br>
  Such as GCC-7~GCC-10, CLANG, etc.
- CMAKE version > 3.15
- Git[Command]

In Big Sur, mostly compiler/git is already installed if you installed `Command-Line-Tools`. Only you need to install is cmake. It could be done easily by [HomeBrew](https://brew.sh/), by executing:
```
$ brew install cmake
```

To build, simply invoke CMAKE command and make it.
```
$ cd $SOURCE_DIR
$ mkdir build
$ cd build
$ cmake ..
$ make -j4
```

And the compilation result will be named as "TrashManager", and ready to execute it!

BUILDING - Linux[Ubuntu]
------------------
Ubuntu 20.04[tested]<br>
You need to have:
- C++17 - standard available compiler.<br>
  Such as GCC-7~GCC-10, CLANG, etc.
- CMAKE version > 3.15
- Git[Command]

Most of them could be installed within this command:
```
$ sudo apt install build-essential cmake
```

<u><b>For building LINUX, ALL Users MUST DEFINE `CUSTOM_TRASH_LOCATION` and `CUSTOM_TRASHDATA_LOCATION` in order to work!</b></u><br>
Usually if you are using GUI-Ubuntu[Ubuntu Desktop], trashcans are located on /home/$USERS/.local/share/Trash, but if you are using Server-Ubuntu[Ubuntu Server], you need to create your own trashcan folder and define with CUSTOM_TRASH_LOCATION. <br>

Anyway, ALL linux user must define CUSTOM_TRASH_LOCATION/CUSTOM_TRASHDATA_LOCATION manually!<br>
For example, let:
- Trashdir = /home/ubuntu/trash_locatoin
- TrashDatadir = /home/ubuntu/.trashdata

Cmake command would be:
```
$ cmake .. -DCUSTOM_TRASH_LOCATION=/home/ubuntu/trash_location -DCUSTOM_TRASHDATA_LOCATION=/home/ubuntu/.trashdata
```