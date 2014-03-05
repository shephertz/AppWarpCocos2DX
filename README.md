##Cocos2DX SDK for AppWarp

Visit [AppWarp Cocos2DX home page](http://appwarp.shephertz.com/game-development-center/cocos2dx-game-developers-home/) to learn about AppWarp features and documentation.

**IMPORTANT**
The instructions below assume you have already Cocos2DX environment already setup for iOS and Android development. To help set up your development for Cocos2DX, you can read this [Cocos2DX getting started guide](http://www.raywenderlich.com/33750/cocos2d-x-tutorial-for-ios-and-android-getting-started).

The steps below describe the integration points of AppWarpX in your Cococs2DX application. 

##Instructions for iOS

* Download the contents of this repo

* Extract the contents and add (drag n drop) the AppWarpX folder to your xcode project.

* Your XCode structure should look something like this

![AppWarp Cocos2dx iOS](https://dl.dropboxusercontent.com/u/61084350/xcode_cocos2dx.png)

* Build

##Instructions for Android

* Download the contents of this repo

* Extract and add the AppWarpX folder next to your Classes folder. The directory structure should look like this

![AppWarp Cocos2dx iOS](https://dl.dropboxusercontent.com/u/61084350/android_cocos2dx.png)

* Edit proj.android\jni\Android.mk file

Add AppWarp source c and cpp files so that they are built. For example something like this

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

Also add the following at the end of your Android.mk file for curl

```
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
```
* Build the native code

* Add the following to your manifest file

```
<uses-permission android:name="android.permission.INTERNET" />
```

* Run your Android application
