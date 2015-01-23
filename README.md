# VirtualMapper v0.8

<img src="https://raw.github.com/baku89/VirtualMapper/master/thumbnail.png" />

## About

Utility tool for making projection mapping. You can preview 3d projected image in realtime from a software supports [Syphon](http://syphon.v002.info/)(ex. After Effects, Max 6, VDMX, Resolume, Module8). This app made with [openFrameworks](http://openframeworks.cc/).

麦(Baku) [site](http://baku89.com)

## Instructions

You have to prepare a 3D screen model and a net image of screen.

To use:

### Export the screen model

* Merge screen models into one object in your 3DCG application.
* Check if the model has UV coords.
* Export as a file being supported by [Assimp](https://github.com/assimp/assimp). (I recommend .obj)

### Setup an application publishing net image via Syphon

For Example:

* After Effects: using [AESyphon plugin](http://tobiasebsen.dk/experiments/aesyphon/).
* VDMX: refer to this page [Syphon Output - VDMX](http://vdmx.vidvox.net/tutorials/syphon-output)

You have to keep this application opened while using VirtualMapper.

### Open VirtualMapper

* In control panel, choose an application from drop down menu under the [Source].
* Click [select 3d file..] and select 3d model on file dialogue.

Notes: if it do not work properly, try restarting the application.

### Additional Usage

I am going to publish tutorial on [my vimeo](http://vimeo.com/baku89) in a few days.

## Licensing

VirtualMapper is published under a MIT License. See the included LISENCE file.

## Requirements

Mac OS X 10.10 or greater Open Frameworks 008 or better

## TODO

* support image and movie
* support retina display
* support windows
* add rendering option (lighting, projector's intensity)