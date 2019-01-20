# econfig

Error checking API for libconfig

## Overview

econfig provides error checking functionaliy for libconfig function
calls. Majority of the API remains intact, so most of the functions
have the same naming convention of libconfig including a prepended
'e'. Some functionality has been added such as *econfig_get_value()*
and the error and assertion function; the only API change is with
*econfig_setting_get_value()* which includes a "path" paramter, thus
allowing data retrieval for paths relative to a provided setting.
 
These functions become useful for obtaining configuration information
for required settings. If a setting is optional or has other optional
features, then the standard libconfig functions should be called, with
error checking performed manually for the specific use cases (e.g., a
setting path not found may be ok for an optional setting.)

## Usage

econfig is distributed as a header file. Simply drop it into your
source tree to start including it into your code.
