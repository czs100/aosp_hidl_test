#! /bin/bash

PACKAGE=android.hardware.helloworld@1.0
LOC=hardware/interfaces/helloworld/1.0/default/

hidl-gen -o $LOC -Lc++-impl -randroid.hardware:hardware/interfaces \
        -randroid.hidl:system/libhidl/transport $PACKAGE
hidl-gen -o $LOC -Landroidbp-impl -randroid.hardware:hardware/interfaces \
        -randroid.hidl:system/libhidl/transport $PACKAGE