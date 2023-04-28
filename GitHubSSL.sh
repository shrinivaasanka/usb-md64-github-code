#!/bin/bash

echo $SSL_AUTH_SOCK
git remote set-url origin git@github.com:shrinivaasanka/usb-md64-github-code.git
eval "$(ssh-agent -s)"
ssh-add /root/.ssh/id_ed25519_gitlab
ssh git@github.com
git push

