"""
buildInc.py
-----------
automatically increments build number and configuration
"""

import sys
import os

def writeHeader(filePath, config, version, build):
    file = open(filePath, 'w+')
    file.write("#pragma once\n\n")
    file.write("#define CONFIGURATION \"%s\"\n" % config)
    file.write("#define VERSION_BUILD %s\n" % build)
    if(config.lower() == "release"):
        file.write("#define FILE_VERSION \"%s.%s\"\n" % (version, build))
    else:
        file.write("#define FILE_VERSION \"%s.%s-RC\"\n" % (version, build))

    file.close()
    pass

def main():
    print(sys.argv)
    if(len(sys.argv) < 3):
        raise BaseException("Something went wrong with build")
    
    writeHeader(os.path.abspath("../Display-Lock/resources/version.h"), sys.argv[1], sys.argv[2], sys.argv[3])


main()