#!/bin/zsh
scp -P 2222 CVE-2019-8605/exp seo@192.168.22.130:~/vulntest/CVE-2019-8605
ssh -p 2222 seo@192.168.22.130 'sync; sync; sync; sync; sync; sync; sync; sync; sync;'