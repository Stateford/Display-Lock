"""generate license headers for all files in the project""" ""
import os
from typing import List, Optional


class License:
    """license class"""

    extensions = [".h", ".c", ".cpp"]
    file_paths: List[str] = []

    def __init__(self, ignored_files: List[str]):
        self.cwd = os.getcwd()
        self.ignored_files = ignored_files
        self.parent_dir: Optional[List[str]] = None

    def __enter__(self):
        os.chdir("..")
        self.parent_dir = os.listdir()

    def __exit__(self, *_):
        os.chdir(self.cwd)

    def parse(self, path: str, directories: List[str]):
        """parse directory"""
        for directory in directories:
            new_path = os.path.join(path, directory)

            if os.path.isdir(new_path):
                dirs = os.listdir(new_path)
                self.parse(new_path, dirs)

            elif os.path.isfile(new_path):
                file_path, file_extension = os.path.splitext(new_path)
                if (
                    file_extension in self.extensions
                    and directory not in self.ignored_files
                ):
                    self.file_paths.append(f"{file_path}{file_extension}")

    def start(self):
        """start parsing"""
        if self.parent_dir is not None:
            self.parse(os.getcwd(), self.parent_dir)


def main():
    """main entry point"""
    license_parser = License(
        ignored_files=[
            "version.h",
            "versionRC.h",
            "targetver.h",
            "resource.h",
            "unittest1.cpp",
        ]
    )
    with license_parser:
        license_parser.start()

    with open("license.txt", "r", encoding="utf-8") as file_reader:
        header = file_reader.read()

    for item in license_parser.file_paths:
        with open(item, "r+", encoding="utf-8") as file_writer:
            body = file_writer.read()
            file_writer.seek(0)
            file_writer.write(header)
            file_writer.write(body)
            file_writer.truncate()


if __name__ == "__main__":
    main()
