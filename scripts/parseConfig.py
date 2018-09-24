# Parse Config
# ================
# Reads binary from a DLOCK config file and prints it in a human readable format

import struct

# config for config version 5
class ConfigV5:
    # constructor, takes a string path as an argument
    def __init__(self, path):
        self.path = path
        self.header = None
        self.version = None
        self.minimize = None
        self.foreground = None
        self.borderless = None
        self.fullScreen = None
        self.index = 0
        self.file = None
        self.contents = None

    # opens a file and stores it's contents in self.contents
    def openFile(self):
        self.file = open(self.path, "rb")
        self.contents = self.file.read()
        self.file.close()

    # parses the binary string
    def parse(self):
        self.header = struct.unpack('@5s', self.contents[0:5])[0]
        self.version = struct.unpack('>i', self.contents[5:9])[0]
        self.minimize = struct.unpack('>i', self.contents[9:13])[0]
        self.foreground = struct.unpack('>i', self.contents[13:17])[0]
        self.borderless = struct.unpack('>i', self.contents[17:21])[0]
        self.fullScreen = struct.unpack('>i', self.contents[21:25])[0]

    # prints results
    def printResults(self):
        print(self.header)
        print(self.version)
        # check if string is equal to 1
        # if it is, returns True
        # else it returns False
        print(self.minimize == '1')
        print(self.foreground == '1')
        print(self.borderless == '1')
        print(self.fullScreen == '1')

    # start parsing config file
    def start(self):
        self.openFile()
        self.parse()
        self.printResults()
        