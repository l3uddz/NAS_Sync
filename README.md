NAS_Sync
==========

NAS_Sync is a small win32 utility who's sole purpose is to sync files from your computer, to your NAS drive (via FTP).
It was designed with the goal of saving some time/effort when it comes to uploading your media files to your NAS. If you are like me, and you have 10 or so shows being downloaded each day, it becomes rather tiresome of manually uploading each file to your NAS day after day after day. 
NAS_Sync will help save your time & effort as all you have to-do is a one time configuration file and simply run NAS_Sync when you want it to sync your files to your NAS...

Even if one person appreciates it and uses it, then NAS_Sync was a success! 

All coders are welcome! I am sure there are allot of improvements that can be made.

Configuration
=============

[ftp]
addr=mybooklive
port=21
user=admin
pass=secret

[tv]
local=D:\Media\complete\TV
remote=/Public/Shared Videos/TV
ignore=Elementary -,Revenge -
accept=.mp4,.m2ts,.avi

[movies]
local=D:\Media\complete\Movies
remote=/Public/Shared Videos/Movies
accept=.mp4,.avi,.m2ts,.jpg

[music]
local=D:\Media\complete\hp
remote=/Public/Shared Music/Music
accept=.mp3,.jpg


NAS_Sync requires a config.ini file, it MUST be located in the same folder as the NAS_Sync executable. Above is an example configuration.
The ftp section is mandatory, how else will it upload your files to the NAS Drive??
The sections below the ftp section, will be considered category's. This tells NAS_Sync where to look for media files, and where to store them on the remote FTP server (NAS Drive).
The "local/remote/accept" settings are mandatory! The "ignore" setting is optional.

Please note:
All files that are uploaded, are uploaded based on the location/structure the file was found at/in. So as an example, based on the tv category above, NAS_Sync has found the following file:
D:\Media\complete\TV\Crossbones\Season 01\Crossbones - 1x01 - Episode Name.mp4
This will be uploaded to /Public/Shared Videos/TV/Crossbones/Season 01/Crossbones - 1x01 - Episode Name.mp4
If the Crossbones/Season 01 folder does not exist, they will be created and the upload will commence.

Logic
=====

NAS_Sync will build a vector/list of all media files found in the folder specified via the "local" setting, based on the category you have told it to use (you will be asked which category you want to use upon executing NAS_Sync.exe).
It will then connect to the FTP server and traverse the found media files vector/list, performing some very basic checks to determine if the file should be skipped or uploaded.
First it will check whether the local media files remote folder exists (e.g. /Public/Shared Videos/TV/Crossbones/Season 01), if they do not, then it is deemed the file should be uploaded, if the folders do exist, it will then traverse the folder to check whether the file is already uploaded.. If NAS_Sync finds the media file is already stored on the FTP server, it will perform a size comparison, if the sizes match then the file is skipped whereas if the sizes do not match it is deemed that the file should be uploaded (overwriting the file that is already stored on the FTP server).

Example Session
===============

Server: mybooklive
Port: 21
User: admin
Pass: secret

Please enter the category to use: tv
Scanning for media in: "D:\Media\complete\TV"

Accepted: 24 - 9x08 - Day 9- 6-00 P.M. - 7-00 P.M.mp4
Accepted: 24 - 9x09 - Day 9- 7-00 P.M. - 8-00 P.M.mp4
Accepted: 24 - 9x10 - Day 9- 8-00 P.M. - 9-00 P.M.mp4
Accepted: Anger Management - 2x61 - Charlie, Lacey and the Dangerous Plumber.mp4

Accepted: Anger Management - 2x62 - Charlie and the Mother of All Sessions.mp4
Accepted: Arrow - 2x23 - Unthinkable.mp4
Accepted: Beauty and the Beast (2012) - 2x17 - Beast Is The New Black.mp4
Accepted: Beauty and the Beast (2012) - 2x18 - Cat and Mouse.mp4
Accepted: Beauty and the Beast (2012) - 2x19 - Cold Case.mp4
Accepted: Beauty and the Beast (2012) - 2x20 - Ever After.mp4
Accepted: Beauty and the Beast (2012) - 2x21 - Operation Fake Date.mp4
Accepted: Believe - 1x10 - Collapse.mp4
Accepted: Believe - 1x11 - Revelation.mp4
Accepted: Believe - 1x12 - Second Chance.mp4
Accepted: Bones - 9x23 - The Drama in the Queen.mp4
Accepted: Bones - 9x24 - The Recluse in the Recliner.mp4
Accepted: Castle (2009) - 6x23 - For Better or Worse.mp4
Accepted: Catfish- The TV Show - 3x04 - Lucille & Kidd Cole.mp4
Accepted: Catfish- The TV Show - 3x05 - Tracie & Sammie.mp4
Accepted: Catfish- The TV Show - 3x06 - John & Kelsey.mp4
Accepted: Catfish- The TV Show - 3x07 - Solana & Elijah.mp4
Accepted: Catfish- The TV Show - 3x08 - Miranda & Cameryn.mp4
Accepted: Catfish- The TV Show - 3x09 - Jeff & Megan.mp4
Accepted: Constantine - 1x01 - Pilot.mp4
Accepted: Continuum - 3x08 - So Do Our Minutes Hasten.mp4
Accepted: Continuum - 3x09 - Minute Of Silence.mp4
Accepted: Continuum - 3x10 - Revolutions Per Minute.mp4
Accepted: Continuum - 3x11 - 3 Minutes to Midnight.mp4
Accepted: Continuum - 3x12 - The Dying Minutes.mp4
Accepted: Continuum - 3x13 - Last Minute.mp4
Accepted: Criminal Minds - 9x24 - Demons (2).mp4
Accepted: Crossbones - 1x01 - The Devil's Dominion.mp4
Accepted: Crossbones - 1x02 - The Covenant.mp4
Accepted: Crossbones - 1x03 - The Man Who Killed Blackbeard.mp4
Accepted: Crossbones - 1x04 - Antoinette.mp4
Accepted: Defiance - 2x01 - The Opposite of Hallelujah.mp4
Accepted: Defiance - 2x02 - In My Secret Life.mp4
Accepted: Dominion - 1x01 - Pilot.mp4
Accepted: Dominion - 1x02 - Godspeed.mp4
Denied: Elementary - 2x24 - The Grand Experiment.mp4
Accepted: Falling Skies - 4x01 - Ghost in the Machine.m2ts
Accepted: Falling Skies - 4x02 - The Eye.mp4
Accepted: Finding Bigfoot - 5x01 - Bigfoot Call of the Wildman.mp4
Accepted: Ghost Adventures - 9x11 - Whaley House.m2ts
Accepted: Ghost Adventures - 9x12 - Overland Hotel & Saloon.m2ts
Accepted: Grimm - 3x21 - The Inheritance.mp4
Accepted: Grimm - 3x22 - Blond Ambition.mp4
Accepted: Hannibal - 2x11 - Ko No Mono.mp4
Accepted: Hannibal - 2x12 - Tome-wan.mp4
Accepted: Hannibal - 2x13 - Mizumono.mp4
Accepted: Marvel's Agents of S.H.I.E.L.D. - 1x22 - Beginning of the End.mp4
Accepted: NCIS- Los Angeles - 5x24 - Deep Trouble (1).mp4
Accepted: Once Upon a Time (2011) - 3x21-22 - Snow Drifts + There's No Place Lik
e Home.mp4
Accepted: Penny Dreadful - 1x01 - Night Work.mp4
Accepted: Penny Dreadful - 1x02 - SÚance.mp4
Accepted: Penny Dreadful - 1x03 - Resurrection.mp4
Accepted: Penny Dreadful - 1x04 - Demimonde.mp4
Accepted: Penny Dreadful - 1x05 - Closer Than Sisters.mp4
Accepted: Penny Dreadful - 1x06 - What Death Can Join Together.mp4
Accepted: Penny Dreadful - 1x07 - Possession.mp4
Accepted: Penny Dreadful - 1x08 - Grand Guignol.mp4
Accepted: Perception - 3x01 - Paris.mp4
Accepted: Perception - 3x02 - Painless.mp4
Accepted: Perception - 3x03 - Shiver.mp4
Accepted: Power - 1x01 - Not Exactly How We Planned.mp4
Accepted: Power - 1x02 - Whoever He Is.mp4
Accepted: Power - 1x03 - This Is Real.mp4
Denied: Revenge - 3x22 - Execution.mp4
Accepted: River Monsters - 6x06 - Body Snatcher.mp4
Accepted: Salem - 1x01 - The Vow.mp4
Accepted: Salem - 1x02 - The Stone Child.mp4
Accepted: Salem - 1x03 - In Vain.mp4
Accepted: Salem - 1x04 - Survivors.mp4
Accepted: Salem - 1x05 - Lies.mp4
Accepted: Salem - 1x06 - The Red Rose And The Briar.mp4
Accepted: Salem - 1x07 - Our Own Private America.mp4
Accepted: Salem - 1x08 - Departures.mp4
Accepted: Salem - 1x09 - Children Be Afraid.mp4
Accepted: Salem - 1x10 - The House of Pain.mp4
Accepted: Salem - 1x11 - Cat and Mouse.mp4
Accepted: Supernatural - 9x22 - Stairway To Heaven.mp4
Accepted: Supernatural - 9x23 - Do You Believe in Miracles.mp4
Accepted: Teen Wolf - 4x01 - The Dark Moon.mp4
Accepted: Teen Wolf - 4x02 - 117.mp4
Accepted: The Flash (2014) - 1x01 - Pilot.mp4
Accepted: The Last Ship - 1x01 - Phase Six.mp4
Accepted: The Last Ship - 1x02 - Welcome to Gitmo.mp4
Accepted: The Leftovers - 1x01 - Pilot.mp4
Accepted: The Mentalist - 6x21 - Black Hearts.mp4
Accepted: The Mentalist - 6x22 - Blue Bird.mp4
Accepted: The Originals - 1x22 - From a Cradle to a Grave.mp4
Accepted: True Blood - 7x01 - Jesus Gonna Be Here.mp4
Accepted: True Blood - 7x02 - I Found You.mp4
Accepted: Under the Dome - 2x01 - Heads Will Roll.mp4
Accepted: Unforgettable - 3x01 - New Hundred.mp4
Denied: Warehouse 13 - 5x05 - Cangku Shisi.mkv
Denied: Warehouse 13 - 5x06 - Endless.mkv

Total files accepted: 93

Connected to mybooklive:21 as admin
Uploading: Continuum - 3x08 - So Do Our Minutes Hasten.mp4... SUCCESS!
Uploading: Continuum - 3x09 - Minute Of Silence.mp4... SUCCESS!
Uploading: Continuum - 3x10 - Revolutions Per Minute.mp4... SUCCESS!

Skipped 90 files
Uploaded 3 files!
Press any key to continue . . .




^^
As you can see, 90 of the files were skipped because they were located on the FTP server by NAS_Sync, the file sizes were then compared, and determined that they were the same size thus allowing NAS_Sync to decide to skip them.
The Continuum files however were not found / the sizes did not match thus allowing NAS_Sync to easily decide to upload/reupload them to the FTP server.