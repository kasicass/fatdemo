
OSNAME=`uname | tr '[:upper:]' '[:lower:]'`
BUILD_CONF=`echo $1 | tr '[:upper:]' '[:lower:]'`

if [ "$BUILD_CONF" == "debug" ] || [ "$BUILD_CONF" == "release" ] ; then
	BUILD_INI="${OSNAME}_${BUILD_CONF}.ini"
else
	echo "Usage:"
	echo "  build.sh <debug|release>"
	exit
fi

emake --ini=$BUILD_INI FatFramework.mak
emake --ini=$BUILD_INI 01-UnitTest.mak
