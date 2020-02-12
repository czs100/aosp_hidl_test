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
    android::sp<IHelloWorld> service = new HelloWorld();
    configureRpcThreadpool(1, true /*callerWillJoin*/);
    if(android::OK !=  service->registerAsService())
        return 1;
    joinRpcThreadpool();
#endif
}