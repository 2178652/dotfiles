# Dotfiles

This repository contains my personal configuration files for Linux systems (Arch-based, including Omarchy). It includes symlinks, package manifests, SSH setup, and automation scripts for a reproducible environment.

## Repo Structure

dotfiles/
├── .config/              # App configs (nvim, kitty, waybar, etc.)
├── .ssh/                 # SSH config (no private keys)
├── install-pacman-packages.sh
├── install-aur-packages.sh
├── generate-ssh-keys.sh
├── link-config.sh
├── setup.sh
├── pacman-packages.txt
├── aur-packages.txt
└── README.md

## Setup Instructions

### 1. Clone the repo
git clone git@github.com:yourusername/dotfiles.git ~/dotfiles
cd ~/dotfiles

### 2. Run the setup script
bash setup.sh

This will:
- Install official packages from `pacman-packages.txt`
- Install AUR packages from `aur-packages.txt`
- Symlink all config folders into `~/.config`
- Symlink SSH config into `~/.ssh/config`
- Generate SSH keys for GitHub and Bitbucket (if missing)

## SSH Setup

After running `generate-ssh-keys.sh`, copy the public keys to your Git services:

cat ~/.ssh/id_ed25519_github.pub
cat ~/.ssh/id_ed25519_bitbucket.pub

Then paste them into:
- GitHub → Settings → SSH keys
- Bitbucket → Personal settings → SSH keys

## Notes

- **Do not store private keys** in this repo.
- **All configs are symlinked**, not copied — changes in `dotfiles/.config` reflect live in `~/.config`.
- You can extend `pacman-packages.txt` and `aur-packages.txt` to install more tools.
- Add your own scripts or environment files as needed.

## To Do

- [ ] Add backup logic to `link-config.sh`
- [ ] Add validation script (`validate.sh`)
- [ ] Add language-specific setup scripts (Node, Python, Rust)
