# Distribute files to other coco libraries next to coco
#
# Usage:
# 1. Copy presets.txt from support/conan/<OS>/ to root of coco (next to this file)
# 2. Adjust presets.txt according to own needs
# 3. python distribute.py
#

import os, shutil

dir = os.fsencode('..')
for file in os.listdir(dir):
    name = os.fsdecode(file)
    if name.startswith("coco-"):
        dst = f"../{name}/presets.txt"
        print(dst)
        shutil.copyfile('presets.txt', dst)

        if name != 'coco-toolchain':
            dst = f"../{name}/configure.py"
            print(dst)
            shutil.copyfile('configure.py', dst)

            dst = f"../{name}/create.py"
            print(dst)
            shutil.copyfile('create.py', dst)

            dst = f"../{name}/install.py"
            print(dst)
            shutil.copyfile('install.py', dst)
