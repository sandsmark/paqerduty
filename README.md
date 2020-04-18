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

Build and run:
 - `git clone https://github.com/sandsmark/paqerduty.git`
 - `cd paqerduty`
 - `qmake`
 - `make`
 - `./paqerduty`
 - Enter the access token
 - Right click on the tray icon and choose "Enable autostart" to automatically
   launch when you start your computer. Completely untested, but should work on
   KDE, Gnome, other XDG compliant desktops, macOS and Windows.
