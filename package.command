#!/usr/bin/env sh

rm worktest.zip >/dev/null 2>&1
zip -r worktest.zip ./code ./tools *.bat *.command *.md *.lua *.txt *.sh