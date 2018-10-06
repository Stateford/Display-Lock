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
    
    versionFile = open(filePath, 'w+')
    versionFile.write("#pragma once\n\n")
    versionFile.write("#define CONFIGURATION \"%s\"\n" % config)
    versionFile.write("#define VERSION_MAJOR %d\n" % int(versSplit[0]))
    versionFile.write("#define VERSION_MINOR %d\n" % int(versSplit[1]))
    versionFile.write("#define VERSION_REVISION %d\n" % int(versSplit[2]))
    versionFile.write("#define VERSION_BUILD %s\n" % build)
    versionFile.write("#define FILE_VERSION \"%s.%s\"\n" % (version, build))
    versionFile.close()

def main():
    print(sys.argv)
    if(len(sys.argv) < 3):
        raise BaseException("Something went wrong with build")
    
    writeHeader(os.path.abspath("./Display-Lock/resources/version.h"), sys.argv[1], sys.argv[2], sys.argv[3])

main()
