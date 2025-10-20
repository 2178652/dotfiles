#!/bin/bash

echo "🚀 Starting dotfiles setup..."

# Install yay (AUR helper)
bash ./install-aur-helper.sh

# Install official repo packages
bash ./install-pacman-packages.sh

# Install AUR packages
bash ./install-aur-packages.sh

#Needed for esp-idf
sudo chown -R cgar:cgar /opt/esp-idf
sudo chmod -R u+rwX,go+rX /opt/esp-idf
bash /opt/esp-idf/install.sh
bash /opt/esp-idf/export.sh
echo "Setup for ESP-idf completed"

echo "🎉 Dotfiles setup complete."
