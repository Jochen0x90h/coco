# Distribute files to other coco libraries next to coco
#
# Usage (see support/conan/README.md):
# 1. Copy cpresets.txt from support/conan/<OS>/ to root of coco (next to this file)
# 2. Adjust cpresets.txt according to own needs
# 3. $ python distribute.py
#

import os, shutil

dir = os.fsencode('..')
for file in os.listdir(dir):
    name = os.fsdecode(file)
    if name.startswith("coco-"):
        dst = f"../{name}/cpresets.txt"
        print(dst)
        shutil.copyfile('cpresets.txt', dst)

        # also copy helper scripts
        if False: #name != 'coco-toolchain':
            dst = f"../{name}/cinstall.py"
            print(dst)
            shutil.copyfile('cinstall.py', dst)

            dst = f"../{name}/ccreate.py"
            print(dst)
            shutil.copyfile('ccreate.py', dst)

            dst = f"../{name}/install.py"
            print(dst)
            shutil.copyfile('install.py', dst)
