
https://github.com/fabiangreffrath/crispy-doom

### Build
```
$ git clone https://github.com/fabiangreffrath/crispy-doom.git
$ sudo apt install build-essential automake git
$ # sudo apt build-dep crispy-doom # Not needed and also fails
$ # One more package that I do not remember.
$ cd crispy-doom
$ autoreconf -fiv
$ ./configure
$ make
```

### WAD
```
$ wget http://cdn.debian.net/debian/pool/non-free/d/doom-wad-shareware/doom-wad-shareware_1.9.fixed.orig.tar.gz
$ gunzip doom-wad-shareware_1.9.fixed.orig.tar.gz
$ tar -xf doom-wad-shareware_1.9.fixed.orig.tar
```

### Run
```
$ ./src/crispy-doom -iwad doom-wad-shareware-1.9.fixed/doom1.wad
```
