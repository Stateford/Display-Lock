# Parse Config
# ================
# Reads binary from a DLOCK config file and prints it in a human readable format
import struct
import sys


# config for config version 5
class Config:
    # constructor, takes a string path as an argument
    def __init__(self, path: str):
        self.path = path
        self.header = None
        self.version = None
        self.minimize = None
        self.foreground = None
        self.borderless = None
        self.full_screen = None
        self.index = 0
        self.file = None
        self.contents = None
        self.num_of_hotkeys = None
        self.hotkey = None
        self.hotkey_mod = None

    # opens a file and stores it's contents in self.contents
    def openFile(self):
        self.file = open(self.path, "rb")
        self.contents = self.file.read()
        self.file.close()

    # parses the binary string
    def parse(self):
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
    def printResults(self):
        print(self.header)
        print(self.version)
        # check if string is equal to 1
        # if it is, returns True
        # else it returns False
        print(self.minimize == "1")
        print(self.foreground == "1")
        print(self.borderless == "1")
        print(self.full_screen == "1")

        if self.version > 5:
            print(self.num_of_hotkeys)
            print(self.hotkey_mod)
            print(self.hotkey)

    # start parsing config file
    def start(self):
        self.openFile()
        self.parse()
        self.printResults()


def main():
    if len(sys.argv) < 2:
        raise Exception("Pass an argument in")

    output = Config(sys.argv[1])
    output.start()


main()
