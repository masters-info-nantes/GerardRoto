# Gerard - Roto
Qt application to make rotoscoping from videos

![Interface du logiciel](http://img15.hostingpics.net/pics/497593Capturedcran20150228194031.png)

## Requirements
* tar
* ffmpeg

## Troubleshots
### Can't create a new project
ffmpeg must be in */usr/bin* but if you installed it with brew or others it may be not.
To check run:

```
$ which ffmpeg
/usr/local/bin/ffmpeg
```

If it's not in the right path, create a symlink:

```
$ ln -s /usr/local/bin/ffmpeg /usr/bin/ffmpeg
```

## Authors
* Anthony Pena
* Jérémy Bardon
