# Changelog
All notable changes to this project will be documented in this file

## [1.0.8]
### Fixed
- Fixed an issue where borderless windows without fullscreen would cause the game to not work properly
    - This was caused by not resizing the window after removing the title bar

### Removed
- Removed recommendation to using only borderless window with fullscreen


## [1.0.7]
### Added
- Added recommendation to using only borderless window with fullscreen

### Fixed
- Fixed an issue where background processes would appear in the selection box
- Fixed an issue where display would not handle reading files properly
    - This caused the program to not open properly on occasion
    - This caused the program to not save settings on occasion

## [1.0.6]
### Added
- Added option to bring selected window to the foreground

### Fixed
- Fixed an issue where when Display Lock was closed before pressing stop, the window would not revert
- Fixed an issue where the window that was locked being closed before pressing stop would not unlock menu items
- Fixed an issue where the settings window would not close from the title bar
- Fixed an issue where the program was not shutting down properly

## [1.0.5]
### Added
- Added option for borderless windowed mode
- Added option for fullscreen mode

### Fixed
- Fixed issue where window could be resized when locked in the window
    - This could create an issue with programs running in elevated permissions

### Removed
- Removed the ability for display lock to select the itself

## [1.0.4]
### Added
- Added Settings Menu
	- Option to toggle minimizing on start
- Added Config File
	- Config File saves to `appdata/roaming/DisplayLock/settings.DLOCK`

### Fixed
- Fixed issue when using alt+tab to switch windows would change the currently selected window in the drop down list

### Removed
- Unessecary symbols that would use more RAM

## [1.0.3]
### Added
- Added meta tags for windows

### Fixed
- Fixed issue when the program closed before unlocking the cursor
- Fixed issue where the program would not shut down properly
