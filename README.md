Paqerduty
=========

A very simple and lightweight application that shows a notification on new
Pagerduty incidents.

Written because I wanted to be notified on my laptop, and didn't want to waste
20% of my RAM just to get that.

Usage
=====

Get an access token:
 - https://github.com/sandsmark/paqerduty.git

Dependencies:
 - Qt
 - CMake
 - A compiler

Build and run:
 - `git clone https://github.com/sandsmark/paqerduty.git`
 - `cd paqerduty`
 - `cmake .`
 - `cmake --build .`
 - `./paqerduty`
 - Enter the access token
 - Right click on the tray icon and choose "Enable autostart" to automatically
   launch when you start your computer. Completely untested, but should work on
   KDE, Gnome, other XDG compliant desktops, macOS and Windows.

Screenshots
===========

Notification:
![Notification](/screenshots/notification.png)

Tray icon when no active incidents:
![Tray icon with no active incident](/screenshots/tray-inactive.png)

Tray icon when there's an active incident:
![Tray icon with active incident](/screenshots/tray-active.png)
