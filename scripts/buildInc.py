"""
buildInc.py
-----------
automatically increments build number and configuration
"""
import sys
import os


def writeHeader(file_path: str, config: str, version: str, build: str):
    print("Writing to path: %s\n" % file_path)
    version_split = version.split(".")

    with open(file_path, "w+") as version_file:
        version_file.write("#pragma once\n\n")
        version_file.write(f'#define CONFIGURATION "{config}"\n')
        version_file.write(f"#define VERSION_MAJOR {int(version_split[0])}\n")
        version_file.write(f"#define VERSION_MINOR {int(version_split[1])}\n")
        version_file.write(f"#define VERSION_REVISION {int(version_split[2])}\n")
        version_file.write(f"#define VERSION_BUILD {build}\n")
        version_file.write(f'#define FILE_VERSION "{version}.{build}"\n')
        version_file.close()


def main():
    print(sys.argv)
    if len(sys.argv) < 3:
        raise BaseException("Something went wrong with build")

    writeHeader(
        os.path.abspath("./Display-Lock/resources/version.h"),
        sys.argv[1],
        sys.argv[2],
        sys.argv[3],
    )


if __name__ == "__main__":
    main()
