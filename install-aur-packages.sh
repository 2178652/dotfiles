#!/bin/bash

# Check if yay is installed
if ! command -v yay &> /dev/null; then
  echo "Error: yay is not installed. Run install-aur-helper.sh first."
  exit 1
fi

# Install packages from aur-packages.txt
echo "Installing AUR packages..."
while IFS= read -r pkg || [[ -n "$pkg" ]]; do
  if [[ -n "$pkg" && "$pkg" != \#* ]]; then
    yay -S --noconfirm "$pkg"
  fi
done < aur-packages.txt