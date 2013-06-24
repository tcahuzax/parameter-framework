LOCAL_PATH:= $(call my-dir)

####################
# Common definitions

common_src_files := \
    main.cpp \
    TestPlatform.cpp

common_module := test-platform
common_module_tags := optional

common_c_includes := \
    $(LOCAL_PATH)/../../parameter/include \
    $(LOCAL_PATH)/../../remote-processor/

common_shared_libraries := libparameter libremote-processor
#############################
# Target build

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= $(common_src_files)

LOCAL_MODULE:= $(common_module)
LOCAL_MODULE_TAGS := $(common_module_tags)

LOCAL_CFLAGS += $(common_cflags)

LOCAL_C_INCLUDES += \
    $(common_c_includes) \
    external/stlport/stlport/ \
    bionic/libstdc++ \
    bionic/

LOCAL_SHARED_LIBRARIES := $(common_shared_libraries) libstlport

include $(BUILD_EXECUTABLE)

##############################
# Host build

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= $(common_src_files)

LOCAL_MODULE:= $(common_module)_host
LOCAL_MODULE_TAGS := $(common_module_tags)

LOCAL_CFLAGS += $(common_cflags)

LOCAL_C_INCLUDES += $(common_c_includes)

LOCAL_SHARED_LIBRARIES := $(foreach shared_library, $(common_shared_libraries), \
    $(shared_library)_host)

include $(BUILD_HOST_EXECUTABLE)
