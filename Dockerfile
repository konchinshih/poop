FROM archlinux:base-devel

WORKDIR /work

ENTRYPOINT pwd && ls 
