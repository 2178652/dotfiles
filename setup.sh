#!/bin/bash

echo "🚀 Starting dotfiles setup..."

# Install yay (AUR helper)
bash ./install-aur-helper.sh

# Install official repo packages
bash ./install-pacman-packages.sh

# Install AUR packages
bash ./install-aur-packages.sh

echo "🎉 Dotfiles setup complete."