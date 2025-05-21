"""
buildInc.py
-----------
automatically increments build number and configuration
"""
import sys
import os


class BuildError(Exception):
    """Build error exception"""


def write_header(file_path: str, config: str, version: str, build: str):
    """Writes the version header file"""
    print(f"Writing to path: {file_path}")
    version_split = version.split(".")

    with open(file_path, "w+", encoding="utf-8") as version_file:
        version_file.write("#pragma once\n\n")
        version_file.write(f'#define CONFIGURATION "{config}"\n')
        version_file.write(f"#define VERSION_MAJOR {int(version_split[0])}\n")
        version_file.write(f"#define VERSION_MINOR {int(version_split[1])}\n")
        version_file.write(f"#define VERSION_REVISION {int(version_split[2])}\n")
        version_file.write(f"#define VERSION_BUILD {build}\n")
        version_file.write(f'#define FILE_VERSION "{version}.{build}"\n')
        version_file.close()


def main():
    """main entry point"""
    print(sys.argv)
    if len(sys.argv) < 3:
        raise BuildError("Something went wrong with build")

    write_header(
        os.path.abspath("./src/resources/version.h"),
        sys.argv[1],
        sys.argv[2],
        sys.argv[3],
    )


if __name__ == "__main__":
    try:
        main()
    except BuildError as error:
        print(error)
        sys.exit(1)
