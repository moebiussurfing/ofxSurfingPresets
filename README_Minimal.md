# ofxSurfingPresets

## Overview

Simple presets systems for **openFrameworks** with an **ImGui** based GUI.

## Screenshot
![image](docs/Capture1.PNG?raw=true "image")

## Features
- De/Serialize JSON files to an ofParameterGroup.

## Usage
```c++
// ofApp.h
#include "ofxSurfingPresets.h"
ofxSurfingPresets dataPresets;

// ofApp.cpp
void ofApp::setup() {
    // param group
    params.setName("paramsGroup");
    params.add(fill.set("fill", true));
    params.add(lineWidth.set("lineWidth", 1, 0.1, 10));
    params.add(size.set("size", 100, 5, 200));

    dataPresets.addGroup(params);
}
```

## Dependencies
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)

## Tested Systems
* **Windows 10** / **VS 2017** / **OF ~0.11**

## Author
An addon by **@moebiusSurfing**  
*( ManuMolina ) 2021*  

<br/>

[Twitter](https://twitter.com/moebiussurfing/)  
[Instagram](https://www.instagram.com/moebiussurfing/)  
[YouTube](https://www.youtube.com/channel/UCzUw96_wjmNxyIoFXf84hQg)  

## License
[**MIT License**](https://github.com/LICENSE)