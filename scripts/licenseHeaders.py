import os

class FileWriter:

    def __init__(self, path):
        self.path = path

    def __enter__(self):
        self.file = open(self.path, "w")

    def __exit__(self, a, b, c):
        self.file.close()

    def write(self):
        self.file.write("test")

class License:
    extensions = ['.h', '.c', '.cpp']

    def __init__(self):
        self.cwd = os.getcwd()

    def __enter__(self):
        os.chdir("..")
        self.parentDir = os.listdir()

    def __exit__(self, a, b, c):
        os.chdir(self.cwd)

    def parse(self, path, directory):
        for it in directory:
            newPath = "%s\\%s" % (path, it)
            #print(newPath)

            if(os.path.isdir(newPath)):
                dirs = os.listdir(newPath)
                self.parse(newPath, dirs)
                

            elif(os.path.isfile(newPath)):
                _, file_extension = os.path.splitext(newPath)
                if file_extension in self.extensions:
                    print(it)

    def start(self):
        self.parse(os.getcwd(), self.parentDir)
        
def main():
    x = License()
    with x:
        x.start()
    
main()