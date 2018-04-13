# Changelog
All notable changes to this project will be documented in this file

## [1.0.5]
### Fixed
- Fixed issue where window could be resized when locked in the window
    - This could create an issue with programs running in elevated permissions

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
