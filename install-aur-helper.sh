#!/bin/bash

# Install yay if not already installed
if ! command -v yay &> /dev/null; then
  echo "Installing yay (AUR helper)..."
  git clone https://aur.archlinux.org/yay.git /tmp/yay
  cd /tmp/yay || exit
  makepkg -si --noconfirm
  cd - || exit
else
  echo "yay is already installed."
fi