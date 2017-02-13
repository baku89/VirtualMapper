# VirtualMapper v2.0

![](https://raw.github.com/baku89/VirtualMapper/master/doc/thumbnail.png)

[Download](https://github.com/baku89/VirtualMapper/releases)

## About

Utility tool for projection mapping. You can preview 3d projected image in realtime while editing net of screen. This app can receive image from applications supports [Syphon](http://syphon.v002.info/) or [Spout](http://spout.zeal.co/) (ex: After Effects, VDMX, Arena). It made with [openFrameworks](http://openframeworks.cc/).

Develped by [麦(Baku)](http://baku89.com)

## Instructions

To use:

### 1. Export Alembic data which has uv tag from a 3DCG software

### 2. Setup an sender application

#### Mac (via Syphon)

There's several plugins for softwares below:

* After Effects: [AESyphon plugin](http://tobiasebsen.dk/experiments/aesyphon/)
* VDMX: [Syphon Output - VDMX](http://vdmx.vidvox.net/tutorials/syphon-output)

Other supporting softwares is here: [Syphon official site](http://syphon.v002.info/).

#### Windows (via Spout)

You can download plugins from [Spout official site](http://spout.zeal.co/).

* After Effects: [benkuper/Spout-After-Effects](https://github.com/benkuper/Spout-AfterEffects)

### 3. Launch VirtualMapper

* Launch sender application and VirtualMapper.
* Load the Alembic file from `Open Scene`
* Select the appropreate input source.

Notes: if it doesn't work, try restarting the application.

<!--I uploaded [tutorial (when using After Effects as sender on mac)](https://vimeo.com/117640423).-->

## License

VirtualMapper is published under a MIT License. See the included LISENCE file.

## TODO

* supports image and movie as source
* supports retina display
* adds rendering options (lighting, projector's intensity)
