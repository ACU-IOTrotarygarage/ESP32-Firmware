# Original idea by Pascal Gollor at 2022-12-13

Import("env")

import os
import Firmwarepiolib

# You need to specify 'upload_port' in platform_override.ini at '[env]' section

# clear upload flags
env.Replace(UPLOADERFLAGS="")

# Use espupload.py which supports both unzipped and zipped binaries
env.Replace(UPLOADER=os.path.join("pio-tools", "espupload.py"))

# unzipped binary location: build_output\firmware\Firmware-theo.bin
bin_file = Firmwarepiolib.get_final_bin_path(env)
# zipped binary location: build_output\firmware\Firmware-theo.bin.gz
bin_gz_file = bin_file.with_suffix(".bin.gz")

if os.path.exists(bin_gz_file):
  # Zipped binary file - build_output\firmware\Firmware-theo.bin.gz
  env.Replace(UPLOADCMD="$PYTHONEXE $UPLOADER -u $UPLOAD_PORT -f {}".format(bin_gz_file))
elif os.path.exists(bin_file):
  # Unzipped binary file - build_output\firmware\Firmware-theo.bin
  env.Replace(UPLOADCMD="$PYTHONEXE $UPLOADER -u $UPLOAD_PORT -f {}".format(bin_file))
else:
  # Unzipped binary file - C:\tmp\.pioenvs\Firmware-theo\firmware.bin
  env.Replace(UPLOADCMD="$PYTHONEXE $UPLOADER -u $UPLOAD_PORT -f $SOURCES")
