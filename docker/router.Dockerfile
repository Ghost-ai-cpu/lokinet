ARG bootstrap="https://i2p.rocks/i2procks.signed"
FROM alpine:edge as builder

RUN apk update && \
    apk add build-base cmake git libcap-dev libcap-static libuv-dev libuv-static curl ninja bash binutils-gold curl-dev

WORKDIR /src/
COPY . /src/

RUN make NINJA=ninja STATIC_LINK=ON BUILD_TYPE=Release DOWNLOAD_SODIUM=ON
RUN ./worktipsnet-bootstrap ${bootstrap}

FROM alpine:latest

COPY worktipsnet-docker.ini /root/.worktipsnet/worktipsnet.ini
COPY --from=builder /src/build/daemon/worktipsnet .
COPY --from=builder /root/.worktipsnet/bootstrap.signed /root/.worktipsnet/

CMD ["./worktipsnet"]
EXPOSE 1090/udp 1190/tcp
