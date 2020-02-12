软件包前缀 | 位置
---|---
android.hardware.*	| hardware/interfaces/*
android.frameworks.* | frameworks/hardware/interfaces/*
android.system.* | system/hardware/interfaces/*
android.hidl.* | system/libhidl/transport/*

### 一 创建.hal文件，
```
创建hardware/interfaces/helloworld/1.0/IHelloWorld.hal

package android.hardware.helloworld@1.0;

interface IHelloWorld {
    justTest(string name) generates (string result, HelloTest value);

    justTest1(HelloTest name);
};
```
其中HelloTest类型是自己定义的
```
创建hardware/interfaces/helloworld/1.0/types.hal
package android.hardware.helloworld@1.0;

enum HelloTest : uint8_t {
    V_TEST1 = 0,
    V_TEST2 = 1,
};
```
### 二 生成HAL相关的文件
#### 2.1 编译hidl-gen工具
```
make hidl-gen 
```
#### 2.2 创建update-files.sh脚本
```
#! /bin/bash
PACKAGE=android.hardware.helloworld@1.0
LOC=hardware/interfaces/helloworld/1.0/default/
 
hidl-gen -o $LOC -Lc++-impl -randroid.hardware:hardware/interfaces \
        -randroid.hidl:system/libhidl/transport $PACKAGE
hidl-gen -o $LOC -Landroidbp-impl -randroid.hardware:hardware/interfaces \
        -randroid.hidl:system/libhidl/transport $PACKAGE
```
#### 2.3 
在1.0/default目录底下可以生成以下三个文件，我是在根目录下执行的脚本
```
. ./update-files.sh
-rw-rw-r-- 1 shift shift  973 1月  17 20:05 Android.bp
// 实现接口的关键文件
-rw-rw-r-- 1 shift shift  605 1月  17 20:05 HelloWorld.cpp
-rw-rw-r-- 1 shift shift 1159 1月  17 20:05 HelloWorld.h
```
如果是实用passthrough 模式，则需要打开HIDL_FETCH_IHelloWorld() 函数的注释，并且在-impl 的C++文件中实现，例如这里如果使用passthrough 模式，需要在HelloWorld.cpp 中实现该函数，详细可以看nfc 或tv 等模块中实现。

### 三 生成android.bp文件
```
cd hardware/interfaces
. update-makefiles.sh
```
会自动扫描hardware/interfaces目录下的所有hal文件，生成android.bp

### 四 新增service文件
#### 在default目录下新增service.cpp
```
#define LOG_TAG "android.hardware.helloworld@1.0-service"
 
#include <android/hardware/helloworld/1.0/IHelloWorld.h>
 
#include <hidl/LegacySupport.h>
#include "HelloWorld.h"
 
// Generated HIDL files
using android::hardware::helloworld::V1_0::IHelloWorld;
using android::hardware::helloworld::V1_0::implementation::HelloWorld;
 
using android::hardware::defaultPassthroughServiceImplementation;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
 
int main() {
#if 0
    return defaultPassthroughServiceImplementation<IHelloWorld>();
#else
    sp<IHelloWorld> service = new HelloWorld();
    configureRpcThreadpool(1, true /*callerWillJoin*/);
    if(android::OK !=  service->registerAsService())
        return 1;
    joinRpcThreadpool();
#endif
} 
```
#### 在default目录下新增
android.hardware.helloworld@1.0-service.rc

#### 修改default目录底下的Android.bp文件
里面已经有impl的编译，新增service的编译
```
cc_binary {
    // FIXME: this should only be -impl for a passthrough hal.
    // In most cases, to convert this to a binderized implementation, you should:
    // - change '-impl' to '-service' here and make it a cc_binary instead of a
    //   cc_library_shared.
    // - add a *.rc file for this module.
    // - delete HIDL_FETCH_I* functions.
    // - call configureRpcThreadpool and registerAsService on the instance.
    // You may also want to append '-impl/-service' with a specific identifier like
    // '-vendor' or '-<hardware identifier>' etc to distinguish it.
    name: "android.hardware.helloworld@1.0-service",
    defaults: ["hidl_defaults"],
    relative_install_path: "hw",
    // FIXME: this should be 'vendor: true' for modules that will eventually be
    // on AOSP.
    vendor: true,
    init_rc: ["android.hardware.helloworld@1.0-service.rc"],

    srcs: [
        "HelloWorld.cpp",
        "service.cpp"
    ],
    shared_libs: [
        "liblog",
        "libhidlbase",
        "libhidltransport",
        "libutils",
        "libhardware",
        "android.hardware.helloworld@1.0",
    ],
}
```
### 四 编译 
```
mm 
```
或者在根目录下
```
make android.hardware.helloworld@1.0
```
或者
```
mmm 路径
```

### 七 修改manifest.xml
为了让服务端被客户端访问到，要修改这个xml，具体原理还不清楚
不是这个device\brobwind\rpi3\manifest.xml
是手机环境中的 vendor/etc/vintf/也可能是在system中
```
<hal format="hidl">
    <name>android.hardware.helloworld</name>
    <transport>hwbinder</transport>
    <version>1.0</version>
    <interface>
        <name>IHelloWorld</name>
        <instance>default</instance>
    </interface>
</hal>
```

### 八 编译结果
```
/vendor/lib64/hw/android.hardware.helloworld@1.0-impl.so // 模块实现端的代码编译生成，binder server端
/vendor/etc/init/android.hardware.helloworld@1.0-service.rc //Android native 进程入口
/vendor/bin/hw/android.hardware.helloworld@1.0-service
/system/lib64/android.hardware.helloworld@1.0.so  //模块调用端的代码，binder client端
```

### 九 调试过程
```
adb root
adb remount

adb push vendor\lib\hw\android.hardware.helloworld@1.0-impl.so /vendor/lib/hw/
adb push vendor\etc\init/android.hardware.helloworld@1.0-service.rc /vendor/etc/init/
adb push vendor\bin/hw/android.hardware.helloworld@1.0-service /vendor/bin/hw/
adb push system\lib/android.hardware.helloworld@1.0.so /system/lib/
adb push system\lib/android.hardware.helloworld@1.0.so /vendor/lib/
adb push vendor\bin/bidl_hello_test system/bin

pause
adb shell chmod 777 -R system/bin/bidl_hello_test
pause

adb push manifest.xml vendor/etc/vintf/
adb reboot

./
```
