"""
buildInc.py
-----------
automatically increments build number and configuration
"""

import sys
import os

def writeHeader(filePath, config, version, build):
    print("Writing to path: %s\n" % filePath)
    versSplit = version.split('.')
    
    file = open(filePath, 'w+')
    file.write("#pragma once\n\n")
    file.write("#define CONFIGURATION \"%s\"\n" % config)
    file.write("#define VERSION_MAJOR %d\n" % int(versSplit[0]))
    file.write("#define VERSION_MINOR %d\n" % int(versSplit[1]))
    file.write("#define VERSION_REVISION %d\n" % int(versSplit[2]))
    file.write("#define VERSION_BUILD %s\n" % build)
    file.write("#define FILE_VERSION \"%s.%s\"\n" % (version, build))
    file.close()

def main():
    print(sys.argv)
    if(len(sys.argv) < 3):
        raise BaseException("Something went wrong with build")
    
    writeHeader(os.path.abspath("./Display-Lock/resources/version.h"), sys.argv[1], sys.argv[2], sys.argv[3])


main()