import os
import sys

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
        header = file_reader.read()

    for item in x.file_paths:
        with open(item, "r+") as file_writer:
            body = file_writer.read()
            file_writer.seek(0)
            file_writer.write(header)
            file_writer.write(body)
            file_writer.truncate()

if __name__ == "__main__":
    main()
