import os
import sys

class FileWriter:
    header = ""
    def __init__(self, path):
        self.path = path

    def __enter__(self):
        self.file = open(self.path, "r+")

    def __exit__(self, a, b, c):
        self.file.close()

    def write(self):
        body = self.file.read()
        self.file.seek(0)
        self.file.write(self.header)
        self.file.write(body)
        self.file.truncate()

class License:
    extensions = ['.h', '.c', '.cpp']
    file_paths = []

    def __init__(self):
        self.cwd = os.getcwd()
        self.ignored_files = None

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
                file_path, file_extension = os.path.splitext(newPath)
                if file_extension in self.extensions and not it in self.ignored_files:
                    self.file_paths.append(f"{file_path}{file_extension}")

    def start(self):
        self.parse(os.getcwd(), self.parentDir)

def main():
    x = License()
    x.ignored_files = [
        "version.h",
        "versionRC.h",
        "targetver.h",
        "resource.h",
        "unittest1.cpp"
    ]
    with x:
        x.start()

    with open("license.txt", "r") as file_reader:
        FileWriter.header = file_reader.read()

    for item in x.file_paths:
        with FileWriter(item) as file_writer:
            file_writer.write()

if __name__ == "__main__":
    main()
