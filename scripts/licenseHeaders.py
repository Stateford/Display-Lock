"""

"""
import os

class FileWriter:


    def __init__(self, path):
        self.path = path
        self.file = open(self.path, "r")
        self.contents = self.file.readlines()
        self.file.close()
        

    def write(self):
        """
        write
        inserts license as a header to the file
        """
        # header
        self.file = open(self.path, "w")    
        self.file.write("test\n")
        self.file.writelines(self.contents)
        self.file.close()

class License:
    extensions = ['.h', '.c', '.cpp', '.py']
    excludedFiles = ['versionRC.h', 'resource.h', 'targetver.h', 'header.h', 'common.h']


    def __init__(self):
        self.cwd = os.getcwd()


    def __enter__(self):
        os.chdir("..")
        self.parentDir = os.listdir()


    def __exit__(self, a, b, c):
        os.chdir(self.cwd)

    def parse(self, path, directory):
        for it in directory:
            newPath = "%s\\%s" % (path, it)

            if(os.path.isdir(newPath)):
                dirs = os.listdir(newPath)
                self.parse(newPath, dirs)
                

            elif(os.path.isfile(newPath)):
                _, file_extension = os.path.splitext(newPath)
                if file_extension in self.extensions:
                    filewriter = FileWriter(newPath)
                    filewriter.write()

    def _check_excluded(self):
        pass

    def start(self):
        self.parse(os.getcwd(), self.parentDir)
        
def main():
    x = License()
    with x:
        x.start()
    
main()
