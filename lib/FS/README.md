# Backport of FS.h

FS.h is a source file that is part of the absolute-latest version of the Teensyduino framework. It's needed to get the
Teensy 4.1's SD card reader to work with the latest SDFat library, so it can use long filenames and fast data transfer.
Unfortunately, it hasn't made it into an official Teensyduino release yet (as of Dec. 2020) so I pulled it out
and included it separately. It's originally from here:

https://github.com/PaulStoffregen/cores/blob/master/teensy4/FS.h

The following post explains what I'm doing:

https://forum.pjrc.com/threads/64136-File-abstraction-and-SdFat-integration?p=257665&viewfull=1#post257665

All this won't be necessary in a future version of Teensyduino's core library, but until then this will have to do.