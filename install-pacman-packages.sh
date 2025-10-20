#!/bin/bash

# Install packages from pacman-packages.txt
echo "📦 Installing official repo packages..."
while IFS= read -r pkg || [[ -n "$pkg" ]]; do
  if [[ -n "$pkg" && "$pkg" != \#* ]]; then
    sudo pacman -Sy --noconfirm --needed "$pkg"
  fi
done < pacman-packages.txt
