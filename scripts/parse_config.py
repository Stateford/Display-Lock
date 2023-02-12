"""
Parse Config

Reads binary from a DLOCK config file and prints it in a human readable format
"""
import struct
import sys
from typing import Optional


class ParseException(Exception):
    """parse exception"""


# config for config version 5
class ConfigParser:
    """config parser"""
    # pylint: disable=too-many-instance-attributes

    def __init__(self, path: str):
        self.path = path
        self.header: Optional[str] = None
        self.version: Optional[int] = None
        self.minimize = None
        self.foreground = None
        self.borderless = None
        self.full_screen = None
        self.contents: Optional[bytes] = None
        self.num_of_hotkeys = None
        self.hotkey = None
        self.hotkey_mod = None

    def open_file(self):
        """opens a file and stores it's contents in self.contents"""
        with open(self.path, "rb") as file:
            self.contents = file.read()
            file.close()

    # parses the binary string
    def parse(self):
        """parse the binary string"""
        if not self.contents:
            raise ParseException("No contents")

        self.header = struct.unpack("@5s", self.contents[0:5])[0]
        self.version = struct.unpack(">i", self.contents[5:9])[0]
        self.minimize = struct.unpack(">i", self.contents[9:13])[0]
        self.foreground = struct.unpack(">i", self.contents[13:17])[0]
        self.borderless = struct.unpack(">i", self.contents[17:21])[0]
        self.full_screen = struct.unpack(">i", self.contents[21:25])[0]

        if self.version > 5:
            self.num_of_hotkeys = struct.unpack(">i", self.contents[25:29])[0]
            self.hotkey_mod = struct.unpack(">i", self.contents[29:33])[0]
            self.hotkey = struct.unpack(">i", self.contents[33:37])[0]

    # prints results
    def print_results(self):
        """print results"""
        print(self.header)
        print(self.version)
        # check if string is equal to 1
        # if it is, returns True
        # else it returns False
        print(self.minimize == "1")
        print(self.foreground == "1")
        print(self.borderless == "1")
        print(self.full_screen == "1")

        if self.version and self.version > 5:
            print(self.num_of_hotkeys)
            print(self.hotkey_mod)
            print(self.hotkey)

    # start parsing config file
    def start(self):
        """start parsing"""
        self.open_file()
        self.parse()
        self.print_results()


def main():
    """main entry point"""
    if len(sys.argv) < 2:
        raise ParseException("Pass an argument in")

    output = ConfigParser(sys.argv[1])
    output.start()


if __name__ == "__main__":
    main()
