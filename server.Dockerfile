FROM ghcr.io/cavoke-project/cavoke-ci:server

# Build arguments. Use `--build-arg` in docker run
ARG SERVER_PORT=8080

EXPOSE $SERVER_PORT

# Cavoke server
ENV MYSERVER_ROOT="$IROOT/demo_orm_drogon/"
ADD ./ $MYSERVER_ROOT
# Install server
WORKDIR $MYSERVER_ROOT
RUN mkdir -p cmake-build-docker && \
    cmake . -B cmake-build-docker && \
    make -C cmake-build-docker install
WORKDIR /
RUN rm -rf $MYSERVER_ROOT

ENTRYPOINT ["/usr/local/bin/demo_orm_drogon"]
