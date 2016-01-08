##Cocos2DX SDK for AppWarp

Visit [AppWarp Cocos2DX home page](http://appwarp.shephertz.com/game-development-center/cocos2dx-game-developers-home/) to learn about AppWarp features and documentation.

**IMPORTANT**
The instructions below assume you have already setup Cocos2DX environment for iOS and Android development. To help set up your development for Cocos2DX, you can read this [Cocos2DX getting started guide](http://www.raywenderlich.com/33750/cocos2d-x-tutorial-for-ios-and-android-getting-started).

The steps below describe the integration points of AppWarpX in your Cococs2DX application. 

##Instructions for iOS

* Download the contents of this repo

* Extract the contents and add (drag n drop) the AppWarpX folder to your xcode project.

* Your XCode structure should look something like this

![AppWarp Cocos2dx iOS](https://raw.githubusercontent.com/shephertz/AppWarpDeveloper/master/Cocos2dX/xcode_cocos2dx.png)


* Now add path for curl as follows:

  
    -Open Xcode project 
    
    -Click on project name in the left pane 
    
    -Select your project under PROJECT heading and search for Header Search Path 
    
    -Double click on the value of Header Search Path
    
    -Click on "+" button on the bottom left cornor of the popup appeared 
    
    -Add this: "$(SRCROOT)/../cocos2d/external/curl/include/ios"

__The Header Search path should look like this__


![AppWarp Cocos2dx iOS](https://raw.githubusercontent.com/shephertz/AppWarpDeveloper/master/Cocos2dX/SetCurlPath.png)


* Build

##Instructions for Android

* Download the contents of this repo

* Extract and add the AppWarpX folder next to your Classes folder. The directory structure should look like this

![AppWarp Cocos2dx iOS](https://raw.githubusercontent.com/shephertz/AppWarpDeveloper/master/Cocos2dX/68747470733a2f2f646c2e64726f70626f7875736572636f6e74656e742e636f6d2f752f36313038343335302f616e64726f69645f636f636f733264782e706e67.png)

* Edit proj.android\jni\Android.mk file

* Add $(call import-module,curl/prebuilt/android) to Android.mk file if you are using cocos V3.X or above for enabling cURL support.

Add AppWarp source c and cpp files so that they can also be built. After adding the AppWarp source files, your android.mk file should be as follows:

```
LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../AppWarpX/appwarp.cpp \
                   ../../AppWarpX/appwarp_extended.cpp \
                   ../../AppWarpX/base64.cpp \
                   ../../AppWarpX/cJSON.c \
                   ../../AppWarpX/HMAC_SHA1.cpp \
                   ../../AppWarpX/requests.cpp \
                   ../../AppWarpX/SHA1.cpp \
                   ../../AppWarpX/socket.cpp \
                   ../../AppWarpX/udpsocket.cpp \
                   ../../AppWarpX/urlencode.cpp \
                   ../../AppWarpX/utilities.cpp
```
Add AppWarp source file folder so that it can be detected by Compiler. For Example :

```
	LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
			    	$(LOCAL_PATH)/../../AppWarpX
```

* Build the native code

* Add the following to your manifest file to give internet permission if not given already

```
<uses-permission android:name="android.permission.INTERNET" />
```

* Run your Android application
