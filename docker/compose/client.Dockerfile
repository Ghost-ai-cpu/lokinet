FROM compose-base:latest

COPY ./docker/compose/client.ini /root/.worktipsnet/worktipsnet.ini

CMD ["/worktipsnet"]
EXPOSE 1090/udp 1190/tcp
