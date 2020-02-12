#include <android/hardware/helloworld/1.0/IHelloWorld.h>
# include <hidl/Status.h>
# include <hidl/LegacySupport.h>
# include <utils/misc.h>
# include <hidl/HidlSupport.h>
# include <stdio.h>
#include <log/log.h>

using android::hardware::helloworld::V1_0::IHelloWorld;
using android::sp;
using android::hardware::hidl_string;

int main()
{
    //int ret;

    android::sp<IHelloWorld> service = IHelloWorld::getService();
    if(service == nullptr) {
        printf("Failed to get service\n");
        return -1;
    }
    hidl_string packageName("android.hardware.helloworld.test");
    service->justTest(packageName, [&](std::string result, ::android::hardware::helloworld::V1_0::HelloTest value) {
            ALOGE("[Test] name is %s, the value is %hhu\n", result.c_str(), value);
        });
    service->justTest1(::android::hardware::helloworld::V1_0::HelloTest::V_TEST2);

    return 0;
}