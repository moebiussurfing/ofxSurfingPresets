ofxSurfingPresets
=============================
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Stage: beta](https://img.shields.io/badge/-alpha-red)

# Overview
**ofxSurfingPresets** is an **openFrameworks** addon template for MYSELF.

## Screenshot
![image](docs/readme_images/Capture1.PNG?raw=true "image")

## Features
- Customize GUI.
- Basic parameters and app modes.
- Store/Recall settings.
- 

The first step is to create texture just like every texture using threejs.

    const texturePath =  'https://i.imgur.com/Oj6RJV9.png';
    const spriteTexture = new  THREE.TextureLoader().load(texturePath)

   
Next step is about creating the animator object:

     const animator =  new  PlainAnimator(spriteTexture, 4, 4, 10, 15);
These magic numbers are the follows:
|value| description |
|--|--|
| 4 | number of frames horizontally |
| 4 | number of frames vertically |
| 10 | total number of frames |
| 15 | frames per second (fps) |

## Usage
 
** ofApp.h **
```.cpp
#include "ofxSurfingPresets.h"
ofxSurfingPresets myAddon;
```

** ofApp.cpp **
```.cpp
ofApp::setup(){
	ofxSurfingPresets.setup();
}

ofApp::update(){
	ofxSurfingPresets.update();
}

ofApp::draw(){
	ofxSurfingPresets.draw();
	ofxSurfingPresets.drawGui();
}
```

details>
  <summary>Dependencies</summary>
  <p>

Clone these add-ons and include into the **OF Project Generator** to allow compile your projects or the examples:
* [ofxColorClient](https://github.com/moebiussurfing/ofxColorClient)
* [ofxSurfingHelpers](https://github.com/moebiussurfing/ofxSurfingHelpers)  
* [ofxScaleDragRect](https://github.com/moebiussurfing/ofxScaleDragRect)
* ofxGui  [ **OF** ]
* ofxXmlSettings [ **OF** ]
* [ofxWindowApp](https://github.com/moebiussurfing/ofxWindowApp)  [ Only for the example ]

Above add-ons already packed into **OF_ADDON/libs**. No need to add them manually with the **OF Project Generator**:  
* [ofxColorQuantizerHelper](https://github.com/moebiussurfing/ofxColorQuantizerHelper)

*Thanks a lot to all these ofxAddons coders. Look into each folder for authoring credits, original forks, and license info.*  
  </p>
</details>

<details>
  <summary>Tested Systems</summary>
  <p>

  - **Windows 10** / **VS 2017** / **OF ~0.11**
  - **macOS**. **High Sierra** / **Xcode9** & **Xcode10** / **OF ~0.11**
  </p>
</details>

<br/>

### TODO
* 

### Notes
*

## Author
An addon by **@moebiusSurfing**  
*( ManuMolina ) 2019-2021*  

[Twitter](https://twitter.com/moebiussurfing/)  
[Instagram](https://www.instagram.com/moebiussurfing/)  
[YouTube](https://www.youtube.com/channel/UCzUw96_wjmNxyIoFXf84hQg)  

## License

[**MIT License**](https://github.com/moebiussurfing/ofxColorManager/blob/b29c56f7b0e374b6a6fe2406e45fbfaaf2726112/LICENSE)