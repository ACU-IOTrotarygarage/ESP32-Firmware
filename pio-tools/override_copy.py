Import('env')

import os
import shutil

# copy Firmware/user_config_override_sample.h to Firmware/user_config_override.h
if os.path.isfile("Firmware/user_config_override.h"):
    print ("*** use provided user_config_override.h as planned ***")
else:
    shutil.copy("Firmware/user_config_override_sample.h", "Firmware/user_config_override.h")

# copy platformio_override_sample.ini to platformio_override.ini
if os.path.isfile("platformio_override.ini"):
    print ("*** use provided platformio_override.ini as planned ***")
else:
    shutil.copy("platformio_override_sample.ini", "platformio_override.ini")

# copy platformio_Firmware_cenv_sample.ini to platformio_Firmware_cenv.ini
if os.path.isfile("platformio_Firmware_cenv.ini"):
    print ("*** use provided platformio_Firmware_cenv.ini as planned ***")
else:
    shutil.copy("platformio_Firmware_cenv_sample.ini", "platformio_Firmware_cenv.ini")
