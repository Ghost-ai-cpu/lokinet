FROM compose-base:latest

ENV WORKTIPSNET_NETID=docker

COPY ./docker/compose/bootstrap.ini /root/.worktipsnet/worktipsnet.ini

CMD ["/worktipsnet"]
EXPOSE 1090/udp 1190/tcp
