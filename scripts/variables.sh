#!/usr/bin/env bash

declare -A buildStringVariables=(
    ["PROGRAM_NAME"]="Honeylight Firmware"
    ["GIT_REVISION"]="$(git rev-parse HEAD)"
    ["GIT_SHORT_REVISION"]="$(git rev-parse --short=8 HEAD)"
    ["GIT_BRANCH"]="$(git rev-parse --abbrev-ref HEAD)"
    ["BUILD_DATE"]="$(date -u "+%Y-%m-%d")"
    ["BUILD_TIME"]="$(date -u "+%T")"
    ["BUILD_NUMBER"]="${BUILD_NUMBER:-DEV}"
    ["VERSION_STRING"]="${VERSION_MAJOR:-0}.${VERSION_MINOR:-0}.${VERSION_PATCH:-0}-${BUILD_NUMBER:-DEV}"
);

declare -A buildNumericVariables=(
    ["VERSION_MAJOR"]="${VERSION_MAJOR:-0}"
    ["VERSION_MINOR"]="${VERSION_MINOR:-0}"
    ["VERSION_PATCH"]="${VERSION_PATCH:-0}"
);

declare -A buildFlags=(
    ["HONEYLIGHT_DEBUG"]=1
);

export buildStringVariables buildNumericVariables buildFlags;
