"""parse application config file"""
import os
import sys
from io import BytesIO
from typing import Self, List, Iterator
import argparse


class ApplicationSettings:
    """Applications settings data"""

    @classmethod
    def from_bytes(cls, buffer: BytesIO) -> Self:
        """create from bytes"""
        application_name = buffer.read(260 * 2).decode("utf-16")
        application_path = buffer.read(260 * 2).decode("utf-16")
        fullscreen = int.from_bytes(
            buffer.read(4), byteorder=sys.byteorder, signed=True
        )
        borderless = int.from_bytes(
            buffer.read(4), byteorder=sys.byteorder, signed=True
        )
        enabled = int.from_bytes(buffer.read(4), byteorder=sys.byteorder, signed=True)
        return cls(
            application_name=application_name,
            application_path=application_path,
            fullscreen=fullscreen,
            borderless=borderless,
            enabled=enabled,
        )

    @property
    def fullscreen(self) -> bool:
        """fullscreen"""
        return self._fullscreen == 1

    @property
    def borderless(self) -> bool:
        """borderless"""
        return self._borderless == 1

    @property
    def enabled(self) -> bool:
        """enabled"""
        return self._enabled == 1

    def __init__(  # pylint: disable=too-many-arguments
        self,
        application_name: str,
        application_path: str,
        fullscreen: int,
        borderless: int,
        enabled: int,
    ):
        self.application_name = application_name
        self.application_path = application_path
        self._fullscreen = fullscreen
        self._borderless = borderless
        self._enabled = enabled

    def __repr__(self) -> str:
        return (
            f"{self.application_name}\n"
            f"{self.application_path}\n"
            f"{self.fullscreen}\n"
            f"{self.borderless}\n"
            f"{self.enabled}\n"
            "-----------------"
        )


class ApplicationList:
    """List of applications settings"""

    @classmethod
    def _from_bytes(cls, buffer: BytesIO) -> Self:
        """create from bytes"""
        count = int.from_bytes(buffer.read(4), byteorder=sys.byteorder, signed=True)
        applications: List[ApplicationSettings] = []

        for _ in range(count):
            applications.append(ApplicationSettings.from_bytes(buffer))

        return cls(count=count, applications=applications)

    @classmethod
    def from_file(cls, file_path: str) -> Self:
        """create from file"""
        with open(file_path, "rb") as file:
            with BytesIO(file.read()) as buffer:
                return cls._from_bytes(buffer)

    def __init__(self, count: int, applications: List[ApplicationSettings]):
        self.count = count
        self.applications = applications

    def __iter__(self) -> Iterator[ApplicationSettings]:
        return iter(self.applications)


def main():
    """main function"""

    parser = argparse.ArgumentParser("parse application config file")
    parser.add_argument("file", type=str)

    args = parser.parse_args()

    if not args.file:
        print("file not specified")
        sys.exit(1)

    if not os.path.exists(args.file):
        print(f"file not found: {args.file}")
        sys.exit(1)

    applications = ApplicationList.from_file(args.file)

    for application in applications:
        print(application)


if __name__ == "__main__":
    main()
