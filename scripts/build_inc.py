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
    print(f"Current working directory: {os.getcwd()}")
    print(f"Script directory: {os.path.dirname(os.path.abspath(__file__))}")
    print(f"Arguments: {sys.argv}")
    
    if len(sys.argv) < 3:
        raise BuildError("Something went wrong with build")

    # Find the project root (where we expect src/resources to be)
    script_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(script_dir)  # Assuming script is in a scripts/ directory at project root
    
    # Try different possible locations for the version.h file
    possible_paths = [
        os.path.join(project_root, "src", "resources", "version.h"),  # Project root/src/resources/version.h
        os.path.join(project_root, "Display-Lock", "resources", "version.h"),  # Project root/Display-Lock/resources/version.h
        os.path.join(os.getcwd(), "src", "resources", "version.h"),  # Current dir/src/resources/version.h
        os.path.join(os.getcwd(), "Display-Lock", "resources", "version.h"),  # Current dir/Display-Lock/resources/version.h
    ]
    
    # Print all possible paths for debugging
    print("Searching for version.h in the following locations:")
    for path in possible_paths:
        print(f"- {path} ({os.path.exists(path)})")
        
    # Try to find the first existing directory path
    for path in possible_paths:
        dir_path = os.path.dirname(path)
        if os.path.exists(dir_path):
            print(f"Found resources directory at: {dir_path}")
            use_path = path
            break
    else:
        # If we can't find any of the directories, default to the new path
        use_path = os.path.join(project_root, "src", "resources", "version.h")
        print(f"None of the directories found, defaulting to: {use_path}")
    
    write_header(
        use_path,
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
    except Exception as e:
        print(f"Unexpected error: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)
