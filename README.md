# SoWw
Coin GUI binding for wxWidgets

## cpack 

cpack can generate configuration for predefined platform.
cpack needs to be enabled in cmake setting SOWW_USE_CPACK to ON

Packages for platform can be generated using specific platforms 
(e.g. cpack --config cmake-build-debug/cpack.d/centos.cmake).


## Windows address sanitizier
Add 
	"addressSanitizerEnabled": true,
in CMakeSettings.json
