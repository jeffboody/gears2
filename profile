# To update project for new SDK
# android update project -p project -t android-7

# App
export APP=GearsES2eclair

# Update SDK/NDK to point to the Android SDK and NDK
SDK=/home/jeffboody/android/android-sdk
NDK=/home/jeffboody/android/android-ndk

# Path for jdk on Ubuntu 12.04 (required for manual install)
export PATH=/home/jeffboody/jdk1.6.0_31/bin:$PATH

#-- DON'T CHANGE BELOW LINE --

export PATH=$SDK/tools:$SDK/platform-tools:$PATH
export ANDROID_NDK=$NDK
echo "sdk.dir=$SDK" > project/local.properties

export TOP=`pwd`
alias croot='cd $TOP'
