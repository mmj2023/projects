#!/usr/bin/env bash

# Function to display usage information
usage() {
 echo "Usage: $(basename "$0") <language_name>"
 echo "Create a new project directory for the specified language."
 echo ""
 echo "Arguments:"
 echo "  <language_name>  The name of the programming language (e.g. C, Rust, Cpp, Python)."
 echo ""
 echo "Options:"
 echo "  --help           Display this help message and exit."
}

# Check if a language name is provided
if [[ -z "$1" ]]; then
 echo "Error: Missing language name."
 usage
 exit 1
fi

# Check for --help flag
if [[ "$1" == "--help" ]]; then
 usage
 exit 0
else
 lang=$1
fi

mkdir -p "${lang}_Projects"
