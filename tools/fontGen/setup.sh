#!/bin/bash

if [ $EUID -ne 0 ]
then
    echo "need run this as root"
    exit 1
fi

if ! [ -f ./setup.sh ]
then
    echo "need run this right in the same dir as setup.sh"
    exit 1
fi

echo "******** checking dependencies ********"

echo "**** checking for freetype ****"
if ! [ -f /usr/local/lib/libfreetype.a ]
then
    echo "*** freetype not found ***"
    echo "** installing freetype dependencies **"
    printf "y\ny\n" | apt-get install libpng-dev libharfbuzz-dev
    echo "** freetype dependencies installed **"
   echo "** downloading freetype v2.6.5 **"
   wget https://download.savannah.gnu.org/releases/freetype/freetype-2.6.5.tar.gz
   echo "** freetype v2.6.5 downloaded **"

   echo "** installing freetype v2.6.5 **"
   tar -xzvf freetype-2.6.5.tar.gz
   cd freetype-2.6.5
   mkdir build
   cd build
   rm -rfv ./* && cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake --build . --target install
   rm -rfv ./* && cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . --target install
   echo "** freetype v2.6.5 installed **"
   cd ../../
   if [ -f /usr/local/lib/libfreetype.a ]
   then
       rm -rfv freetype-2.6.5*
       echo "**** freetype found ****"
   else
       echo "******** freetype installed failed ********"
       exit 1
   fi
else
       echo "**** freetype found ****"
fi    

echo "******** dependencies checked ********"
exit 0
