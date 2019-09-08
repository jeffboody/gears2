# App
export APP=GearsES2eclair

# Update SDK to point to the Android SDK
SDK=$HOME/Android/Sdk
SDK2=$HOME/Android/android-sdk-macosx-r24.3.3

#-- DON'T CHANGE BELOW LINE --

export PATH=$SDK/tools:$SDK/platform-tools:$PATH
export PATH=$SDK/build-tools/28.0.3:$PATH
export PATH=$SDK2/build-tools/22.0.1:$PATH
export TOP=`pwd`
alias croot='cd $TOP'
