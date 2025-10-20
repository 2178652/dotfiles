#!/bin/bash

# Create ~/.ssh if it doesn't exist
mkdir -p ~/.ssh
chmod 700 ~/.ssh

# GitHub key
if [ ! -f ~/.ssh/id_ed25519_github ]; then
  echo "🔐 Generating GitHub SSH key..."
  ssh-keygen -t ed25519 -C "2178652@cstjean.qc.ca" -f ~/.ssh/id_ed25519_github -N ""
else
  echo "✅ GitHub SSH key already exists."
fi

# Bitbucket key
if [ ! -f ~/.ssh/id_ed25519_bitbucket ]; then
  echo "🔐 Generating Bitbucket SSH key..."
  ssh-keygen -t ed25519 -C "2178652@cstjean.qc.ca" -f ~/.ssh/id_ed25519_bitbucket -N ""
else
  echo "✅ Bitbucket SSH key already exists."
fi

# Set permissions
chmod 600 ~/.ssh/id_ed25519_github
chmod 600 ~/.ssh/id_ed25519_bitbucket

ssh-add ~/.ssh/id_ed25519_bitbucket
ssh-add ~/.ssh/id_ed25519_github

echo "📋 Public keys:"
echo "GitHub:"
cat ~/.ssh/id_ed25519_github.pub
echo ""
echo "Bitbucket:"
cat ~/.ssh/id_ed25519_bitbucket.pub
