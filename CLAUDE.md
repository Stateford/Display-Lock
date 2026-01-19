# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Display-Lock is a lightweight Windows application that locks the cursor to a selected window. Written primarily in C using the Win32 API, it's designed for gamers and multi-monitor users who need to keep their cursor confined to a specific window.

## Build Commands

```bash
# Configure and build (Release)
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Configure and build (Debug)
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug

# Run tests (requires Conan for GTest dependency)
pip install conan==1.59.0
ctest --test-dir build -C Release
```

The build requires Visual Studio 2022 (MSVC) on Windows. Conan package manager is used to fetch Google Test for unit testing.

## Architecture

**Component Library (`src/components/`)**: Core functionality as a static library (`display_lock_components`)
- `win.c` - Cursor locking logic and window manipulation
- `settings.c` - Binary config file persistence (`.DLOCK` format in `%APPDATA%/DisplayLock/`)
- `applications.c` - Application whitelist management and monitoring thread
- `notify.c` - System tray icon and notifications
- `update.c` - Version checking via HTTP
- `menu.c` - Menu UI handling

**Main Application (`src/`)**: Win32 dialog-based UI
- `main.c` - Entry point, window class registration, message loop
- `ui.c` - Tab view rendering (Window Select, Settings, Applications)
- `procedures/` - Dialog procedure handlers

**Key Data Structures (`include/common.h`)**:
- `SETTINGS` - User preferences (minimize on start, fullscreen, borderless, etc.)
- `WINDOWLIST` - Array of up to 35 windows for selection
- `APPLICATION_LIST` - Whitelisted applications for automatic cursor locking

**Threading Model**: Cursor locking and application monitoring run in dedicated threads, synchronized via mutex (`DLockApplicationMutex`).

## Testing

Tests use Google Test (fetched via Conan). Test files are in `tests/` with test resources copied to the build directory.

```bash
# Run a single test
ctest --test-dir build -C Release -R SettingsTest
```

## Contributing Guidelines

- Work on `develop` branch for features, `master` for hotfixes
- Avoid external dependencies (keep it lightweight)
- Do not manually change version numbers or build scripts
- Update CHANGELOG.md with changes
- All builds and tests must pass before merge
