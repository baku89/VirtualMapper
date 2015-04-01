# VirtualMapper v0.9

![](https://raw.github.com/baku89/VirtualMapper/master/doc/thumbnail.png)

[Download](https://github.com/baku89/VirtualMapper/releases)

* [02/28/15] Now supports Windows 8.1! You can download it from [here](https://github.com/baku89/VirtualMapper/releases).

## About

Utility tool for making projection mapping. You can preview 3d projected image in realtime while editing net of screen. This app can receive image from applications supports [Syphon](http://syphon.v002.info/) or [Spout](http://spout.zeal.co/) (ex: After Effects, VDMX, Arena). It made with [openFrameworks](http://openframeworks.cc/).

Develped by [麦(Baku)](http://baku89.com)

## Instructions

To use:

### 1.Export the screen model

This app loads first polygon as screen, others as guide objects(stage, interior and so on) automatically.
So you have to add your project some ajustments showing below.

* In your 3DCG application, merge screen models into one object and bring it top of Object Tree.
* Check if the model has UV coords.
* Export as a file being supported by [Assimp](https://github.com/assimp/assimp). (I recommend .obj)

![](https://raw.github.com/baku89/VirtualMapper/master/doc/object_tree.png)

### 2.Setup an sender application

#### Mac (via Syphon)

Setup Example:

* After Effects: [AESyphon plugin](http://tobiasebsen.dk/experiments/aesyphon/)
* VDMX: [Syphon Output - VDMX](http://vdmx.vidvox.net/tutorials/syphon-output)

Plugins for some other applicaitons are available on [Syphon official site](http://syphon.v002.info/).

#### Windows (via Spout)

You can download plugins from [Spout official site](http://spout.zeal.co/).

Setup Example:

* After Effects: [benkuper/Spout-After-Effects](https://github.com/benkuper/Spout-AfterEffects)

### 3.Launch VirtualMapper

* Launch sender application and VirtualMapper.
* In control panel, choose an application from drop down menu under the [Source].
* Click [select 3d file..] and select 3d model on file dialogue.

Notes: if it do not work properly, try restarting the application.

I uploaded [tutorial (when using After Effects as sender on mac)](https://vimeo.com/117640423).

## Licensing

VirtualMapper is published under a MIT License. See the included LISENCE file.

## Notes

* This plugin has been complied on Mac OSX 10.10 and Windows 8.1, and has not been tested with other environment.

## TODO

* support image and movie
* support retina display
* add rendering option (lighting, projector's intensity)
