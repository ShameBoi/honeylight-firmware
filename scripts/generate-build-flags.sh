#!/usr/bin/env bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )";

# shellcheck source=./variables.sh
. "${SCRIPT_DIR}/variables.sh";

for key in "${!buildStringVariables[@]}"; do
    echo -n "-D""$key""=\"\\\"""${buildStringVariables[$key]}""\\\"\" ";
done;

for key in "${!buildNumericVariables[@]}"; do
    echo -n "-D""$key""=\"""${buildNumericVariables[$key]}""\" ";
done;

for key in "${!buildFlags[@]}"; do
    if [ "${buildFlags[$key]}" -eq 1 ]; then
        echo -n "-D""$key"" ";
    fi
done;

echo;
