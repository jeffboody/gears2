#!/bin/bash
echo `date` > project/res/raw/timestamp.raw
ant -f project/build.xml clean
ant -f project/build.xml debug
