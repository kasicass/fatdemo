REM for debug
REM set EMAKEPRINT=4
set EMAKEPRINT=

python emake.py --ini=android-armv7-debug.ini FatFramework.mak
python emake.py --ini=android-armv7-debug.ini 00-UnitTest.mak
