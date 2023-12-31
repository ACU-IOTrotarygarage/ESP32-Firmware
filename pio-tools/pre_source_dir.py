Import("env")

import glob
import os

def FindInoNodes(env):
    src_dir = glob.escape(env.subst("$PROJECT_SRC_DIR"))
    return env.Glob(os.path.join(src_dir, "*.ino")) + env.Glob(
        os.path.join(src_dir, "Firmware_*", "*.ino")
    )

env.AddMethod(FindInoNodes)
