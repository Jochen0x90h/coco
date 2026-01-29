import os

# patch CCMRAM function into startup code
for dirname in ["../stm32f3/coco/platform/system/", "../stm32g4/coco/platform/system/"]:
    directory = os.fsencode(dirname)
    for file in os.listdir(directory):
        filename = os.fsdecode(file)
        if filename.endswith(".s"):
            # read file
            with open(dirname + filename) as f:
                lines = [line.rstrip() for line in f]

            # write file
            with open(dirname + filename, 'w') as f:
                skip = False
                after = False
                print(filename)
                for line in lines:
                    # skip previous copy ccmram function
                    if line == "/* Copy ccmram segment from flash to CCMRAM */":
                        skip = True

                    # print existing line
                    if not skip and (not after or line != ""):
                        print(line, file=f)
                        after = False

                    # stop skipping at end of copy ccram function
                    if line == "  bcc CopyCcmInit":
                        skip = False

                    # add new copy ccmram function after copy data function
                    if " ".join(line.split()) == "bcc CopyDataInit":
                        print("", file=f)
                        print("/* Copy ccmram segment from flash to CCMRAM */", file=f)
                        print("  ldr r0, =_sccmram", file=f)
                        print("  ldr r1, =_eccmram", file=f)
                        print("  ldr r2, =_siccmram", file=f)
                        print("  movs r3, #0", file=f)
                        print("  b LoopCopyCcmInit", file=f)
                        print("", file=f)
                        print("CopyCcmInit:", file=f)
                        print("  ldr r4, [r2, r3]", file=f)
                        print("  str r4, [r0, r3]", file=f)
                        print("  adds r3, r3, #4", file=f)
                        print("", file=f)
                        print("LoopCopyCcmInit:", file=f)
                        print("  adds r4, r0, r3", file=f)
                        print("  cmp r4, r1", file=f)
                        print("  bcc CopyCcmInit", file=f)
                        print("", file=f)
                        after = True
                print("patched!")
