#!/usr/bin/env bash
#/***********************************************************************
# Freeciv - Copyright (C) 2017-2023
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2, or (at your option)
#   any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#***********************************************************************/

# rulesets_autohelp.sh [ruleset]...
# Exits with 0 when each specified ruleset or, if no rulesets are specified,
# each ruleset that is developed with Freeciv, are able to automatically
# generate help texts. Exits with 1 if autohelp fails for any ruleset.

if test x$1 = x ; then
  # Try to load all rulesets that are developed with Freeciv
  rulesets="$(cat /Users/karman/Desktop/projects/mvp/freeciv-3.1.5/tests/rs_test_res/ruleset_list_dist.txt)"
  optional="$(cat /Users/karman/Desktop/projects/mvp/freeciv-3.1.5/tests/rs_test_res/ruleset_list_opt.txt)"
  for opt in $optional ; do
    if test -d "/Users/karman/Desktop/projects/mvp/freeciv-3.1.5/data/$opt" ; then
      rulesets="$rulesets $opt"
    fi
  done
else
  # Try to load the specified ruleset(s)
  rulesets=$@
fi

if test "x$FREECIV_DATA_PATH" = "x" ; then
  FREECIV_DATA_PATH=".:data"
fi
FREECIV_DATA_PATH="${FREECIV_DATA_PATH}:/Users/karman/Desktop/projects/mvp/freeciv-3.1.5/data:/Users/karman/Desktop/projects/mvp/freeciv-3.1.5/data"
export FREECIV_DATA_PATH

for ruleset in $rulesets; do
  echo "Loading $ruleset"
  /Users/karman/Desktop/projects/mvp/freeciv-3.1.5/tools/freeciv-manual -r $ruleset || exit 1
done

echo "The automatic help text generation works for all tested rulesets."
exit 0
