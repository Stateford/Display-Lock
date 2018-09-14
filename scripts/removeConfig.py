import os
import shutil

def main():
    configPath = os.getenv("APPDATA")
    configPath += "\\displayLock"
    print (configPath)
    try:
        shutil.rmtree(configPath)
        print("directory cleaned")
    except:
        print("directory doesn't exist")


main()