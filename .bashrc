# All the default Omarchy aliases and functions
# (don't mess with these directly, just overwrite them here!)
source ~/.local/share/omarchy/default/bash/rc

#sshkey
#eval "$(keychain --quiet id_ed25519)"
#source ~/.keychain/$HOSTNAME-sh

# Add your own exports, aliases, and functions here.
#
# Make an alias for invoking commands you use constantly
# alias p='python'
alias sail='sh $([ -f sail ] && echo sail || echo vendor/bin/sail)'
export PATH="/home/cgar/bin:$PATH"
export IDF_PATH=/opt/esp-idf
