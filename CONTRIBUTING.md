# Contributing

When contributing to this repository, please first discuss the change you wish to make via issue or PR.

## Things to Avoid

1. Please do not add external dependencies or libraries without discussing them first.
2. The goal of this program is keep the program as lightweight as possible. Try to avoid things that might add uneeded bloat.
3. Do not manually increase the version number, or touch the build scripts unless it is related to the issue.
4. Do not make machine specific changes to solutions or projects. The project should be able to build on any machine with the files provided.
5. Do not use a build system other than MSBUILD.
6. Code Style is not strict, but try to follow what is already in the project.

## Pull Request Process

1. Coordinate work being done via issues or PR before work is done.
2. All work must be done on the develop branch or a branch checked out from develop.
3. Update the [CHANGELOG](CHANGELOG.md) with details of changes made. 
4. If tests can be added, consider adding them.
5. All builds must actually build, pass pre-scripts, and all tests before being merged.

