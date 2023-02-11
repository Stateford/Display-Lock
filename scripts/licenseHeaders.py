"""generate license headers for all files in the project""" ""
import os
from typing import List


class License:
    extensions = [".h", ".c", ".cpp"]
    file_paths: List[str] = []

    def __init__(self):
        self.cwd = os.getcwd()
        self.ignored_files = None

    def __enter__(self):
        os.chdir("..")
        self.parent_dir = os.listdir()

    def __exit__(self, a, b, c):
        os.chdir(self.cwd)

    def parse(self, path, directory):
        for it in directory:
            new_path = os.path.join(path, it)

            if os.path.isdir(new_path):
                dirs = os.listdir(new_path)
                self.parse(new_path, dirs)

            elif os.path.isfile(new_path):
                file_path, file_extension = os.path.splitext(new_path)
                if file_extension in self.extensions and it not in self.ignored_files:
                    self.file_paths.append(f"{file_path}{file_extension}")

    def start(self):
        self.parse(os.getcwd(), self.parent_dir)


def main():
    license = License()
    license.ignored_files = [
        "version.h",
        "versionRC.h",
        "targetver.h",
        "resource.h",
        "unittest1.cpp",
    ]
    with license:
        license.start()

    with open("license.txt", "r") as file_reader:
        header = file_reader.read()

    for item in license.file_paths:
        with open(item, "r+") as file_writer:
            body = file_writer.read()
            file_writer.seek(0)
            file_writer.write(header)
            file_writer.write(body)
            file_writer.truncate()


if __name__ == "__main__":
    main()
