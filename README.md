# ofxSurfingPresets

## Overview

Presets System for **openFrameworks** with an **ImGui** based GUI.

## Screenshot
![gif](docs/ofxSurfingPresets.gif?raw=true "gif")

## Features
- De/Serializes JSON files from/to an **ofParameterGroup**.  
- **ImGui** based Gui.
- Easy to integrate to your projects/add-ons.
- Reset and basic Randomize parameters.
- One Memory slot to Store/Recall compare and copy presets.
- Key Commands.
- Clear and auto populate (randomized or not) kit of Presets.
- Compatible with some Midi add-ons.
- Simple smoother and player.

## Usage

ofApp.h
```c++
#include "ofxSurfingPresets.h"

ofxSurfingPresets presets;
ofParameterGroup params;
```

ofApp.cpp
```c++
void ofApp::setup() 
{
    // Params Group
    params.setName("paramsGroup");
    params.add(fill.set("fill", true));
    params.add(lineWidth.set("lineWidth", 1, 0.1, 10));
    params.add(size.set("size", 100, 5, 200));

    presets.addGroup(params);
}
```

## Dependencies
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxSurfingImGui](https://github.com/moebiussurfing/ofxSurfingImGui)
* [ofxImGui](https://github.com/Daandelange/ofxImGui/)  [ _FORK from_ @**Daandelange** ]  
**OPTIONAL**:  
* [ofxMidiParams](https://github.com/moebiussurfing/ofxMidiParams)  [ _FORK_ ]  
* [ofxSurfingPlayer](https://github.com/moebiussurfing/ofxSurfingPlayer)  

<details>
  <summary>A MORE POWERFUL ALTERNATIVE</summary>
  <p>
      
[ofxPresetsManager](https://github.com/moebiussurfing/ofxPresetsManager)  
      
    - Presets for multiple groups in parallel.
    - Global Combinations of the groups to presets.
    - BPM timed randomizers with probabilities to select presets.
    - Range randomizers to explore parameter settings.
    - Tweening or Smoothing of parameters with plotting.
    - Standalone preset files.
   </p>
</details>

## Tested Systems
* **Windows 10** / **VS 2017** / **OF ~0.11**

## Author
An add-on by **@moebiusSurfing**  
*( ManuMolina ) 2021*  

## License
[**MIT License**](https://github.com/LICENSE)
