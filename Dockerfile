FROM archlinux:base-devel

RUN mkdir /work
WORKDIR /work

ENTRYPOINT pwd && ls && make clean && make 
