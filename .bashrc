# All the default Omarchy aliases and functions
# (don't mess with these directly, just overwrite them here!)
source ~/.local/share/omarchy/default/bash/rc

#sshkey
eval "$(keychain id_ed25519_bitbucket id_ed25519_github id_ed25519_digitalocean)"
source ~/.keychain/$HOSTNAME-sh

# Add your own exports, aliases, and functions here.
#
# Make an alias for invoking commands you use constantly
# alias p='python'
# Silent ESP-IDF alias
alias gidf='[ -f "/home/cgar/esp-idf/export.sh" ] && . "/home/cgar/esp-idf/export.sh" >/dev/null 2>&1'
alias sail='sh $([ -f sail ] && echo sail || echo vendor/bin/sail)'
export PATH="/home/cgar/bin:$PATH"
export SSL_CERT_FILE=/etc/ssl/certs/ca-certificates.crt
