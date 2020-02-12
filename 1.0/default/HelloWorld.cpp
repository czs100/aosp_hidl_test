#define LOG_TAG "HelloWorldImpl"

#include <log/log.h>
#include "HelloWorld.h"

namespace android {
namespace hardware {
namespace helloworld {
namespace V1_0 {
namespace implementation {

// Methods from ::android::hardware::helloworld::V1_0::IHelloWorld follow.
Return<void> HelloWorld::justTest(const hidl_string& name, justTest_cb _hidl_cb) {
    ALOGE("justTest, name = %s\n", name.c_str());
    _hidl_cb(name, HelloTest::V_TEST2);
    ALOGD("justTest end\n");
    return Void();
}

Return<void> HelloWorld::justTest1(::android::hardware::helloworld::V1_0::HelloTest name) {
    ALOGD("justTest1, name = %hhu\n", name);
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IHelloWorld* HIDL_FETCH_IHelloWorld(const char* /* name */) {
    //return new HelloWorld();
//}
//
}  // namespace implementation
}  // namespace V1_0
}  // namespace helloworld
}  // namespace hardware
}  // namespace android
