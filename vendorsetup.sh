#
# Copyright (C) 2011 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# This file is executed by build/envsetup.sh, and can use anything
# defined in envsetup.sh.
#
# In particular, you can add lunch options with the add_lunch_combo
# function: add_lunch_combo generic-eng

# frameworks/av
repopick -f 231170
repopick -f 231171
repopick -f 231172

# frameworks/base
repopick -f 231169

# frameworks/native
repopick -f 231162
repopick -f 70578

# hardware/qcom/display-caf
repopick -f 75260
repopick -f 231168
repopick -f 231166
repopick -f 231167

# vendor/cm
repopick -f 231164
repopick -f 231165

add_lunch_combo lineage_pico-userdebug
