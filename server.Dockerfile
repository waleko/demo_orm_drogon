FROM ghcr.io/cavoke-project/cavoke-ci:server

# Build arguments. Use `--build-arg` in docker run
ARG SERVER_PORT=8080

EXPOSE $SERVER_PORT

# Cavoke server
ENV MYSERVER_ROOT="$IROOT/demo_orm_drogon/"
ADD ./ $MYSERVER_ROOT
# Install server
RUN mkdir -p build && \
    cmake . -B build && \
    make -C build install
WORKDIR /
RUN rm -rf $MYSERVER_ROOT

ENTRYPOINT ["/usr/local/bin/demo_orm_drogon"]
