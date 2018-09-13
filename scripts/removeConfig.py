import os
import shutil

def main():
    configPath = os.getenv("APPDATA")
    configPath += "\\displayLock"
    print (configPath)
    shutil.rmtree(configPath)


main()