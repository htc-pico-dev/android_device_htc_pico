LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE                    := block_symlink
LOCAL_MODULE_TAGS               := optional
LOCAL_MODULE_PATH               := $(TARGET_ROOT_OUT)/sbin
LOCAL_SRC_FILES                 := block_symlink.c
LOCAL_CFLAGS                    += -Wall -Werror
LOCAL_STATIC_LIBRARIES          := libc
LOCAL_FORCE_STATIC_EXECUTABLE   := true
include $(BUILD_EXECUTABLE)
