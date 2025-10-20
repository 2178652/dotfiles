#!/bin/bash

# Path to your dotfiles repo
DOTFILES="$HOME/dotfiles/.config"
SSHCONFIG="$HOME/dotfiles/.ssh/config"

# List of recommended config folders to symlink
CONFIGS=(
  alacritty
  btop
  code
  copyq
  eza
  fastfetch
  fcitx
  fcitx5
  fontconfig
  git
  gtk-3.0
  gtk-4.0
  hypr
  jetbrains
  kitty
  lazygit
  mako
  nvim
  waybar
  yay
)

# Link bashrc
if [ -f ~/dotfiles/.bashrc ]; then
  rm -f ~/.bashrc
  ln -s ~/dotfiles/.bashrc ~/.bashrc
fi

echo "🔗 Linking config folders from $DOTFILES to ~/.config..."

for folder in "${CONFIGS[@]}"; do
  SRC="$DOTFILES/$folder"
  DEST="$HOME/.config/$folder"

  if [ -d "$SRC" ]; then
    if [ -L "$DEST" ] || [ -d "$DEST" ]; then
      echo "⚠️ Removing existing $DEST"
      rm -rf "$DEST"
    fi

    echo "✅ Linking $folder"
    ln -s "$SRC" "$DEST"
  else
    echo "❌ Skipping $folder (not found in dotfiles)"
  fi
done

# Link SSH config
echo "🔐 Linking SSH config..."
mkdir -p "$HOME/.ssh"
if [ -f "$SSHCONFIG" ]; then
  if [ -L "$HOME/.ssh/config" ] || [ -f "$HOME/.ssh/config" ]; then
    echo "⚠️ Removing existing ~/.ssh/config"
    rm -f "$HOME/.ssh/config"
  fi
  ln -s "$SSHCONFIG" "$HOME/.ssh/config"
  chmod 600 "$HOME/.ssh/config"
  echo "✅ SSH config linked"
else
  echo "❌ SSH config not found in dotfiles"
fi

# Link bakc the theme for lazyvim with the Omarchy theme
ln -sf ~/.config/omarchy/current/theme/neovim.lua ~/.config/nvim/lua/plugins/theme.lua

echo "🎉 All configs linked!"
